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

static size_t writeFunction(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written;
    written = fwrite(ptr, size, nmemb, stream);
    return written;
}

void mcsm::download(const std::string& name, const std::string& url){
    download(name, url, std::filesystem::current_path().string());
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
    std::string filename = path + "/" + name;
    file = std::fopen(filename.c_str(), "wb");

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