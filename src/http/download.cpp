#include "download.h"

static size_t writeFunction(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written;
    written = fwrite(ptr, size, nmemb, stream);
    return written;
}

void mcsm::download(const std::string& name, const std::string& url){
    download(name, url, std::filesystem::current_path().string() + "/" +  name);
}

void mcsm::download(const std::string& name, const std::string& url, const std::string& path){
    CURL* curl = curl_easy_init();
    if(!curl){
        std::cerr << "Error: Unable to initialize curl\n";
        std::cerr << "Please try re-running the program, reboot the PC or reinstall the program.\n";
        std::cerr << "If none of it isn't working for you, please open a new issue in GitHub (https://github.com/dodoman8067/mcsm).\n";
        curl_easy_cleanup(curl);
        std::exit(1);
    }
    CURLcode res;
    std::FILE* file;

    file = std::fopen(path.c_str(), "wb");

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
    
    res = curl_easy_perform(curl);

    if(res != CURLE_OK){
        std::cerr << "Error : Failed to download a file from specified url : " << url << " with the following reason : " << curl_easy_strerror(res) << "\n";
        std::fclose(file);
        curl_easy_cleanup(curl);
        std::exit(1);
    }

    std::fclose(file);
    curl_easy_cleanup(curl);
}