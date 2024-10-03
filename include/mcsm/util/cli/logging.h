/*
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

#ifndef __MCSM_LOGGING_H__
#define __MCSM_LOGGING_H__

#include <mcsm/util/string_utils.h>
/*
 I know this file does not use libcurl functions.
 But the files that include this header does, and that causes windows header issue(winsock2.h included before windows.h)
*/
#include <curl/curl.h>
#ifdef CURLINC_CURL_H
    #include <termcolor/termcolor.hpp>
#endif

namespace mcsm {
    extern bool log; // means defined elsewhere

    inline void set_log_enabled(const bool& value){
        log = value;
    }

    /**
     * Prints normal information message to the console.
     * @param message string to print
     */
    inline void info(const std::string& message){
        if(!log) return;
        std::string msg = message;
        if(!mcsm::endsWith(msg, "\n")) msg += "\n";
        std::cout << "[mcsm/INFO] " << msg;
    }

    /**
     * Prints successful (green) message to the console.
     * @param message string to print
     */
    inline void success(const std::string& message){
        if(!log) return;
        std::string msg = message;
        if(!mcsm::endsWith(msg, "\n")) msg += "\n";
        std::cout << termcolor::bright_green << "[mcsm/INFO] " << msg << termcolor::reset;
    }

    /**
     * Prints warning (yellow) message to the console.
     * @param message string to print
    */
    inline void warning(const std::string& message){
        if(!log) return;
        std::string msg = message;
        if(!mcsm::endsWith(msg, "\n")) msg += "\n";
        std::cerr << termcolor::bright_yellow << "[mcsm/WARN] " << msg << termcolor::reset;
    }

    /**
     * Prints error (red) message to the console.
     * @param message string to print
    */
    inline void error(const std::string& message){
        if(!log) return;
        std::string msg = message;
        if(!mcsm::endsWith(msg, "\n")) msg += "\n";
        std::cerr << termcolor::bright_red << "[mcsm/ERROR] " << msg << termcolor::reset;
    }
}

#endif // __MCSM_LOGGING_H__