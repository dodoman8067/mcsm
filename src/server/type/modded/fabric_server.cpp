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
    auto path1 = mcsm::getCurrentPath();
    if(!path1) return tl::unexpected(path1.error());
    
    auto jar = getJarFile();
    if(!jar) return tl::unexpected(jar.error());

    return download(version, path1.value(), jar.value(), path1.value());
}

mcsm::VoidResult mcsm::FabricServer::download(const std::string& version, const std::string& path){
    auto path1 = mcsm::getCurrentPath();
    if(!path1) return tl::unexpected(path1.error());

    auto jar = getJarFile();
    if(!jar) return tl::unexpected(jar.error());

    return download(version, path, jar.value(), path1.value());
}

mcsm::VoidResult mcsm::FabricServer::download(const std::string& version, const std::string& path, const std::string& name){
    auto optionPpath1 = mcsm::getCurrentPath();
    if(!optionPpath1) return tl::unexpected(optionPpath1.error());
    std::string optionPath = optionPpath1.value();

    mcsm::Option opt(optionPath, "server");
    auto optExists = opt.exists();
    if(!optExists) return tl::unexpected(optExists.error());
    if(!optExists.value()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::SERVER_NOT_CONFIGURED, {});
        return tl::unexpected(err);
    }

    auto optLRes1 = opt.load(mcsm::GeneralOption::getGeneralOption().advancedParseEnabled());
    if(!optLRes1) return optLRes1;

    std::string loaderVer, installerVer;

    auto sTREs = opt.getValue("type");
    if(!sTREs) return tl::unexpected(sTREs.error());
    nlohmann::json sType = sTREs.value();
    if(sType == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_NOT_FOUND_PLUS_FIX, {"\"type\"", opt.getName(), "add \"type\": \"[yourtype]\""});
        return tl::unexpected(err);
    }
    if(!sType.is_string()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_WRONG_TYPE_PLUS_FIX, {"\"type\"", opt.getName(), "string", "change it into \"type\": \"[yourtype]\""});
        return tl::unexpected(err);
    }
    if(sType != "fabric"){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::SERVER_WRONG_INSTANCE_GENERATED, {"Fabric"});
        return tl::unexpected(err);
    }

    auto sLGREs = opt.getValue("loader_version");
    if(!sLGREs) return tl::unexpected(sLGREs.error());
    nlohmann::json sLoaderVer = sLGREs.value();

    if(sLoaderVer == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_NOT_FOUND_PLUS_FIX, {"\"loader_version\"", opt.getName(), "add \"loader_version\": \"latest\" to server.json for automatic download"});
        return tl::unexpected(err);
    }
    if(!sLoaderVer.is_string()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_WRONG_TYPE_PLUS_FIX, {"\"loader_version\"", opt.getName(), "string", "add \"loader_version\": \"latest\" to server.json for automatic download."});
        return tl::unexpected(err);
    }
    if(sLoaderVer != "latest"){
        loaderVer = sLoaderVer.get<std::string>();
        if(mcsm::isWhitespaceOrEmpty(loaderVer)){
            mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_NOT_FOUND_PLUS_FIX, {"\"loader_version\"", opt.getName(), "add \"loader_version\": \"latest\" to server.json for automatic download"});
            return tl::unexpected(err);
        }
    }else if(sLoaderVer == "latest"){
        auto gVRes = getVersion(version);
        if(!gVRes) return tl::unexpected(gVRes.error());
        loaderVer = gVRes.value();
        if(mcsm::isWhitespaceOrEmpty(loaderVer)){
            auto customTemp = mcsm::errors::SERVER_UNSUPPORTED_VERSION;
            customTemp.message = "Unsupported loader version.";
            mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, customTemp, {});
            return tl::unexpected(err);
        }
    }

    auto sILVGRes = opt.getValue("installer_version");
    if(!sILVGRes) return tl::unexpected(sILVGRes.error());
    nlohmann::json sInstallerVer = sILVGRes.value();

    if(sInstallerVer == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_NOT_FOUND_PLUS_FIX, {"\"installer_version\"", opt.getName(), "add \"installer_version\": \"latest\" to server.json for automatic download"});
        return tl::unexpected(err);
    }
    if(!sInstallerVer.is_string()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_WRONG_TYPE_PLUS_FIX, {"\"installer_version\"", opt.getName(), "string", "add \"installer_version\": \"latest\" to server.json for automatic download."});
        return tl::unexpected(err);
    }
    if(sInstallerVer != "latest"){
        installerVer = sInstallerVer.get<std::string>();
        if(mcsm::isWhitespaceOrEmpty(installerVer)){
            mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_NOT_FOUND_PLUS_FIX, {"\"installer_version\"", opt.getName(), "add \"installer_version\": \"latest\" to server.json for automatic download"});
            return tl::unexpected(err);
        }
    }else if(sInstallerVer == "latest"){
        auto gVRes = getVersion();
        if(!gVRes) return tl::unexpected(gVRes.error());
        installerVer = gVRes.value();
        if(mcsm::isWhitespaceOrEmpty(installerVer)){
            auto customTemp = mcsm::errors::SERVER_UNSUPPORTED_VERSION;
            customTemp.message = "Unsupported installer version.";
            mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, customTemp, {});
            return tl::unexpected(err);
        }
    }

    return download(version, loaderVer, installerVer, path, name, optionPath);
}

