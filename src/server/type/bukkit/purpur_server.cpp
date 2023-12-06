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

#include <mcsm/server/type/bukkit/purpur_server.h>

mcsm::PurpurServer::PurpurServer() {}

mcsm::PurpurServer::~PurpurServer() {}

int mcsm::PurpurServer::getVersion(const std::string& ver) const {
    std::string res = mcsm::get("https://api.purpurmc.org/v2/purpur/" + ver);
    nlohmann::json json = nlohmann::json::parse(res, nullptr, false);
    if(json.is_discarded()){
        mcsm::error("Parse of json failed.");
        mcsm::error("If you believe that this is an error, please report it to GitHub. (https://github.com/dodoman8067/mcsm)");
        mcsm::error("Error informations : ");
        mcsm::error("Called method : mcsm::PurpurServer::getVersion() with arguments : " + ver);
        std::exit(1);
    }
    if(json["builds"] != nullptr){
        nlohmann::json builds = json["builds"];
        if(builds["latest"] == nullptr) return -1;
        return std::stoi(builds["latest"].get<std::string>());
    }else{
        return -1;
    }
}

std::vector<std::string> mcsm::PurpurServer::getAvailableVersions(){
    std::vector<std::string> versions;
    for(const std::string& s : mcsm::getMinecraftVersions()){
        versions.push_back(s);
    }
    return versions;
}

std::string mcsm::PurpurServer::getJarFile() const {
    mcsm::Option opt(".", "server");
    if(opt.exists() && opt.getValue("server_jar") != nullptr){
        return opt.getValue("server_jar");
    }
    return getTypeAsString() + ".jar";
}

std::string mcsm::PurpurServer::getSupportedVersions() const {
    return "1.14.1~";
}

std::string mcsm::PurpurServer::getBasedServer() const {
    return "pufferfish";
}

std::string mcsm::PurpurServer::getWebSite() const {
    return "https://purpurmc.org";
}

std::string mcsm::PurpurServer::getGitHub() const {
    return "https://github.com/PurpurMC/Purpur";
}

void mcsm::PurpurServer::download(const std::string& version){
    download(version, std::filesystem::current_path().string(), getJarFile());
}

void mcsm::PurpurServer::download(const std::string& version, const std::string& path){
    download(version, path, getJarFile());
}

void mcsm::PurpurServer::download(const std::string& version, const std::string& path, const std::string& name){
    int ver = getVersion(version);
    if(ver == -1){
        mcsm::error("Unsupported version.");
        mcsm::error("Please try again with a different version.");
        std::exit(1);
    }
    std::string strVer = std::to_string(ver);
    std::string url = "https://api.purpurmc.org/v2/purpur/" + version + "/" + strVer + "/download/";
    mcsm::download(name, url, path);
}

void mcsm::PurpurServer::start(mcsm::JvmOption& option){
    mcsm::ServerOption sOpt;
    if(!std::filesystem::exists(getJarFile())){
        mcsm::info("Downloading " + getJarFile() + "...");
        download(sOpt.getServerVersion());
    }
    Server::start(option);
}

bool mcsm::PurpurServer::hasVersion(const std::string& version){
    return getVersion(version) != -1;
}

std::string mcsm::PurpurServer::getTypeAsString() const {
    return "purpur";
}