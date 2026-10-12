#pragma once

#include <string>
#include <unordered_map>

using namespace std;

class HttpResponse{
    public:
    int status_code;
    string status_text;
    unordered_map<string, string> headers;
    string body;

    HttpResponse(int code = 200, const string& text = "OK");

    void set_header(const string& key, const string& value);
    void set_body(const string& content, const string& content_type = "text/plain");

    //serialize response object to valid HTTP wire format
    string to_string() const;
};