#include <mcsm/http/holder.h>

CURL* mcsm::curl_holder::curl = nullptr;
std::mutex mcsm::curl_holder::curl_mutex;

mcsm::VoidResult mcsm::curl_holder::init(){
    std::lock_guard<std::mutex> lock(curl_mutex);
    if(!curl){
        curl = curl_easy_init();
        if(!curl){
            return {mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::curlInitFailed()};
        }
    }
    return {mcsm::ResultType::MCSM_SUCCESS, {"Success"}};
}

void mcsm::curl_holder::cleanup(){
    std::lock_guard<std::mutex> lock(curl_mutex);
    if(curl){
        curl_easy_cleanup(curl);
        curl = nullptr;
    }
}