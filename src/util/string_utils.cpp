/*
MIT License

Copyright (c) 2023 dodoman8067

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <mcsm/util/string_utils.h>

bool mcsm::startsWith(const std::string& str, const std::string& value){
    return str.rfind(value, 0) == 0;
}

bool mcsm::endsWith(const std::string& str, const std::string& value){
    // Ensure that `value` is not longer than `str` to avoid out-of-bounds comparisons
    if(str.length() < value.length()){
        return false;
    }
    return str.compare(str.length() - value.length(), value.length(), value) == 0;
}

bool mcsm::isWhitespaceOrEmpty(const std::string& str){
    // If the string is empty, consider it whitespace
    if(str.empty()){
        return true;
    }
    // Check if the string has only whitespace characters
    return str.find_first_not_of(" \t\n\v\f\r") == std::string::npos;
}

void mcsm::replaceAll(std::string& str, const std::string& value, const std::string& replacement){
    std::string::size_type pos = 0;
    // Iterate through the string and replace all occurrences of `value`
    while((pos = str.find(value, pos)) != std::string::npos){
        str.replace(pos, value.length(), replacement);
        pos += replacement.length();
    }
}

std::string mcsm::safeString(const std::string& str){
    std::string value = str;
    mcsm::replaceAll(value, "&", "_");
    mcsm::replaceAll(value, "|", "_");
    mcsm::replaceAll(value, "\\", "_");
    mcsm::replaceAll(value, "..", "__");
    mcsm::replaceAll(value, "/", "_");
    mcsm::replaceAll(value, "\"", "_");
    mcsm::replaceAll(value, "\'", "_");
    return value;
}

bool mcsm::isSafeString(const std::string& str){
    return str.find("&") == std::string::npos && str.find("&") == std::string::npos && str.find("|") == std::string::npos && str.find("\\") == std::string::npos &&
    str.find("..") == std::string::npos && str.find("/") == std::string::npos && str.find("\"") == std::string::npos && str.find("\'") == std::string::npos;
}

bool mcsm::is_number(const std::string& s){
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

std::string mcsm::normalizePath(const std::string& p){
    std::string result = p;
    // Replace multiple slashes with a single slash
    result = std::regex_replace(result, std::regex(R"(\/+)"), "/");
    // Remove trailing slash unless it's the root "/"
    if(result.length() > 1 && result.back() == '/'){
        result.pop_back();
    }
    return result;
}