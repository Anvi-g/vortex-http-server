#include "MimeTypes.hpp"
#include <unordered_map>
#include <filesystem>

using namespace std;

string MimeTypes::get_mime_type(const string& path) {
    static const unordered_map<string, string> mime_map = {
        {".html", "text/html; charset=UTF-8"},
        {".htm",  "text/html; charset=UTF-8"},
        {".css",  "text/css"},
        {".js",   "application/javascript"},
        {".json", "application/json"},
        {".png",  "image/png"},
        {".jpg",  "image/jpeg"},
        {".jpeg", "image/jpeg"},
        {".gif",  "image/gif"},
        {".svg",  "image/svg+xml"},
        {".ico",  "image/x-icon"},
        {".txt",  "text/plain"}
    };
    string extension = filesystem::path(path).extension().string();
    auto it = mime_map.find(extension);
    if (it != mime_map.end()) {
        return it->second;
    }
    return "application/octet-stream"; // safe default for unknown binary files
}