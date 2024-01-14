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

#include <mcsm/server/type/modded/fabric_server.h>

mcsm::FabricServer::FabricServer() {
}

mcsm::FabricServer::~FabricServer() {}

/**
 * Returns fabric loader version.
 */
std::string mcsm::FabricServer::getVersion(const std::string& ver) const {
    std::string res = mcsm::get("https://meta.fabricmc.net/v2/versions/loader/" + ver);
    nlohmann::json json = nlohmann::json::parse(res, nullptr, false);
    if(json.is_discarded()){
        mcsm::error("Parse of json failed.");
        mcsm::error("If you believe that this is a software issue, please report it to GitHub. (https://github.com/dodoman8067/mcsm)");
        mcsm::error("Error informations : ");
        mcsm::error("Called method : mcsm::FabricServer::getVersion() with arguments : " + ver);
        std::exit(1);
    }
    if(json.is_array() && !json.empty()){
        nlohmann::json firstLoader = json[0]["loader"];
        if(firstLoader == nullptr || firstLoader["version"] == nullptr || !firstLoader["version"].is_string()) return "";
        std::string version = firstLoader["version"];
        return version;
    }else{
        return "";
    }
}

/**
 * Returns fabric installer version.
 */
std::string mcsm::FabricServer::getVersion() const {
    std::string res = mcsm::get("https://meta.fabricmc.net/v2/versions/installer");
    nlohmann::json json = nlohmann::json::parse(res, nullptr, false);
    if(json.is_discarded()){
        mcsm::error("Parse of json failed.");
        mcsm::error("If you believe that this is a software issue, please report it to GitHub. (https://github.com/dodoman8067/mcsm)");
        mcsm::error("Error informations : ");
        mcsm::error("Called method : mcsm::FabricServer::getVersion()");
        std::exit(1);
    }
    if(json.is_array() && !json.empty()){
        nlohmann::json first = json[0];
        if(first == nullptr || first["version"] == nullptr || !first["version"].is_string()) return "";
        std::string version = first["version"];
        return version;
    }else{
        return "";
    }
}

std::vector<std::string> mcsm::FabricServer::getAvailableVersions(){
    std::vector<std::string> versions;
    for(const std::string& s : mcsm::getMinecraftVersions()){
        versions.push_back(s);
    }
    return versions;
}

std::string mcsm::FabricServer::getSupportedVersions() const {
    return "1.14~";
}

std::string mcsm::FabricServer::getBasedServer() const {
    return "vanilla";
}

std::string mcsm::FabricServer::getWebSite() const {
    return "https://fabricmc.net";
}

std::string mcsm::FabricServer::getGitHub() const {
    return "https://github.com/FabricMC/fabric";
}

void mcsm::FabricServer::download(const std::string& version){
    download(version, std::filesystem::current_path().string(), getJarFile());
}

void mcsm::FabricServer::download(const std::string& version, const std::string& path){
    download(version, path, getJarFile());
}

void mcsm::FabricServer::download(const std::string& version, const std::string& path, const std::string& name){
    mcsm::Option opt(".", "server");
    std::string loaderVer, installerVer;
    if(opt.hasValue("loader_version") && opt.getValue("loader_version") != "latest"){
        loaderVer = opt.getValue("loader_version").get<std::string>();
        if(mcsm::isWhitespaceOrEmpty(loaderVer)){
            mcsm::error("Missing \"loader_version\" option in server.json");
            mcsm::error("To fix, add \"loader_version\": \"latest\" to server.json for automatic download.");
            std::exit(1);
        }
    }else if(opt.hasValue("loader_version") && opt.getValue("loader_version") == "latest"){
        loaderVer = getVersion(version);
        if(mcsm::isWhitespaceOrEmpty(loaderVer)){
            mcsm::error("Unsupported loader version.");
            mcsm::error("Please try again with a different version.");
            std::exit(1);
        }
    }

    if(opt.hasValue("installer_version") && opt.getValue("installer_version") != "latest"){
        installerVer = opt.getValue("installer_version").get<std::string>();
        if(mcsm::isWhitespaceOrEmpty(installerVer)){
            mcsm::error("Missing \"installer_version\" option in server.json");
            mcsm::error("To fix, add \"installer_version\": \"latest\" to server.json for automatic download.");
            std::exit(1);
        }
    }else if(opt.hasValue("installer_version") && opt.getValue("installer_version") == "latest"){
        installerVer = getVersion();
        if(mcsm::isWhitespaceOrEmpty(installerVer)){
            mcsm::error("Unsupported installer version.");
            mcsm::error("Please try again with a different version.");
            std::exit(1);
        }
    }

    download(version, loaderVer, installerVer, path, name);
}

