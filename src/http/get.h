#ifndef __MCSM_GET_H__
#define __MCSM_GET_H__

#include <string>
#include <curl/curl.h>

namespace mcsm {
    const std::string get(const std::string& url);
}


#endif // __MCSM_GET_H__