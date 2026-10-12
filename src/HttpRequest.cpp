#include "HttpRequest.hpp"
#include <sstream>
#include <iostream>

using namespace std;

HttpRequest HttpRequest::parse(const string& raw_request) {
    HttpRequest request;
    istringstream stream(raw_request);
    string line;

    //parse the Request Line (e.g. "GET /index.html HTTP/1.1")
    if (getline(stream, line)) {
        // Strip trailing '\r' if present
        if (!line.empty() && line.back() == '\r') line.pop_back();

        istringstream line_stream(line);
        line_stream >> request.method >> request.path >> request.version;
    }

    //parse Headers until an empty line is encountered
    while (getline(stream, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();

        //empty line marks the end of headers
        if (line.empty()) break;

        //headers are "Key: Value"
        auto colon_pos = line.find(':');
        if (colon_pos != string::npos) {
            string key = line.substr(0, colon_pos);
            // Skip ": " (colon and optional space)
            size_t value_start = colon_pos + 1;
            while (value_start < line.size() && line[value_start] == ' ') {
                value_start++;
            }
            string value = line.substr(value_start);
            request.headers[key] = value;
        }
    }

    //remaining content in stream is body
    string remaining;
    while (getline(stream, line)) {
        request.body += line + "\n";
    }

    return request;
}
