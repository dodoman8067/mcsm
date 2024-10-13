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


#include <mcsm/http/download.h>

static size_t writeFunction(mcsm::Result *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written;
    written = fwrite(ptr, size, nmemb, stream);
    return written;
}

static int progressCallback(void * /* clientp */, curl_off_t dltotal, curl_off_t dlnow, curl_off_t /* ultotal */, curl_off_t /* ulnow */) {
    const int barWidth = 20;

    if(dltotal > 0){
        double percentage = static_cast<double>(dlnow) / static_cast<double>(dltotal) * 100.0;
        int progress = static_cast<int>(barWidth * percentage / 100.0);

        std::cout << "\r[";
        for(int i = 0; i < barWidth; i++){
            if(i < progress) std::cout << "=";
            else std::cout << " ";
        }

        std::cout << "] ";
        std::cout << std::fixed << std::setprecision(1) << percentage << "%";
        std::cout.flush();
    }
    return 0;
}

static int xferinfoCallback(void *clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow) {
    return progressCallback(clientp, dltotal, dlnow, ultotal, ulnow);
}

mcsm::Result mcsm::download(const std::string& name, const std::string& url){
    const std::string& path = mcsm::getCurrentPath();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    return download(name, url, path);
}

mcsm::Result mcsm::download(const std::string& name, const std::string& url, const std::string& path){
    return download(name, url, path, false);
}

mcsm::Result mcsm::download(const std::string& name, const std::string& url, const std::string& path, const bool& percentages){
    CURL* curl = mcsm::curl_holder::curl;
    CURLcode res;
    std::FILE* file;
    const std::string& filename = path + "/" + name;
    file = std::fopen(filename.c_str(), "wb");

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl/8.10.0");
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
    curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);
    curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2TLS);
    curl_easy_setopt(curl, CURLOPT_DNS_CACHE_TIMEOUT, 60L);
    
    if(percentages){
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
        curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, xferinfoCallback);
    }

    res = curl_easy_perform(curl);

    if(percentages) std::cout << "\n";

    if(res != CURLE_OK){
        mcsm::Result result({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::downloadRequestFailed(url, curl_easy_strerror(res))});
        std::fclose(file);
        curl_easy_reset(curl);
        return result;
    }

    std::fclose(file);
    curl_easy_reset(curl);
    mcsm::Result result({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
    return result;
}

bool mcsm::isText(const std::string& url){
    CURL* curl = mcsm::curl_holder::curl;
    char *contentType = nullptr;
    bool isText = false;
    CURLcode res;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
    curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);
    curl_easy_setopt(curl, CURLOPT_DNS_CACHE_TIMEOUT, 60L);

    res = curl_easy_perform(curl);

    if(res != CURLE_OK){
        mcsm::Result result({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::getRequestFailed(url, curl_easy_strerror(res))});
        curl_easy_reset(curl);
        return false;
    }

    res = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &contentType);
    if(res == CURLE_OK && contentType){
        const std::string& contentTypeStr = contentType;
        isText = contentTypeStr.find("text") == 0 || contentTypeStr.find("json") != std::string::npos;
    }else{
        mcsm::Result result({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::getRequestFailed(url, curl_easy_strerror(res))});
        curl_easy_reset(curl);
        return false;
    }

    curl_easy_reset(curl);
    mcsm::Result result({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
    return isText;
}