#ifndef __MCSM_HOLDER_H__
#define __MCSM_HOLDER_H__

#include <curl/curl.h>
#include <mcsm/util/cli/result.h>

namespace mcsm {
    struct curl_holder {
        static CURL* curl;
        static std::mutex curl_mutex;
        
        static mcsm::Result init();
        static void cleanup();
    };
}
#endif // __MCSM_HOLDER_H__