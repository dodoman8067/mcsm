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

#include "paper_server.h"

int mcsm::PaperServer::getVersion(const std::string& ver) const {
    std::string res = mcsm::get("https://api.papermc.io/v2/projects/paper/versions/" + ver);
    nlohmann::json json = nlohmann::json::parse(res, nullptr, false);
    if(json.is_discarded()){
        std::cerr << "Error : Parse of json failed.\n";
        std::cerr << "If you believe that this is an error, please report it to GitHub. (https://github.com/dodoman8067/mcsm)\n\n";
        std::cerr << "Error informations : \n";
        std::cerr << "Called method : mcsm::PaperServer::getVersion() with arguments : " << ver << "\n";
        std::exit(1);
    }
    if(json["builds"].is_array()){
        return json["builds"][json["builds"].size() - 1];
    }else{
        return -1;
    }
}

std::unique_ptr<std::vector<std::string>> mcsm::PaperServer::getAvailableVersions() {
    
}

void mcsm::PaperServer::download(const std::string& version){
    
}

void mcsm::PaperServer::download(const std::string& version, const std::string& path){

}

void mcsm::PaperServer::download(const std::string& version, const std::string& path, const std::string& name){

}

bool mcsm::PaperServer::hasVersion(const std::string& version){
    
}