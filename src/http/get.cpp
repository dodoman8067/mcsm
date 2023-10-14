#include "get.h"

static size_t writeFunction(void* ptr, size_t size, size_t nmemb, std::string* data) {
    data->append((char*)ptr, size * nmemb);
    return size * nmemb;
}

const std::string mcsm::get(const std::string& url){
    CURL *curl = curl_easy_init();
    CURLcode result;

    if(!curl){
        std::cerr << "Error: Unable to initialize curl\n";
        std::cerr << "Please try re-running the program, reboot the PC or reinstall the program.\n";
        std::cerr << "If none of it isn't working for you, please open a new issue in GitHub (https://github.com/dodoman8067/mcsm).\n";
        std::exit(1);
    }

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
        curl_easy_setopt(curl, CURLOPT_USERPWD, "user:pass");
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "curl/8.4.0");
        curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 50L);
        curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);
        
        std::string response_string;
        std::string header_string;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header_string);

    result = curl_easy_perform(curl);

    if(result != CURLE_OK){
        std::cerr << "Error: Failed to perform GET request with the following error : " << curl_easy_strerror(result) << "\n";
    }

    curl_easy_cleanup(curl);

    return response_string;
}