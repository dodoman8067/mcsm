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
mcsm::StringResult mcsm::FabricServer::getVersion(const std::string& ver) const {
    if(!mcsm::isSafeString(ver)){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::UNSAFE_STRING, {ver});
        return tl::unexpected(err);
    }
    auto res = mcsm::get("https://meta.fabricmc.net/v2/versions/loader/" + ver);
    if(!res) return res;
    nlohmann::json json = nlohmann::json::parse(res.value(), nullptr, false);
    if(json.is_discarded()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::GET_REQUEST_FAILED, {"https://meta.fabricmc.net/v2/versions/loader/" + ver, "Invalid API json responce"});
        return tl::unexpected(err);
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
mcsm::StringResult mcsm::FabricServer::getVersion() const {
    auto res = mcsm::get("https://meta.fabricmc.net/v2/versions/installer");
    if(!res) return res;
    nlohmann::json json = nlohmann::json::parse(res.value(), nullptr, false);
    if(json.is_discarded()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::GET_REQUEST_FAILED, {"https://meta.fabricmc.net/v2/versions/installer", "Invalid API json responce"});
        return tl::unexpected(err);
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

mcsm::VoidResult mcsm::FabricServer::download(const std::string& version){
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

mcsm::VoidResult mcsm::FabricServer::download(const std::string& version, const std::string& path){
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

mcsm::VoidResult mcsm::FabricServer::download(const std::string& version, const std::string& path, const std::string& name){
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

    opt.load(mcsm::GeneralOption::getGeneralOption().advancedParseEnabled());
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
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

mcsm::VoidResult mcsm::FabricServer::download(const std::string& version, const std::string& path, const std::string& name, const std::string& optionPath){
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

    opt.load(mcsm::GeneralOption::getGeneralOption().advancedParseEnabled());
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
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

mcsm::VoidResult mcsm::FabricServer::download(const std::string& version, const std::string& loaderVersion, const std::string& installerVersion, const std::string& path, const std::string& name){
    std::string optionPath = mcsm::getCurrentPath();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    return download(version, loaderVersion, installerVersion, path, name, optionPath);
}

mcsm::VoidResult mcsm::FabricServer::download(const std::string& version, const std::string& loaderVersion, const std::string& installerVersion, const std::string& path, const std::string& name, const std::string& optionPath){
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

    opt.load(mcsm::GeneralOption::getGeneralOption().advancedParseEnabled());
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
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

    mcsm::Result sLoadRes = sDataOpt.load();
    if(!sLoadRes.isSuccess()) return sLoadRes;

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

mcsm::VoidResult mcsm::FabricServer::obtainJarFile(const std::string& version, const std::string& path, const std::string& name, const std::string& optionPath){
    return download(version, path, name, optionPath);
}

mcsm::StringResult mcsm::FabricServer::start(mcsm::ServerConfigLoader* loader, mcsm::JvmOption& option){
    // ServerOption class handles the data file stuff
    mcsm::StringResult cPath = mcsm::getCurrentPath();
    if(!cPath) return cPath;
    return start(loader, option, cPath.value(), cPath.value());
}

mcsm::StringResult mcsm::FabricServer::start(mcsm::ServerConfigLoader* loader, mcsm::JvmOption& option, const std::string& path, const std::string& optionPath){
    // ServerOption class handles the data file stuff
    
    mcsm::StringResult jar = loader->getServerJarFile();
    if(!jar) return jar;

    mcsm::BoolResult fileExists = mcsm::fileExists(path + "/" + jar);
    if(!fileExists) return tl::unexpected(fileExists.error());

    if(!fileExists.value()){
        mcsm::info("Downloading " + jar + "...");
        mcsm::StringResult sVer = loader->getServerVersion();
        if(!sVer) return sVer;

        mcsm::VoidResult res = download(sVer.value(), path, jar.value(), optionPath);
        if(!res) return res;
    }else{
        mcsm::BoolResult doesUpdate = loader->doesAutoUpdate();
        if(!doesUpdate) return doesUpdate;

        if(doesUpdate.value()){
            mcsm::VoidResult res = update(path, optionPath);
            if(!res) return res;
        }
    }
    return Server::start(loader, option, path, optionPath);
}

mcsm::VoidResult mcsm::FabricServer::update(){
    std::string path = mcsm::getCurrentPath();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    return update(path, path);
}

mcsm::VoidResult mcsm::FabricServer::update(const std::string& optionPath){
    std::string path = mcsm::getCurrentPath();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    return update(path, optionPath);
}

mcsm::VoidResult mcsm::FabricServer::update(const std::string& path, const std::string& optionPath){
    mcsm::FabricServerDataOption sDataOpt(optionPath);
    mcsm::ServerConfigLoader loader(optionPath);

    mcsm::Result sLoadRes = sDataOpt.load();
    if(!sLoadRes.isSuccess()) return sLoadRes;

    mcsm::Result loadRes = loader.loadConfig();
    if(!loadRes.isSuccess()) return loadRes;

    std::string loaderVer, installerVer;

    mcsm::info("Checking updates...");

    loaderVer = loader.get<std::string>("loader_version");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    if(loaderVer != "latest"){
        mcsm::warning("This server won't update to the latest loader version.");
        mcsm::warning("Change server.json into \"loader_version\": \"latest\" for automatic download.");
    }

    installerVer = loader.get<std::string>("installer_version");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    if(installerVer != "latest"){
        mcsm::warning("This server won't update to the latest installer version.");
        mcsm::warning("Change server.json into \"installer_version\": \"latest\" for automatic download.");
    }

    std::string sVer = loader.getServerVersion();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

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

    if(loaderVer == "latest"){
        loaderVer = getVersion(sVer);
        if(mcsm::isWhitespaceOrEmpty(loaderVer)){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
                "Unsupported loader version. (Can't find a loader version for Minecraft " + sVer + ")",
                "Please try again with a different version."
            }});
            return res;
        }
        if(tempLoaderVer != loaderVer){
            mcsm::success("Update found for the loader version : "  + loaderVer + ". Current build : " + tempLoaderVer);
        }
    }
    if(installerVer == "latest"){
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

    bool fileExists = mcsm::fileExists(path + "/" + jar);
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    if(fileExists){
        mcsm::removeFile(path + "/" + jar);
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
            std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
            mcsm::Result res(resp.first, resp.second);
            return res;
        }
    }

    return download(sVer, loaderVer, installerVer, path, jar, optionPath);
}

mcsm::VoidResult mcsm::FabricServer::generate(const std::string& name, mcsm::JvmOption& option, const std::string& path, const std::string& version, const bool& autoUpdate, const std::map<std::string, std::string>& extraValues){
    mcsm::GeneralProperty* property = mcsm::GeneralOption::getGeneralOption().getProperty("skip_version_check_while_configuring");

    if(property == nullptr){
        return {mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFoundPlusFix("skip_version_check_while_configuring", "general option", "\"skip_version_check_while_configuring\": false")};
    }

    const nlohmann::json& propertyValue = property->getCurrentValue();
    if(!propertyValue.is_boolean()){
        return {mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongTypePlusFix("skip_version_check_while_configuring", "general option", "boolean", "false or true")};
    }

    bool skipCheck = propertyValue;

    if(!skipCheck){
        bool vExists = this->hasVersion(version);
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
            std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
            mcsm::Result res(resp.first, resp.second);
            return res;
        }
        if(!vExists){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverUnsupportedVersion()});
            return res;
        }
    }
    mcsm::FabricServerDataOption fSDOpt(path);
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    // No need to call fSDOpt.load() here. create() in ServerDataOption will call it eventually

    mcsm::ServerConfigGenerator generator(path);
    mcsm::Result generateRes = generator.generate(version, this, &fSDOpt, name, option, autoUpdate);
    if(!generateRes.isSuccess()) return generateRes;

    std::unique_ptr<mcsm::Option>& fabricOpt = generator.getHandle();
    mcsm::Result res9 = fabricOpt->setValue("loader_version", extraValues.find("server_loader_version")->second);
    if(!res9.isSuccess()) return res9;

    mcsm::Result res10 = fabricOpt->setValue("installer_version", extraValues.find("server_installer_version")->second);
    if(!res10.isSuccess()) return res10;
    
    mcsm::Result fSaveRes = fabricOpt->save();
    if(!fSaveRes.isSuccess()) return fSaveRes;

    mcsm::ServerConfigLoader serverOption(path);
    mcsm::Result loadRes = serverOption.loadConfig();
    if(!loadRes.isSuccess()) return loadRes;

    std::string sName = serverOption.getServerName();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    std::string type = serverOption.getServerType();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    std::string sVersion = serverOption.getServerVersion();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    std::string profile = serverOption.getDefaultOption()->getProfileName();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    mcsm::success("Configured server's information : ");
    mcsm::info("Server name : " + sName);
    mcsm::info("Server type : " + type);
    mcsm::info("Server version : " + sVersion);
    mcsm::info("Server JVM launch profile : " + profile);
    if(!autoUpdate) mcsm::info("Automatic updates : disabled");

    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
    return res;
}

const tl::expected<std::map<std::string, std::string>, mcsm::Error> mcsm::FabricServer::getRequiredValues() const {
    return {
        {"name", "" },
        {"minecraft_version", ""},
        {"default_jvm_launch_profile_search_path", "current"},
        {"default_jvm_launch_profile_name", ""},
        {"server_jarfile_name", getTypeAsString() + ".jar"},
        {"server_loader_version", "latest"},
        {"server_installer_version", "latest"},
        {"auto_server_jar_update", "true"}
    };
}

mcsm::BoolResult mcsm::FabricServer::hasVersion(const std::string& version) const {
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