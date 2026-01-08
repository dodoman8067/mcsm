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
#include <filesystem>

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
    if(s.empty()) return false;

    std::string::const_iterator it = s.begin();

    if(*it == '+' || *it == '-'){
        ++it;
        if (it == s.end()) return false;
    }

    while (it != s.end() && std::isdigit(static_cast<unsigned char>(*it))){
        ++it;
    }

    return it == s.end();
}

std::string mcsm::normalizePath(const std::string& p){
    if(p.empty()) return p; // Handle empty input

    // Use std::filesystem for better handling of ".." and "."
    std::filesystem::path pathObj = std::filesystem::absolute(p);
    std::string result = pathObj.lexically_normal().string();

    // Ensure it does not end with a trailing slash unless it's root "/"
    if(result.length() > 1 && result.back() == '/'){
        result.pop_back();
    }
    return result;
}

std::string mcsm::formatPrompt(const std::string &key){
    std::string formatted = key;
    size_t pos = 0;
    while ((pos = formatted.find('_', pos)) != std::string::npos){
        formatted.replace(pos, 1, " ");
        pos++;
    }
    formatted[0] = toupper(formatted[0]); // Capitalize
    if(key == "custom_run_command"){
        formatted = formatted + ". Overrides server JVM profile based start system.";
    }
    if(key == "default_jvm_launch_profile_search_path"){
        formatted = formatted + " (current/global)";
    }
    if(key == "server_file_location"){
        formatted = formatted + " (url/filepath)";
    }
    if(key == "auto_server_jar_update"){
        formatted = "if server should update the server jarfile automatically";
    }
    if(key == "sponge_api_search_recommended_versions"){
        formatted = "if the server search on recommended versions api";
    }
    if(key == "server_jarfile") {
        formatted = "server jarfile path. Must include filename at the end not including \"/\".";
    }
    return formatted;
}

std::string mcsm::vecToString(const std::vector<std::string>& vec) {
    std::string s;
    for(const auto& v : vec){
        s += v + " ";
    }
    return s;
}

std::vector<std::string> mcsm::stringToVec(const std::string& str){
    std::vector<std::string> vec;
    std::istringstream iss(str);
    std::string part;
    while(std::getline(iss, part, ' ')){
        if(!part.empty()){
            vec.push_back(part);
        }
    }
    return vec;
}