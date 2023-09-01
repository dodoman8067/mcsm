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