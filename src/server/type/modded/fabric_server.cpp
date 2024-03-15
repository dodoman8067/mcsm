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
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";
    nlohmann::json json = nlohmann::json::parse(res, nullptr, false);
    if(json.is_discarded()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonParseFailedCannotBeModified()});
        return "";
    }
    if(json.is_array() && !json.empty()){
        nlohmann::json firstLoader = json[0]["loader"];
        if(firstLoader == nullptr || firstLoader["version"] == nullptr || !firstLoader["version"].is_string()) return "";
        std::string version = firstLoader["version"];
        mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
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
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";
    nlohmann::json json = nlohmann::json::parse(res, nullptr, false);
    if(json.is_discarded()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonParseFailedCannotBeModified()});
        return "";
    }
    if(json.is_array() && !json.empty()){
        nlohmann::json first = json[0];
        if(first == nullptr || first["version"] == nullptr || !first["version"].is_string()) return "";
        std::string version = first["version"];
        mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
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
    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
    return versions;
}

std::string mcsm::FabricServer::getSupportedVersions() const {
    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
    return "1.14~";
}

std::string mcsm::FabricServer::getBasedServer() const {
    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
    return "vanilla";
}

std::string mcsm::FabricServer::getWebSite() const {
    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
    return "https://fabricmc.net";
}

std::string mcsm::FabricServer::getGitHub() const {
    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
    return "https://github.com/FabricMC/fabric";
}

mcsm::Result mcsm::FabricServer::download(const std::string& version){
    std::string path = mcsm::getCurrentPath();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }
    
    std::string jar = getJarFile();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    return download(version, path, jar, path);
}

mcsm::Result mcsm::FabricServer::download(const std::string& version, const std::string& path){
    std::string path1 = mcsm::getCurrentPath();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    std::string jar = getJarFile();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    return download(version, path, jar, path1);
}

mcsm::Result mcsm::FabricServer::download(const std::string& version, const std::string& path, const std::string& name){
    std::string optionPath = mcsm::getCurrentPath();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    mcsm::Option opt(optionPath, "server");
    bool exists = opt.exists();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    if(!exists){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverNotConfigured()});
        return res;
    }

    std::string loaderVer, installerVer;

    nlohmann::json sType = opt.getValue("type");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }
    if(sType == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFoundPlusFix("\"type\"", opt.getName(), "add \"type\": \"[yourtype]\"")});
        return res;
    }
    if(!sType.is_string()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongTypePlusFix("\"type\"", opt.getName(), "string", "change it into \"type\": \"[yourtype]\"")});
        return res;
    }
    if(sType != "fabric"){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverWrongInstanceGenerated("Fabric")});
        return res;
    }

    nlohmann::json sLoaderVer = opt.getValue("loader_version");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    if(sLoaderVer == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFoundPlusFix("\"loader_version\"", opt.getName(), "add \"loader_version\": \"latest\" to server.json for automatic download")});
        return res;
    }
    if(!sLoaderVer.is_string()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongTypePlusFix("\"loader_version\"", opt.getName(), "string", "add \"loader_version\": \"latest\" to server.json for automatic download.")});
        return res;
    }
    if(sLoaderVer != "latest"){
        loaderVer = sLoaderVer.get<std::string>();
        if(mcsm::isWhitespaceOrEmpty(loaderVer)){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFoundPlusFix("\"loader_version\"", opt.getName(), "add \"loader_version\": \"latest\" to server.json for automatic download")});
            return res;
        }
    }else if(sLoaderVer == "latest"){
        loaderVer = getVersion(version);
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
            std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
            mcsm::Result res(resp.first, resp.second);
            return res;
        }
        if(mcsm::isWhitespaceOrEmpty(loaderVer)){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
                "Unsupported loader version.",
                "Please try again with a different version."
            }});
            return res;
        }
    }


    nlohmann::json sInstallerVer = opt.getValue("installer_version");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    if(sInstallerVer == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFoundPlusFix("\"installer_version\"", opt.getName(), "add \"installer_version\": \"latest\" to server.json for automatic download")});
        return res;
    }
    if(!sInstallerVer.is_string()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongTypePlusFix("\"installer_version\"", opt.getName(), "string", "add \"installer_version\": \"latest\" to server.json for automatic download.")});
        return res;
    }
    if(sInstallerVer != "latest"){
        installerVer = sInstallerVer.get<std::string>();
        if(mcsm::isWhitespaceOrEmpty(installerVer)){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFoundPlusFix("\"installer_version\"", opt.getName(), "add \"installer_version\": \"latest\" to server.json for automatic download")});
            return res;
        }
    }else if(sInstallerVer == "latest"){
        installerVer = getVersion();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
            std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
            mcsm::Result res(resp.first, resp.second);
            return res;
        }
        if(mcsm::isWhitespaceOrEmpty(installerVer)){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
                "Unsupported installer version.",
                "Please try again with a different version."
            }});
            return res;
        }
    }

    return download(version, loaderVer, installerVer, path, name, optionPath);
}

