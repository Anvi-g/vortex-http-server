#pragma once

#include <string>
#include <unordered_map>

using namespace std;

class HttpRequest {
public:
    string method;
    string path;
    string version;
    unordered_map<string, string> headers;
    string body;

    // Factory method: parses a raw HTTP request string
    static HttpRequest parse(const string& raw_request);
};
