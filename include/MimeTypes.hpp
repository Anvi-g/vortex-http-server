#pragma once

#include <string>

using namespace std;

class MimeTypes {
public:
    static string get_mime_type(const string& path);
};
