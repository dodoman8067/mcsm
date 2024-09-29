#include <mcsm/http/holder.h>

CURL* mcsm::curl_holder::curl = nullptr;

mcsm::Result mcsm::curl_holder::init(){
    if(!curl){
        curl = curl_easy_init();
        if(!curl){
            return {mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::curlInitFailed()};
        }
    }
    return {mcsm::ResultType::MCSM_SUCCESS, {"Success"}};
}

void mcsm::curl_holder::cleanup(){
    if(curl){
        curl_easy_cleanup(curl);
        curl = nullptr;
    }
}