mcsm::VoidResult mcsm::FabricServer::download(const std::string& version, const std::string& path, const std::string& name, const std::string& optionPath){
    mcsm::Option opt(optionPath, "server");
    auto optExists = opt.exists();
    if(!optExists) return tl::unexpected(optExists.error());
    if(!optExists.value()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::SERVER_NOT_CONFIGURED, {});
        return tl::unexpected(err);
    }

    auto optLoadRes1 = opt.load(mcsm::GeneralOption::getGeneralOption().advancedParseEnabled());
    if(!optLoadRes1) return optLoadRes1;

    std::string loaderVer, installerVer;

    auto sTREs = opt.getValue("type");
    if(!sTREs) return tl::unexpected(sTREs.error());
    nlohmann::json sType = sTREs.value();
    if(sType == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_NOT_FOUND_PLUS_FIX, {"\"type\"", opt.getName(), "add \"type\": \"[yourtype]\""});
        return tl::unexpected(err);
    }
    if(!sType.is_string()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_WRONG_TYPE_PLUS_FIX, {"\"type\"", opt.getName(), "string", "change it into \"type\": \"[yourtype]\""});
        return tl::unexpected(err);
    }
    if(sType != "fabric"){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::SERVER_WRONG_INSTANCE_GENERATED, {"Fabric"});
        return tl::unexpected(err);
    }

    auto sLGREs = opt.getValue("loader_version");
    if(!sLGREs) return tl::unexpected(sLGREs.error());
    nlohmann::json sLoaderVer = sLGREs.value();

    if(sLoaderVer == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_NOT_FOUND_PLUS_FIX, {"\"loader_version\"", opt.getName(), "add \"loader_version\": \"latest\" to server.json for automatic download"});
        return tl::unexpected(err);
    }
    if(!sLoaderVer.is_string()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_WRONG_TYPE_PLUS_FIX, {"\"loader_version\"", opt.getName(), "string", "add \"loader_version\": \"latest\" to server.json for automatic download."});
        return tl::unexpected(err);
    }
    if(sLoaderVer != "latest"){
        loaderVer = sLoaderVer.get<std::string>();
        if(mcsm::isWhitespaceOrEmpty(loaderVer)){
            mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_NOT_FOUND_PLUS_FIX, {"\"loader_version\"", opt.getName(), "add \"loader_version\": \"latest\" to server.json for automatic download"});
            return tl::unexpected(err);
        }
    }else if(sLoaderVer == "latest"){
        auto gVRes = getVersion(version);
        if(!gVRes) return tl::unexpected(gVRes.error());
        loaderVer = gVRes.value();
        if(mcsm::isWhitespaceOrEmpty(loaderVer)){
            auto customTemp = mcsm::errors::SERVER_UNSUPPORTED_VERSION;
            customTemp.message = "Unsupported loader version.";
            mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, customTemp, {});
            return tl::unexpected(err);
        }
    }

    auto sILVGRes = opt.getValue("installer_version");
    if(!sILVGRes) return tl::unexpected(sILVGRes.error());
    nlohmann::json sInstallerVer = sILVGRes.value();

    if(sInstallerVer == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_NOT_FOUND_PLUS_FIX, {"\"installer_version\"", opt.getName(), "add \"installer_version\": \"latest\" to server.json for automatic download"});
        return tl::unexpected(err);
    }
    if(!sInstallerVer.is_string()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_WRONG_TYPE_PLUS_FIX, {"\"installer_version\"", opt.getName(), "string", "add \"installer_version\": \"latest\" to server.json for automatic download."});
        return tl::unexpected(err);
    }
    if(sInstallerVer != "latest"){
        installerVer = sInstallerVer.get<std::string>();
        if(mcsm::isWhitespaceOrEmpty(installerVer)){
            mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_NOT_FOUND_PLUS_FIX, {"\"installer_version\"", opt.getName(), "add \"installer_version\": \"latest\" to server.json for automatic download"});
            return tl::unexpected(err);
        }
    }else if(sInstallerVer == "latest"){
        auto gVRes = getVersion();
        if(!gVRes) return tl::unexpected(gVRes.error());
        installerVer = gVRes.value();
        if(mcsm::isWhitespaceOrEmpty(installerVer)){
            auto customTemp = mcsm::errors::SERVER_UNSUPPORTED_VERSION;
            customTemp.message = "Unsupported installer version.";
            mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, customTemp, {});
            return tl::unexpected(err);
        }
    }

    return download(version, loaderVer, installerVer, path, name, optionPath);
}

