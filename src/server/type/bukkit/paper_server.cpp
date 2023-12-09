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

mcsm::PaperServer::PaperServer() {}

mcsm::PaperServer::~PaperServer() {}

int mcsm::PaperServer::getVersion(const std::string& ver) const {
    std::string res = mcsm::get("https://api.papermc.io/v2/projects/paper/versions/" + ver);
    nlohmann::json json = nlohmann::json::parse(res, nullptr, false);
    if(json.is_discarded()){
        mcsm::error("Parse of json failed.");
        mcsm::error("If you believe that this is an error, please report it to GitHub. (https://github.com/dodoman8067/mcsm)");
        mcsm::error("Error informations : ");
        mcsm::error("Called method : mcsm::PaperServer::getVersion() with arguments : " + ver);
        std::exit(1);
    }
    if(json["builds"].is_array()){
        return json["builds"][json["builds"].size() - 1];
    }else{
        return -1;
    }
}

int mcsm::PaperServer::getVersion(const std::string& ver, const std::string& build) const {
    std::string res = mcsm::get("https://api.papermc.io/v2/projects/paper/versions/" + ver + "/builds/" + build);
    nlohmann::json json = nlohmann::json::parse(res, nullptr, false);
    if(json.is_discarded()){
        mcsm::error("Parse of json failed.");
        mcsm::error("If you believe that this is an error, please report it to GitHub. (https://github.com/dodoman8067/mcsm)");
        mcsm::error("Error informations : ");
        mcsm::error("Called method : mcsm::PaperServer::getVersion() with arguments : " + ver + ", " + build);
        std::exit(1);
    }

    if(json["build"] == nullptr){
        return -1;
    }else{
        return json["build"];
    }
}

std::vector<std::string> mcsm::PaperServer::getAvailableVersions(){
    std::vector<std::string> versions;
    for(const std::string& s : mcsm::getMinecraftVersions()){
        versions.push_back(s);
    }
    return versions;
}

std::string mcsm::PaperServer::getJarFile() const {
    mcsm::Option opt(".", "server");
    if(opt.exists() && opt.getValue("server_jar") != nullptr){
        return opt.getValue("server_jar");
    }
    return getTypeAsString() + ".jar";
}

std::string mcsm::PaperServer::getSupportedVersions() const {
    return "1.8~";
}

std::string mcsm::PaperServer::getBasedServer() const {
    return "spigot";
}

std::string mcsm::PaperServer::getWebSite() const {
    return "https://papermc.io";
}

std::string mcsm::PaperServer::getGitHub() const {
    return "https://github.com/PaperMC/Paper";
}

void mcsm::PaperServer::download(const std::string& version){
    download(version, std::filesystem::current_path().string(), getJarFile());
}

void mcsm::PaperServer::download(const std::string& version, const std::string& path){
    download(version, path, getJarFile());
}

void mcsm::PaperServer::download(const std::string& version, const std::string& path, const std::string& name){
    mcsm::Option opt(".", "server");
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
        std::string url = "https://api.papermc.io/v2/projects/paper/versions/" + version + "/builds/" + strVer + "/downloads/paper-" + version + "-" + strVer + ".jar";
        mcsm::download(name, url, path);
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
        std::string url = "https://api.papermc.io/v2/projects/paper/versions/" + version + "/builds/" + strVer + "/downloads/paper-" + version + "-" + strVer + ".jar";
        mcsm::download(name, url, path);
    }
}

void mcsm::PaperServer::start(mcsm::JvmOption& option){
    mcsm::ServerOption sOpt;
    if(!std::filesystem::exists(getJarFile())){
        mcsm::info("Downloading " + getJarFile() + "...");
        download(sOpt.getServerVersion());
    }
    Server::start(option);
}

bool mcsm::PaperServer::hasVersion(const std::string& version){
    return getVersion(version) != -1;
}

std::string mcsm::PaperServer::getTypeAsString() const {
    return "paper";
}