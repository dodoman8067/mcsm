#ifndef __MCSM_LOGGING_H__
#define __MCSM_LOGGING_H__

#include <string>
#include <iostream>
/*
 I know this file does not use libcurl functions.
 But the files that include this header does, and that causes windows header issue(winsock2.h included before windows.h)
*/
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