mcsm::VoidResult mcsm::FabricServer::download(const std::string& version, const std::string& loaderVersion, const std::string& installerVersion, const std::string& path, const std::string& name){
    auto path1 = mcsm::getCurrentPath();
    if(!path1) return tl::unexpected(path1.error());

    return download(version, loaderVersion, installerVersion, path, name, path1.value());
}

mcsm::VoidResult mcsm::FabricServer::download(const std::string& version, const std::string& loaderVersion, const std::string& installerVersion, const std::string& path, const std::string& name, const std::string& optionPath){
    //The reason why variable path and optionPath are separated is because of flexibility
    mcsm::Option opt(optionPath, "server");
    auto optExists = opt.exists();
    if(!optExists) return tl::unexpected(optExists.error());
    if(!optExists.value()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::SERVER_NOT_CONFIGURED, {});
        return tl::unexpected(err);
    }

    auto optLoadRes1 = opt.load(mcsm::GeneralOption::getGeneralOption().advancedParseEnabled());
    if(!optLoadRes1) return optLoadRes1;

    auto sTREs = opt.getValue("type");
    if(!sTREs) return tl::unexpected(sTREs.error());
    nlohmann::json sType = sTREs.value();
    if(sType == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_NOT_FOUND_PLUS_FIX, {"\"type\"", opt.getName(), "add \"type\": \"[yourtype]\""});
        return tl::unexpected(err);
    }
    if(!sType.is_string()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_WRONG_TYPE_PLUS_FIX, {"\"type\"", opt.getName(), "string", "change it into \"type\": \"[yourtype]\""});
        return tl::unexpected(err);
    }
    if(sType != "fabric"){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::SERVER_WRONG_INSTANCE_GENERATED, {"Fabric"});
        return tl::unexpected(err);
    }
    mcsm::FabricServerDataOption sDataOpt(optionPath);

    mcsm::VoidResult sLoadRes = sDataOpt.load();
    if(!sLoadRes) return sLoadRes;

    auto sLGREs = opt.getValue("loader_version");
    if(!sLGREs) return tl::unexpected(sLGREs.error());
    nlohmann::json sLoaderVer = sLGREs.value();

    if(sLoaderVer == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_NOT_FOUND_PLUS_FIX, {"\"loader_version\"", opt.getName(), "add \"loader_version\": \"latest\" to server.json for automatic download"});
        return tl::unexpected(err);
    }
    if(!sLoaderVer.is_string()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_WRONG_TYPE_PLUS_FIX, {"\"loader_version\"", opt.getName(), "string", "add \"loader_version\": \"latest\" to server.json for automatic download."});
        return tl::unexpected(err);
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

    auto sILVGRes = opt.getValue("installer_version");
    if(!sILVGRes) return tl::unexpected(sILVGRes.error());
    nlohmann::json sInstallerVer = sILVGRes.value();

    if(sInstallerVer == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_NOT_FOUND_PLUS_FIX, {"\"installer_version\"", opt.getName(), "add \"installer_version\": \"latest\" to server.json for automatic download"});
        return tl::unexpected(err);
    }
    if(!sInstallerVer.is_string()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_WRONG_TYPE_PLUS_FIX, {"\"installer_version\"", opt.getName(), "string", "add \"installer_version\": \"latest\" to server.json for automatic download."});
        return tl::unexpected(err);
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

    auto text = mcsm::isText(url);
    if(!text) return tl::unexpected(text.error());

    if(text.value()){
        auto rGRes = mcsm::get(url);
        if(!rGRes) return tl::unexpected(rGRes.error());
        std::string result = rGRes.value();
    
        if(result == "Unable to find valid version for loader_version" || result == "Fabric loader 0.12 or higher is required for unattended server installs. Please use a newer fabric loader version, or the full installer."){
            auto customTemp = mcsm::errors::SERVER_UNSUPPORTED_VERSION;
            customTemp.message = "Unsupported loader version : " + loaderVersion;
            mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, customTemp, {});
            return tl::unexpected(err);
        }else if(result == "Unable to find valid version for installer_version" || result == "Fabric loader 0.12 or higher is required for unattended server installs. Please use a newer fabric loader version, or the full installer."){
            auto customTemp = mcsm::errors::SERVER_UNSUPPORTED_VERSION;
            customTemp.message = "Unsupported installer version : " + installerVersion;
            mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, customTemp, {});
            return tl::unexpected(err);
        }else{
            auto customTemp = mcsm::errors::INTERNAL_FUNC_EXECUTION_FAILED;
            customTemp.message = "mcsm::FabricServer::download() failed: Cannot download the file due to the following reason : " + result;
            mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, customTemp, {});
            return tl::unexpected(err);
        }
    }
    
    mcsm::VoidResult res1 = mcsm::download(name, url, path, true);
    if(!res1) return res1;
    mcsm::VoidResult res2 = sDataOpt.updateLoaderVersion(loaderVersion);
    if(!res2) return res2;

    return sDataOpt.updateInstallerVersion(installerVersion);
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

    mcsm::BoolResult fileExists = mcsm::fileExists(path + "/" + jar.value());
    if(!fileExists) return tl::unexpected(fileExists.error());

    if(!fileExists.value()){
        mcsm::info("Downloading " + jar.value() + "...");
        mcsm::StringResult sVer = loader->getServerVersion();
        if(!sVer) return sVer;

        mcsm::VoidResult res = download(sVer.value(), path, jar.value(), optionPath);
        if(!res) return tl::unexpected(res.error());
    }else{
        mcsm::BoolResult doesUpdate = loader->doesAutoUpdate();
        if(!doesUpdate) return tl::unexpected(doesUpdate.error());

        if(doesUpdate.value()){
            mcsm::VoidResult res = update(path, optionPath);
            if(!res) return tl::unexpected(res.error());
        }
    }
    return Server::start(loader, option, path, optionPath);
}

