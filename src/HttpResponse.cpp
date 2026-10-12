#include "HttpResponse.hpp"

#include <sstream>

using namespace std;

HttpResponse::HttpResponse(int code, const string& text):status_code(code), status_text(text){
    // def headers
    set_header("Connection", "close");
    set_header("Server", "Vortex/0.1.0");
} 

void HttpResponse::set_header(const string& key, const string& value){
    headers[key] = value;
}

void HttpResponse::set_body(const string& content, const string& content_type){
    body = content;
    set_header("Content-Type", content_type);
    set_header("Content-Length", std::to_string(body.size()));
}

string HttpResponse::to_string() const {
    ostringstream response;

    // status line
    response << "HTTP/1.1 " << status_code << " " << status_text << "\r\n";

    //headers
    for(const auto& [key, value] : headers){
        response << key << ": " << value << "\r\n";
    }

    // delimiter between headers and body
    response << "\r\n";
    response << body;

    return response.str();
}