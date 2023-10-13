#include "get.h"

const std::string mcsm::get(const std::string& url){
    CURL *curl = curl_easy_init();
    curl_easy_cleanup(curl);
    curl = nullptr;
}