mcsm::VoidResult mcsm::FabricServer::update(){
    auto path1 = mcsm::getCurrentPath();
    if(!path1) return tl::unexpected(path1.error());

    return update(path1.value(), path1.value());
}

mcsm::VoidResult mcsm::FabricServer::update(const std::string& optionPath){
    auto path1 = mcsm::getCurrentPath();
    if(!path1) return tl::unexpected(path1.error());

    return update(path1.value(), optionPath);
}

mcsm::VoidResult mcsm::FabricServer::update(const std::string& path, const std::string& optionPath){
    mcsm::FabricServerDataOption sDataOpt(optionPath);
    mcsm::ServerConfigLoader loader(optionPath);

    mcsm::VoidResult sLoadRes = sDataOpt.load();
    if(!sLoadRes) return sLoadRes;

    mcsm::VoidResult loadRes = loader.loadConfig();
    if(!loadRes) return loadRes;

    std::string loaderVer, installerVer;

    mcsm::info("Checking updates...");


    auto lVGRes = loader.get<std::string>("loader_version");
    if(!lVGRes) return tl::unexpected(lVGRes.error());
    loaderVer = lVGRes.value();

    if(loaderVer != "latest"){
        mcsm::warning("This server won't update to the latest loader version.");
        mcsm::warning("Change server.json into \"loader_version\": \"latest\" for automatic download.");
    }

    auto iSTRes = loader.get<std::string>("installer_version");
    if(!iSTRes) return tl::unexpected(iSTRes.error());
    installerVer = iSTRes.value();

    if(installerVer != "latest"){
        mcsm::warning("This server won't update to the latest installer version.");
        mcsm::warning("Change server.json into \"installer_version\": \"latest\" for automatic download.");
    }

    auto sVer = loader.getServerVersion();
    if(!sVer) return tl::unexpected(sVer.error());

    auto tempInstallerVer = sDataOpt.getInstallerVersion();
    if(!tempInstallerVer) return tl::unexpected(tempInstallerVer.error());

    auto tempLoaderVer = sDataOpt.getLoaderVersion();
    if(!tempLoaderVer) return tl::unexpected(tempLoaderVer.error());

    if(tempInstallerVer.value() == installerVer && tempLoaderVer.value() == loaderVer){
        return {};
    }

    if(loaderVer == "latest"){
        auto gVRes = getVersion(sVer.value());
        if(!gVRes) return tl::unexpected(gVRes.error());
        installerVer = gVRes.value();
        if(mcsm::isWhitespaceOrEmpty(loaderVer)){
            auto customTemp = mcsm::errors::SERVER_UNSUPPORTED_VERSION;
            customTemp.message = "Unsupported loader version. (Can't find a loader version for Minecraft " + sVer.value() + ")";
            mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, customTemp, {});
            return tl::unexpected(err);
        }
        if(tempLoaderVer.value() != loaderVer){
            mcsm::success("Update found for the loader version : "  + loaderVer + ". Current build : " + tempLoaderVer.value());
        }
    }
    if(installerVer == "latest"){
        auto gVRes = getVersion();
        if(!gVRes) return tl::unexpected(gVRes.error());
        installerVer = gVRes.value();
        if(mcsm::isWhitespaceOrEmpty(installerVer)){
            auto customTemp = mcsm::errors::SERVER_UNSUPPORTED_VERSION;
            customTemp.message = "Unsupported installer version.";
            mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, customTemp, {});
            return tl::unexpected(err);
        }
        if(tempInstallerVer.value() != installerVer){
            mcsm::success("Update found for the installer version : "  + installerVer + ". Current build : " + tempInstallerVer.value());
        }
    }

    if(tempInstallerVer.value() == installerVer && tempLoaderVer.value() == loaderVer){
        mcsm::success("Server is up to date.");
        return {};
    }

    auto jar = getJarFile(optionPath);
    if(!jar) return tl::unexpected(jar.error());

    auto fileExists = mcsm::fileExists(path + "/" + jar.value());
    if(!fileExists) return tl::unexpected(fileExists.error());

    if(fileExists.value()){
        auto rmRes = mcsm::removeFile(path + "/" + jar.value());
        if(!rmRes) return tl::unexpected(rmRes.error());
    }

    return download(sVer.value(), loaderVer, installerVer, path, jar.value(), optionPath);
}

