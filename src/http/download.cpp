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
#include <mcsm/data/options/general_option.h>

static size_t writeFunction(char* ptr, size_t size, size_t nmemb, void* userdata) {
    std::FILE* stream = static_cast<std::FILE*>(userdata);
    return std::fwrite(ptr, size, nmemb, stream);
}

static size_t header_log(char* buf, size_t sz, size_t nm, void* /*ud*/) {
    std::string_view h(buf, sz*nm);
    std::cerr << "[H] " << h;   // or buffer & parse
    return sz*nm;
}

static int progressCallback(void * /* clientp */, curl_off_t dltotal, curl_off_t dlnow, curl_off_t /* ultotal */, curl_off_t /* ulnow */) {
    const int barWidth = 20;

    if(dltotal > 0){
        double percentage = static_cast<double>(dlnow) / static_cast<double>(dltotal) * 100.0;
        int progress = static_cast<int>(barWidth * percentage / 100.0);

        if(mcsm::GeneralOption::getGeneralOption().colorDownloadProgressBar()){
            if(progress < 4)
                mcsm::setcol(mcsm::NamedColor::DARK_RED);
            else if(progress < 10)
                mcsm::setcol(mcsm::NamedColor::RED);
            else if(progress < 16)
                mcsm::setcol(mcsm::NamedColor::YELLOW);
            else if(progress < 20)
                mcsm::setcol(mcsm::NamedColor::GREEN);
            else
                mcsm::setcol(mcsm::NamedColor::BLUE);
        }

        std::cout << "\r[";
        for(int i = 0; i < barWidth; i++){
            if(i < progress) std::cout << "=";
            else std::cout << " ";
        }

        std::cout << "] ";
        std::cout << std::fixed << std::setprecision(1) << percentage << "%";
        std::cout.flush();

        mcsm::resetcol();
    }else{
        std::cout << "\r" << (dlnow / 1024) << " KiB downloaded...";
        std::cout.flush();
    }
    return 0;
}

static int xferinfoCallback(void *clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow) {
    return progressCallback(clientp, dltotal, dlnow, ultotal, ulnow);
}

mcsm::VoidResult mcsm::download(const std::string& name, const std::string& url){
    auto path = mcsm::getCurrentPath();
    if(!path) return tl::unexpected(path.error());

    return download(name, url, path.value());
}

mcsm::VoidResult mcsm::download(const std::string& name, const std::string& url, const std::string& path){
    return download(name, url, path, false);
}

mcsm::VoidResult mcsm::download(const std::string& name, const std::string& url, const std::string& path, const bool& percentages){
    CURL* curl = mcsm::curl_holder::curl;
    CURLcode res;
    std::FILE* file;
    const std::string& filename = path + "/" + name;
    file = std::fopen(filename.c_str(), "wb");

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, curl_version());
    curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);
    curl_easy_setopt(curl, CURLOPT_DNS_CACHE_TIMEOUT, 60L);
    /*
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_log);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); // optional for redirects/effective URL
    */
    
    if(percentages){
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
        curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, xferinfoCallback);
    }

    res = curl_easy_perform(curl);

    if(percentages) std::cout << "\n";

    if(res != CURLE_OK){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::DOWNLOAD_REQUEST_FAILED, {url, curl_easy_strerror(res)});
        std::fclose(file);
        curl_easy_reset(curl);
        return tl::unexpected(err);
    }

    std::fclose(file);
    curl_easy_reset(curl);
    return {};
}

mcsm::BoolResult mcsm::isText(const std::string& url){
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
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::GET_REQUEST_FAILED, {url, curl_easy_strerror(res)});
        curl_easy_reset(curl);
        return tl::unexpected(err);
    }

    res = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &contentType);
    if(res == CURLE_OK && contentType){
        const std::string& contentTypeStr = contentType;
        isText = contentTypeStr.find("text") == 0 || contentTypeStr.find("json") != std::string::npos;
    }else{
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::GET_REQUEST_FAILED, {url, curl_easy_strerror(res)});
        curl_easy_reset(curl);
        return tl::unexpected(err);
    }

    curl_easy_reset(curl);
    return isText;
}