void mcsm::FabricServer::download(const std::string& version, const std::string& loaderVersion, const std::string& installerVersion, const std::string& path, const std::string& name){
    mcsm::Option opt(".", "server");
    mcsm::FabricServerDataOption sDataOpt;

    if(!opt.hasValue("loader_version")){
        mcsm::error("Missing \"loader_version\" option in server.json");
        mcsm::error("To fix, add \"loader_version\": \"latest\" to server.json for automatic download.");
        std::exit(1);
    }
    if(!opt.getValue("loader_version").is_string()){
        mcsm::error("Value \"loader_version\" option in server.json must be a string type.");
        mcsm::error("To fix, change it into \"loader_version\": \"latest\" .");
        std::exit(1);
    }

    if(!opt.hasValue("installer_version")){
        mcsm::error("Missing \"installer_version\" option in server.json");
        mcsm::error("To fix, add \"installer_version\": \"latest\" to server.json for automatic download.");
        std::exit(1);
    }
    if(!opt.getValue("installer_version").is_string()){
        mcsm::error("Value \"installer_version\" option in server.json must be a string type.");
        mcsm::error("To fix, change it into \"installer_version\": \"latest\" .");
        std::exit(1);
    }

    /*
    if(opt.hasValue("loader_version") && opt.getValue("loader_version") != "latest"){
        loaderVer = opt.getValue("loader_version").get<std::string>();
        if(mcsm::isWhitespaceOrEmpty(loaderVer)){
            mcsm::error("Missing \"loader_version\" option in server.json");
            mcsm::error("To fix, add \"loader_version\": \"latest\" to server.json for automatic download.");
            std::exit(1);
        }
    }else if(opt.hasValue("loader_version") && opt.getValue("loader_version") == "latest"){
        loaderVer = getVersion(version);
        if(mcsm::isWhitespaceOrEmpty(loaderVer)){
            mcsm::error("Unsupported loader version.");
            mcsm::error("Please try again with a different version.");
            std::exit(1);
        }
    }

    if(opt.hasValue("installer_version") && opt.getValue("installer_version") != "latest"){
        installerVer = opt.getValue("installer_version").get<std::string>();
        if(mcsm::isWhitespaceOrEmpty(installerVer)){
            mcsm::error("Missing \"installer_version\" option in server.json");
            mcsm::error("To fix, add \"installer_version\": \"latest\" to server.json for automatic download.");
            std::exit(1);
        }
    }else if(opt.hasValue("installer_version") && opt.getValue("installer_version") == "latest"){
        installerVer = getVersion();
        if(mcsm::isWhitespaceOrEmpty(installerVer)){
            mcsm::error("Unsupported installer version.");
            mcsm::error("Please try again with a different version.");
            std::exit(1);
        }
    }
    */

    std::string url = "https://meta.fabricmc.net/v2/versions/loader/" + version + "/" + loaderVersion + "/" + installerVersion + "/server/jar";
    mcsm::info("URL : " + url);

    if(mcsm::isText(url)){
        std::string res = mcsm::get(url);
    
        if(res == "Unable to find valid version for loader_version" || res == "Fabric loader 0.12 or higher is required for unattended server installs. Please use a newer fabric loader version, or the full installer."){
            mcsm::error("Unsupported loader version : " + loaderVersion);
            mcsm::error("Please try again with a different version.");
            std::exit(1);
        }else if(res == "Unable to find valid version for installer_version" || res == "Fabric loader 0.12 or higher is required for unattended server installs. Please use a newer fabric loader version, or the full installer."){
            mcsm::error("Unsupported installer version : " + installerVersion);
            mcsm::error("Please try again with a different version.");
            std::exit(1);
        }else{
            mcsm::error("Cannot download the file due to the following reason : " + res);
            mcsm::error("Please report it to GitHub (https://github.com/dodoman8067/mcsm).");
            std::exit(1);
        }
    }
    
    mcsm::download(name, url, path, true);
    sDataOpt.updateLoaderVersion(loaderVersion);
    sDataOpt.updateInstallerVersion(installerVersion);
}