mcsm::Result mcsm::FabricServer::download(const std::string& version, const std::string& path, const std::string& name, const std::string& optionPath){
    mcsm::Option opt(optionPath, "server");
    bool exists = opt.exists();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    if(!exists){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverNotConfigured()});
        return res;
    }

    std::string loaderVer, installerVer;

    nlohmann::json sType = opt.getValue("type");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }
    if(sType == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFoundPlusFix("\"type\"", opt.getName(), "add \"type\": \"[yourtype]\"")});
        return res;
    }
    if(!sType.is_string()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongTypePlusFix("\"type\"", opt.getName(), "string", "change it into \"type\": \"[yourtype]\"")});
        return res;
    }
    if(sType != "fabric"){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverWrongInstanceGenerated("Fabric")});
        return res;
    }

    nlohmann::json sLoaderVer = opt.getValue("loader_version");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    if(sLoaderVer == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFoundPlusFix("\"loader_version\"", opt.getName(), "add \"loader_version\": \"latest\" to server.json for automatic download")});
        return res;
    }
    if(!sLoaderVer.is_string()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongTypePlusFix("\"loader_version\"", opt.getName(), "string", "add \"loader_version\": \"latest\" to server.json for automatic download.")});
        return res;
    }
    if(sLoaderVer != "latest"){
        loaderVer = sLoaderVer.get<std::string>();
        if(mcsm::isWhitespaceOrEmpty(loaderVer)){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFoundPlusFix("\"loader_version\"", opt.getName(), "add \"loader_version\": \"latest\" to server.json for automatic download")});
            return res;
        }
    }else if(sLoaderVer == "latest"){
        loaderVer = getVersion(version);
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
            std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
            mcsm::Result res(resp.first, resp.second);
            return res;
        }
        if(mcsm::isWhitespaceOrEmpty(loaderVer)){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
                "Unsupported loader version.",
                "Please try again with a different version."
            }});
            return res;
        }
    }


    nlohmann::json sInstallerVer = opt.getValue("installer_version");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    if(sInstallerVer == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFoundPlusFix("\"installer_version\"", opt.getName(), "add \"installer_version\": \"latest\" to server.json for automatic download")});
        return res;
    }
    if(!sInstallerVer.is_string()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongTypePlusFix("\"installer_version\"", opt.getName(), "string", "add \"installer_version\": \"latest\" to server.json for automatic download.")});
        return res;
    }
    if(sInstallerVer != "latest"){
        installerVer = sInstallerVer.get<std::string>();
        if(mcsm::isWhitespaceOrEmpty(installerVer)){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFoundPlusFix("\"installer_version\"", opt.getName(), "add \"installer_version\": \"latest\" to server.json for automatic download")});
            return res;
        }
    }else if(sInstallerVer == "latest"){
        installerVer = getVersion();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
            std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
            mcsm::Result res(resp.first, resp.second);
            return res;
        }
        if(mcsm::isWhitespaceOrEmpty(installerVer)){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
                "Unsupported installer version.",
                "Please try again with a different version."
            }});
            return res;
        }
    }

    return download(version, loaderVer, installerVer, path, name, optionPath);
}

mcsm::Result mcsm::FabricServer::download(const std::string& version, const std::string& loaderVersion, const std::string& installerVersion, const std::string& path, const std::string& name){
    std::string optionPath = mcsm::getCurrentPath();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    return download(version, loaderVersion, installerVersion, path, name, optionPath);
}

