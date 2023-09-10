#include "string_utils.h"

bool mcsm::startsWith(const std::string& str, const std::string& value){
    return str.rfind(value, 0) == 0;
}

bool mcsm::endsWith(const std::string& str, const std::string& value){
    if (str.length() < value.length()) {
        return false;
    }
    return str.compare(str.length() - value.length(), value.length(), value) == 0;
}

bool mcsm::isWhitespaceOrEmpty(const std::string& str){
    return str.find_first_not_of(" \t\n\v\f\r") == std::string::npos;
}