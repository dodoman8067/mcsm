#include "curl_manager.h"

void mcsm::initCurl(){
    curl_global_init(CURL_VERSION_THREADSAFE);
}

void mcsm::clearCurl(){
    curl_global_cleanup();
}