mcsm::Result mcsm::FabricServer::download(const std::string& version, const std::string& loaderVersion, const std::string& installerVersion, const std::string& path, const std::string& name, const std::string& optionPath){
    //The reason why variable path and optionPath are separated is because of flexibility
    mcsm::Option opt(optionPath, "server");
    bool exists = opt.exists();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    if(!exists){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverNotConfigured()});
        return res;
    }

    nlohmann::json sType = opt.getValue("type");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }
    if(sType == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFoundPlusFix("\"type\"", opt.getName(), "add \"type\": \"[yourtype]\"")});
        return res;
    }
    if(!sType.is_string()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongTypePlusFix("\"type\"", opt.getName(), "string", "change it into \"type\": \"[yourtype]\"")});
        return res;
    }
    if(sType != "fabric"){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverWrongInstanceGenerated("Fabric")});
        return res;
    }
    mcsm::FabricServerDataOption sDataOpt(optionPath);
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    nlohmann::json sLoaderVer = opt.getValue("loader_version");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    if(sLoaderVer == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFoundPlusFix("\"loader_version\"", opt.getName(), "add \"loader_version\": \"latest\" to server.json for automatic download")});
        return res;
    }
    if(!sLoaderVer.is_string()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongTypePlusFix("\"loader_version\"", opt.getName(), "string", "add \"loader_version\": \"latest\" to server.json for automatic download.")});
        return res;
    }

    //line 499 will decide if the version exists; no need to check
    /*
    if(sLoaderVer != "latest"){
        loaderVer = sLoaderVer.get<std::string>();
        if(mcsm::isWhitespaceOrEmpty(loaderVer)){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFoundPlusFix("\"loader_version\"", opt.getName(), "add \"loader_version\": \"latest\" to server.json for automatic download")});
            return res;
        }
    }else if(sLoaderVer == "latest"){
        loaderVer = getVersion(version);
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
            std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
            mcsm::Result res(resp.first, resp.second);
            return res;
        }
        if(mcsm::isWhitespaceOrEmpty(loaderVer)){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
                "Unsupported loader version.",
                "Please try again with a different version."
            }});
            return res;
        }
    }
    */


    nlohmann::json sInstallerVer = opt.getValue("installer_version");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    if(sInstallerVer == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFoundPlusFix("\"installer_version\"", opt.getName(), "add \"installer_version\": \"latest\" to server.json for automatic download")});
        return res;
    }
    if(!sInstallerVer.is_string()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongTypePlusFix("\"installer_version\"", opt.getName(), "string", "add \"installer_version\": \"latest\" to server.json for automatic download.")});
        return res;
    }

    //line 499 will decide if the version exists; no need to check
    /*
    if(sInstallerVer != "latest"){
        installerVer = sInstallerVer.get<std::string>();
        if(mcsm::isWhitespaceOrEmpty(installerVer)){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFoundPlusFix("\"installer_version\"", opt.getName(), "add \"installer_version\": \"latest\" to server.json for automatic download")});
            return res;
        }
    }else if(sInstallerVer == "latest"){
        installerVer = getVersion();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
            std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
            mcsm::Result res(resp.first, resp.second);
            return res;
        }
        if(mcsm::isWhitespaceOrEmpty(installerVer)){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
                "Unsupported installer version.",
                "Please try again with a different version."
            }});
            return res;
        }
    }
    */

    std::string url = "https://meta.fabricmc.net/v2/versions/loader/" + version + "/" + loaderVersion + "/" + installerVersion + "/server/jar";
    mcsm::info("URL : " + url);

    bool text = mcsm::isText(url);
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    if(text){
        std::string result = mcsm::get(url);
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
            std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
            mcsm::Result res(resp.first, resp.second);
            return res;
        }
    
        if(result == "Unable to find valid version for loader_version" || result == "Fabric loader 0.12 or higher is required for unattended server installs. Please use a newer fabric loader version, or the full installer."){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
                "Unsupported loader version : " + loaderVersion,
                "Please try again with a different version."
            }});
            return res;
        }else if(result == "Unable to find valid version for installer_version" || result == "Fabric loader 0.12 or higher is required for unattended server installs. Please use a newer fabric loader version, or the full installer."){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
                "Unsupported installer version : " + installerVersion,
                "Please try again with a different version."
            }});
            return res;
        }else{
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
                "Cannot download the file due to the following reason : " + result,
                "Please report this to GitHub (https://github.com/dodoman8067/mcsm)."
            }});
            return res;
        }
    }
    
    mcsm::Result res1 = mcsm::download(name, url, path, true);
    if(!res1.isSuccess()) return res1;
    mcsm::Result res2 = sDataOpt.updateLoaderVersion(loaderVersion);
    if(!res2.isSuccess()) return res2;
    mcsm::Result res3 = sDataOpt.updateInstallerVersion(installerVersion);
    if(!res3.isSuccess()) return res3;

    mcsm::Result res4({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
    return res4;
}

mcsm::Result mcsm::FabricServer::start(mcsm::JvmOption& option){
    std::string cPath = mcsm::getCurrentPath();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    return start(option, cPath, cPath);
}

mcsm::Result mcsm::FabricServer::start(mcsm::JvmOption& option, const std::string& path, const std::string& optionPath){
    // ServerOption class handles the data file stuff
    mcsm::FabricServerOption sOpt(optionPath);
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }
    
    std::string jar = getJarFile(optionPath);
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    bool fileExists = mcsm::fileExists(path + "/" + jar);
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    if(!fileExists){
        mcsm::info("Downloading " + jar + "...");
        std::string sVer = sOpt.getServerVersion();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
            std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
            mcsm::Result res(resp.first, resp.second);
            return res;
        }

        mcsm::Result res = download(sVer, path, jar, optionPath);
        if(!res.isSuccess()) return res;
    }else{
        bool doesUpdate = sOpt.doesAutoUpdate();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
            std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
            mcsm::Result res(resp.first, resp.second);
            return res;
        }
        
        if(doesUpdate){
            mcsm::Result res = update(optionPath);
            if(!res.isSuccess()) return res;
        }
    }
    return Server::start(option, path, optionPath);
}

