#pragma once

#include <string>
#include <unordered_map>

class HttpResponse {
public:
    int status_code;
    std::string status_text;
    std::unordered_map<std::string, std::string> headers;
    std::string body;

    HttpResponse(int code = 200, const std::string& text = "OK");

    void set_header(const std::string& key, const std::string& value);
    void set_body(const std::string& content, const std::string& content_type = "text/plain");

    // Serialize response object to valid HTTP wire format
    std::string to_string() const;
};