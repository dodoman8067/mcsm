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

#include <mcsm/server/type/bukkit/paper_server.h>

int mcsm::PaperServer::getVersion(const std::string& ver) const {
    std::string res = mcsm::get("https://api.papermc.io/v2/projects/paper/versions/" + ver);
    nlohmann::json json = nlohmann::json::parse(res, nullptr, false);
    if(json.is_discarded()){
        std::cerr << "[mcsm] Error : Parse of json failed.\n";
        std::cerr << "[mcsm] If you believe that this is an error, please report it to GitHub. (https://github.com/dodoman8067/mcsm)\n\n";
        std::cerr << "[mcsm] Error informations : \n";
        std::cerr << "[mcsm] Called method : mcsm::PaperServer::getVersion() with arguments : " << ver << "\n";
        std::exit(1);
    }
    if(json["builds"].is_array()){
        return json["builds"][json["builds"].size() - 1];
    }else{
        return -1;
    }
}

std::vector<std::string> mcsm::PaperServer::getAvailableVersions(){
    std::vector<std::string> versions;
    for(const std::string& s : mcsm::getMinecraftVersions()){
        versions.push_back(s);
    }
    return versions;
}

std::string mcsm::PaperServer::getJarFile(){
    return "paper.jar";
}

void mcsm::PaperServer::download(const std::string& version){
    download(version, std::filesystem::current_path().string(), "paper.jar");
}

void mcsm::PaperServer::download(const std::string& version, const std::string& path){
    download(version, path, "paper.jar");
}

void mcsm::PaperServer::download(const std::string& version, const std::string& path, const std::string& name){
    int ver = getVersion(version);
    if(ver == -1){
        std::cerr << "Error : Invalid version\n";
        std::cerr << "Called method : mcsm::PaperServer::download() with arguments : " << version << "\n";
        std::exit(1);
    }
    std::string strVer = std::to_string(ver);
    std::string url = "https://api.papermc.io/v2/projects/paper/versions/" + version + "/builds/" + strVer + "/downloads/paper-" + version + "-" + strVer + ".jar";
    mcsm::download(name, url, path);
}

void mcsm::PaperServer::start(mcsm::JvmOption& option){
    if(!std::filesystem::exists("paper.jar")){
        std::cout << "Downloading paper.jar...\n";
    }
    Server::start(option);
}

bool mcsm::PaperServer::hasVersion(const std::string& version){
    return getVersion(version) != -1;
}

std::string mcsm::PaperServer::getTypeAsString() const {
    return "paper";
}