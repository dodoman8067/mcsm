#ifndef __MCSM_STRING_UTILS_H__
#define __MCSM_STRING_UTILS_H__

#include <string>

namespace mcsm {
    bool startsWith(const std::string& str, const std::string& value);
    bool endsWith(const std::string& str, const std::string& value);
    bool isWhitespaceOrEmpty(const std::string& str);
}

#endif