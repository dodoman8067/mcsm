#ifndef __MCSM_CURL_MANAGER_H__
#define __MCSM_CURL_MANAGER_H__

#include <curl/curl.h>

namespace mcsm {
    bool initCurl();
    CURL* getCurl();
    void clearCurl();
}


#endif // __MCSM_CURL_MANAGER_H__