mcsm::VoidResult mcsm::FabricServer::generate(const std::string& name, mcsm::JvmOption& option, const std::string& path, const std::string& version, const bool& autoUpdate, const std::map<std::string, std::string>& extraValues){
    mcsm::GeneralProperty* property = mcsm::GeneralOption::getGeneralOption().getProperty("skip_version_check_while_configuring");

    if(property == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_NOT_FOUND, {"skip_version_check_while_configuring", "general option", "\"skip_version_check_while_configuring\": false"});
        return tl::unexpected(err);
    }

    const nlohmann::json& propertyValue = property->getCurrentValue();
    if(!propertyValue.is_boolean()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_WRONG_TYPE_PLUS_FIX, {"skip_version_check_while_configuring", "general option", "boolean", "false or true"});
        return tl::unexpected(err);
    }

    bool skipCheck = propertyValue;

    if(!skipCheck){
        auto vExists = this->hasVersion(version);
        if(!vExists){
            return tl::unexpected(vExists.error());
        }
        if(!vExists.value()){
            mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::SERVER_UNSUPPORTED_VERSION, {});
            return tl::unexpected(err);
        }
    }
    mcsm::FabricServerDataOption fSDOpt(path);

    // No need to call fSDOpt.load() here. create() in ServerDataOption will call it eventually

    mcsm::ServerConfigGenerator generator(path);
    auto generateRes = generator.generate(version, this, &fSDOpt, name, option, autoUpdate);
    if(!generateRes) return generateRes;

    auto fabricOpt = generator.getHandle();
    auto res9 = fabricOpt->setValue("loader_version", extraValues.find("server_loader_version")->second);
    if(!res9) return res9;

    auto res10 = fabricOpt->setValue("installer_version", extraValues.find("server_installer_version")->second);
    if(!res10) return res10;
    
    auto fSaveRes = fabricOpt->save();
    if(!fSaveRes) return fSaveRes;

    mcsm::ServerConfigLoader serverOption(path);
    auto loadRes = serverOption.loadConfig();
    if(!loadRes) return loadRes;

    auto sName = serverOption.getServerName();
    if(!sName) return tl::unexpected(sName.error());

    auto type = serverOption.getServerType();
    if(!type) return tl::unexpected(type.error());

    auto sVersion = serverOption.getServerVersion();
    if(!sVersion) return tl::unexpected(sVersion.error());

    auto profileInstanceRes = serverOption.getDefaultOption();
    if(!profileInstanceRes) return tl::unexpected(profileInstanceRes.error());

    std::string profile = profileInstanceRes.value()->getProfileName();

    mcsm::success("Configured server's information : ");
    mcsm::info("Server name : " + sName.value());
    mcsm::info("Server type : " + type.value());
    mcsm::info("Server version : " + sVersion.value());
    mcsm::info("Server JVM launch profile : " + profile);
    if(!autoUpdate) mcsm::info("Automatic updates : disabled");

    return {};
}

const tl::expected<std::map<std::string, std::string>, mcsm::Error> mcsm::FabricServer::getRequiredValues() const {

    return tl::expected<std::map<std::string, std::string>, mcsm::Error>{
        std::map<std::string, std::string>{
                {"name", "" },
                {"minecraft_version", ""},
                {"default_jvm_launch_profile_search_path", "current"},
                {"default_jvm_launch_profile_name", ""},
                {"server_jarfile_name", getTypeAsString() + ".jar"},
                {"server_loader_version", "latest"},
                {"server_installer_version", "latest"},
                {"auto_server_jar_update", "true"}
        }
    };
}

mcsm::BoolResult mcsm::FabricServer::hasVersion(const std::string& version) const {
    auto verG = getVersion(version);
    if(!verG) return tl::unexpected(verG.error());
    return !mcsm::isWhitespaceOrEmpty(verG.value());
}

std::string mcsm::FabricServer::getTypeAsString() const {
    return "fabric";
}

mcsm::ServerType mcsm::FabricServer::getType() const {
    return mcsm::ServerType::FABRIC;
}