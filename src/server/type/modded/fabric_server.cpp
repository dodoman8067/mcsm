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
    mcsm::FabricServerOption sOpt;
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

    bool fileExists = mcsm::fileExists(jar);
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

        mcsm::Result res = download(sVer);
        if(!res.isSuccess()) return res;
    }else{
        mcsm::Result res = update();
        if(!res.isSuccess()) return res;
    }
    return Server::start(option);
}

mcsm::Result mcsm::FabricServer::update(){
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
        mcsm::error("If you believe that this is a software issue, please report this to GitHub (https://github.com/dodoman8067/mcsm).");
        std::exit(1);
    }
    if(!opt.getValue("version").is_string()){
        mcsm::error("Value \"version\" has to be a string, but it's not.");
        mcsm::error("Manually editing the launch profile might have caused this issue.");
        mcsm::error("If you know what you're doing, I believe you that you know how to handle this issue.");
        mcsm::error("If you believe that this is a software issue, please report this to GitHub (https://github.com/dodoman8067/mcsm).");
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

    return download(opt.getValue("version"), loaderVer, installerVer, std::filesystem::current_path().string(), getJarFile());
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