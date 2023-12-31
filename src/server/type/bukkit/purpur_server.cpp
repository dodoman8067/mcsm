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

int mcsm::PurpurServer::getVersion(const std::string& ver, const std::string& build) const {
    std::string res = mcsm::get("https://api.purpurmc.org/v2/purpur/" + ver + "/" +  build);
    nlohmann::json json = nlohmann::json::parse(res, nullptr, false);
    if(json.is_discarded()){
        mcsm::error("Parse of json failed.");
        mcsm::error("If you believe that this is an error, please report it to GitHub. (https://github.com/dodoman8067/mcsm)");
        mcsm::error("Error informations : ");
        mcsm::error("Called method : mcsm::PurpurServer::getVersion() with arguments : " + ver + ", " + build);
        std::exit(1);
    }

    if(json["build"] == nullptr){
        return -1;
    }else{
        return std::stoi(json["build"].get<std::string>());
    }
}

std::vector<std::string> mcsm::PurpurServer::getAvailableVersions(){
    std::vector<std::string> versions;
    for(const std::string& s : mcsm::getMinecraftVersions()){
        versions.push_back(s);
    }
    return versions;
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
    mcsm::Option opt(".", "server");
    mcsm::ServerDataOption sDataOpt;
    if(opt.hasValue("server_build") && opt.getValue("server_build") != "latest"){
        if(!opt.getValue("server_build").is_string()){
            mcsm::error("Value \"server_build\" option in server.json must be a string type.");
            mcsm::error("To fix, change it into \"server_build\": \"latest\" .");
            std::exit(1);            
        }
        std::string build = opt.getValue("server_build").get<std::string>();
        if(mcsm::isWhitespaceOrEmpty(build)){
            mcsm::error("Missing \"server_build\" option in server.json");
            mcsm::error("To fix, add \"server_build\": \"latest\" to server.json for automatic download.");
            std::exit(1);
        }
        int ver = getVersion(version, build);
        if(ver == -1){
            mcsm::error("Unsupported version : " + build);
            mcsm::error("Please try again with a different version.");
            std::exit(1);
        }
        std::string strVer = std::to_string(ver);
        std::string url = "https://api.purpurmc.org/v2/purpur/" + version + "/" + strVer + "/download/";
        mcsm::info("URL : " + url);
        mcsm::download(name, url, path, true);
        sDataOpt.updateLastDownloadedBuild(strVer);
    }else{
        if(!opt.hasValue("server_build")){
            mcsm::error("Missing \"server_build\" option in server.json");
            mcsm::error("To fix, add \"server_build\": \"latest\" to server.json for automatic download.");
            std::exit(1);
        }
        int ver = getVersion(version);
        if(ver == -1){
            mcsm::error("Unsupported version.");
            mcsm::error("Please try again with a different version.");
            std::exit(1);
        }
        std::string strVer = std::to_string(ver);
        std::string url = "https://api.purpurmc.org/v2/purpur/" + version + "/" + strVer + "/download/";
        mcsm::info("URL : " + url);
        mcsm::download(name, url, path, true);
        sDataOpt.updateLastDownloadedBuild(strVer);
    }
}

void mcsm::PurpurServer::start(mcsm::JvmOption& option){
    mcsm::ServerOption sOpt;
    if(!std::filesystem::exists(getJarFile())){
        mcsm::info("Downloading " + getJarFile() + "...");
        download(sOpt.getServerVersion());
    }else{
        update();
    }
    Server::start(option);
}

void mcsm::PurpurServer::update(){
    // If you change the default build to specific build from latest build, it won't downgrade automatically. (You'll have to manually delete the server jarfile) This is an intented feature.
    mcsm::info("Checking updates...");
    mcsm::ServerDataOption sDataOpt;
    mcsm::Option opt(".", "server");
    if(!opt.getValue("server_build").is_string()){
        mcsm::error("Value \"server_build\" option in server.json must be a string type.");
        mcsm::error("To fix, change it into \"server_build\": \"latest\" .");
        std::exit(1);            
    }
    std::string build = opt.getValue("server_build").get<std::string>();
    if(build != "latest"){
        mcsm::warning("This server won't update to the latest build.");
        mcsm::warning("Change server.json into \"server_build\": \"latest\" for automatic download.");
        return;
    }
    if(opt.getValue("version") == nullptr){
        mcsm::error("No \"version\" value specified in file " + opt.getName());
        mcsm::error("Manually editing the launch profile might have caused this issue.");
        mcsm::error("If you know what you're doing, I believe you that you know how to handle this issue.");
        mcsm::error("If you believe that this is a software issue, please report it to GitHub (https://github.com/dodoman8067/mcsm).");
        std::exit(1);
    }
    if(!opt.getValue("version").is_string()){
        mcsm::error("Value \"version\" has to be a string, but it's not.");
        mcsm::error("Manually editing the launch profile might have caused this issue.");
        mcsm::error("If you know what you're doing, I believe you that you know how to handle this issue.");
        mcsm::error("If you believe that this is a software issue, please report it to GitHub (https://github.com/dodoman8067/mcsm).");
        std::exit(1);            
    }
    std::string version = opt.getValue("version").get<std::string>();
    int ver = getVersion(version);
    if(ver == -1){
        mcsm::error("Unsupported version.");
        mcsm::error("Please try again with a different version.");
        std::exit(1);
    }
    std::string strVer = std::to_string(ver);
    if(sDataOpt.getLastDownloadedBuild() == strVer){
        mcsm::success("Server is up to date.");
        return;
    }
    mcsm::success("Update found : "  + strVer + ". Current build : " + sDataOpt.getLastDownloadedBuild());
    if(std::filesystem::exists(getJarFile())){
        std::filesystem::remove(getJarFile());
    }
    download(version);
}

bool mcsm::PurpurServer::hasVersion(const std::string& version){
    return getVersion(version) != -1;
}

std::string mcsm::PurpurServer::getTypeAsString() const {
    return "purpur";
}