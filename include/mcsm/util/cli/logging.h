#ifndef __MCSM_LOGGING_H__
#define __MCSM_LOGGING_H__

#include <termcolor/termcolor.hpp>
#include <string>

namespace mcsm {
    void info(const std::string& message);
    void success(const std::string& message);
    void warning(const std::string& message);
    void error(const std::string& message);
}

#endif // __MCSM_LOGGING_H__