mcsm::Result mcsm::FabricServer::update(){
    std::string path = mcsm::getCurrentPath();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    return update(path);
}

mcsm::Result mcsm::FabricServer::update(const std::string& optionPath){
    mcsm::FabricServerDataOption sDataOpt(optionPath);
    mcsm::Option opt(optionPath, "server");
    bool exists = opt.exists();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    if(!exists){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverNotConfigured()});
        return res;
    }

    mcsm::info("Checking updates...");

    nlohmann::json sLoaderVer = opt.getValue("loader_version");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    if(sLoaderVer == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFoundPlusFix("\"loader_version\"", opt.getName(), "add \"loader_version\": \"latest\" to server.json for automatic download")});
        return res;
    }
    if(!sLoaderVer.is_string()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongTypePlusFix("\"loader_version\"", opt.getName(), "string", "add \"loader_version\": \"latest\" to server.json for automatic download.")});
        return res;
    }
    if(sLoaderVer != "latest"){
        mcsm::warning("This server won't update to the latest loader version.");
        mcsm::warning("Change server.json into \"loader_version\": \"latest\" for automatic download.");
    }

    nlohmann::json sInstallerVer = opt.getValue("installer_version");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    if(sInstallerVer == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFoundPlusFix("\"installer_version\"", opt.getName(), "add \"installer_version\": \"latest\" to server.json for automatic download")});
        return res;
    }
    if(!sInstallerVer.is_string()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongTypePlusFix("\"installer_version\"", opt.getName(), "string", "add \"installer_version\": \"latest\" to server.json for automatic download.")});
        return res;
    }
    if(sInstallerVer != "latest"){
        mcsm::warning("This server won't update to the latest installer version.");
        mcsm::warning("Change server.json into \"installer_version\": \"latest\" for automatic download.");
    }

    nlohmann::json sVer = opt.getValue("version");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    if(sVer == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFound("\"version\"", opt.getName())});
        return res;
    }
    if(!sVer.is_string()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"version\"", "string")});
        return res;
    }

    std::string loaderVer, installerVer;

    loaderVer = sLoaderVer;
    installerVer = sInstallerVer;

    std::string tempInstallerVer = sDataOpt.getInstallerVersion();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    std::string tempLoaderVer = sDataOpt.getLoaderVersion();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    if(tempInstallerVer == installerVer && tempLoaderVer == loaderVer){
        mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {
            "Success"
        }});
        return res;
    }

    if(sLoaderVer == "latest"){
        std::string mcver = sVer;
        loaderVer = getVersion(mcver);
        if(mcsm::isWhitespaceOrEmpty(loaderVer)){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
                "Unsupported loader version. (Can't find a loader version for Minecraft " + mcver + ")",
                "Please try again with a different version."
            }});
            return res;
        }
        if(tempLoaderVer != loaderVer){
            mcsm::success("Update found for the loader version : "  + loaderVer + ". Current build : " + tempLoaderVer);
        }
    }
    if(sInstallerVer == "latest"){
        installerVer = getVersion();
        if(mcsm::isWhitespaceOrEmpty(installerVer)){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
                "Unsupported installer version.",
                "Please try again with a different version."
            }});
            return res;
        }
        if(tempInstallerVer != installerVer){
            mcsm::success("Update found for the installer version : "  + installerVer + ". Current build : " + tempInstallerVer);
        }
    }

    if(tempInstallerVer == installerVer && tempLoaderVer == loaderVer){
        mcsm::success("Server is up to date.");
        mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {
            "Success"
        }});
        return res;
    }

    std::string jar = getJarFile(optionPath);
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    bool fileExists = mcsm::fileExists(optionPath + "/" + jar);
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    if(fileExists){
        mcsm::removeFile(optionPath + "/" + jar);
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
            std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
            mcsm::Result res(resp.first, resp.second);
            return res;
        }
    }

    return download(sVer, loaderVer, installerVer, optionPath, jar, optionPath);
}

bool mcsm::FabricServer::hasVersion(const std::string& version){
    return !mcsm::isWhitespaceOrEmpty(getVersion(version));
}

std::string mcsm::FabricServer::getTypeAsString() const {
    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
    return "fabric";
}

mcsm::ServerType mcsm::FabricServer::getType() const {
    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
    return mcsm::ServerType::FABRIC;
}