#include "curl_manager.h"

static CURL* curl;

CURL* mcsm::getCurl(){
    return curl;
}

bool mcsm::initCurl(){
    curl_global_init(CURL_VERSION_THREADSAFE);
    curl =  curl_easy_init();
    return curl != nullptr;
}

void mcsm::clearCurl(){
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    curl = nullptr;
}