void mcsm::FabricServer::start(mcsm::JvmOption& option){
    mcsm::ServerOption sOpt;
    if(!std::filesystem::exists(getJarFile())){
        mcsm::info("Downloading " + getJarFile() + "...");
        download(sOpt.getServerVersion());
    }else{
        update();
    }
    Server::start(option);
}

void mcsm::FabricServer::update(){
    mcsm::FabricServerDataOption sDataOpt;
    mcsm::Option opt(".", "server");

    mcsm::info("Checking updates...");

    if(!opt.hasValue("loader_version")){
        mcsm::error("Missing \"loader_version\" option in server.json");
        mcsm::error("To fix, add \"loader_version\": \"latest\" to server.json for automatic download.");
        std::exit(1);
    }
    if(!opt.getValue("loader_version").is_string()){
        mcsm::error("Value \"loader_version\" option in server.json must be a string type.");
        mcsm::error("To fix, change it into \"loader_version\": \"latest\" .");
        std::exit(1);
    }
    if(opt.getValue("loader_version") != "latest"){
        mcsm::warning("This server won't update to the latest loader version.");
        mcsm::warning("Change server.json into \"loader_version\": \"latest\" for automatic download.");
    }

    if(!opt.hasValue("installer_version")){
        mcsm::error("Missing \"installer_version\" option in server.json");
        mcsm::error("To fix, add \"installer_version\": \"latest\" to server.json for automatic download.");
        std::exit(1);
    }
    if(!opt.getValue("installer_version").is_string()){
        mcsm::error("Value \"installer_version\" option in server.json must be a string type.");
        mcsm::error("To fix, change it into \"installer_version\": \"latest\" .");
        std::exit(1);
    }
    if(opt.getValue("installer_version") != "latest"){
        mcsm::warning("This server won't update to the latest installer version.");
        mcsm::warning("Change server.json into \"installer_version\": \"latest\" for automatic download.");
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

    std::string loaderVer, installerVer;

    loaderVer = opt.getValue("loader_version");
    installerVer = opt.getValue("installer_version");

    if(sDataOpt.getInstallerVersion() == installerVer && sDataOpt.getLoaderVersion() == loaderVer){
        return;
    }

    if(opt.getValue("loader_version") == "latest"){
        std::string mcver = opt.getValue("version");
        loaderVer = getVersion(mcver);
        if(mcsm::isWhitespaceOrEmpty(loaderVer)){
            mcsm::error("Unsupported loader version. (Can't find a loader version for Minecraft " + mcver + ")");
            mcsm::error("Please try again with a different version.");
            std::exit(1);
        }
        if(sDataOpt.getLoaderVersion() != loaderVer){
            mcsm::success("Update found for the loader version : "  + loaderVer + ". Current build : " + sDataOpt.getLoaderVersion());
        }
    }
    if(opt.getValue("installer_version") == "latest"){
        installerVer = getVersion();
        if(mcsm::isWhitespaceOrEmpty(installerVer)){
            mcsm::error("Unsupported installer version.");
            mcsm::error("Please try again with a different version.");
            std::exit(1);
        }
        if(sDataOpt.getInstallerVersion() != installerVer){
            mcsm::success("Update found for the installer version : "  + installerVer + ". Current build : " + sDataOpt.getInstallerVersion());
        }
    }

    if(sDataOpt.getInstallerVersion() == installerVer && sDataOpt.getLoaderVersion() == loaderVer){
        mcsm::success("Server is up to date.");
        return;
    }

    download(opt.getValue("version"), loaderVer, installerVer, std::filesystem::current_path().string(), getJarFile());
}

bool mcsm::FabricServer::hasVersion(const std::string& version){
    return !mcsm::isWhitespaceOrEmpty(getVersion(version));
}

std::string mcsm::FabricServer::getTypeAsString() const {
    return "fabric";
}

mcsm::ServerType mcsm::FabricServer::getType() const {
    return mcsm::ServerType::FABRIC;
}