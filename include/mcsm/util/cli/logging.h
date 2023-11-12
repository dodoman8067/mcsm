#ifndef __MCSM_LOGGING_H__
#define __MCSM_LOGGING_H__

#include <string>
#include <iostream>
#include <curl/curl.h>
#ifdef CURLINC_CURL_H
    #include <termcolor/termcolor.hpp>
#endif

namespace mcsm {
    void info(const std::string& message);
    void success(const std::string& message);
    void warning(const std::string& message);
    void error(const std::string& message);
}

#endif // __MCSM_LOGGING_H__