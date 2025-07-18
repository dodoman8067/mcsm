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

#include <mcsm/server/type/base/vanilla_server.h>

mcsm::VanillaServer::VanillaServer(){}

mcsm::VanillaServer::~VanillaServer(){}

mcsm::StringResult mcsm::VanillaServer::getVersionObject(const std::string& ver) const {
    if(!mcsm::isSafeString(ver)){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::UNSAFE_STRING, {ver});
        return tl::unexpected(err);
    }
    auto jsonData = mcsm::get("https://launchermeta.mojang.com/mc/game/version_manifest.json");
    if(!jsonData) return jsonData;
    
    nlohmann::json data = nlohmann::json::parse(jsonData.value(), nullptr, false);
    if(data.is_discarded()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::GET_REQUEST_FAILED, {"https://launchermeta.mojang.com/mc/game/version_manifest.json", "Invalid API json responce"});
        return tl::unexpected(err);
    }

    // Check if "versions" array exists
    if(!data.contains("versions")){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::GET_REQUEST_FAILED, {"https://launchermeta.mojang.com/mc/game/version_manifest.json", "No \"versions\" array exists in vanilla server version manifest."});
        return tl::unexpected(err);
    }

    auto& versions = data["versions"];
    if(!versions.is_array()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::GET_REQUEST_FAILED, {"https://launchermeta.mojang.com/mc/game/version_manifest.json", "Value \"versions\" is not an array."});
        return tl::unexpected(err);
    }

    // Search for entry with given ID
    for(const auto& entry : versions){
        // Check if the entry is an object
        if(!entry.is_object()){
            mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::GET_REQUEST_FAILED, {"https://launchermeta.mojang.com/mc/game/version_manifest.json", "Entry in vanilla server API request is not an object."});
            return tl::unexpected(err);
        }

        // Check if "id" field exists and matches the given ID
        if(!entry.contains("id")){
            mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::GET_REQUEST_FAILED, {"https://launchermeta.mojang.com/mc/game/version_manifest.json", "Value \"id\" in vanilla server API request is not a string | not found."});
            return tl::unexpected(err);
        }

        if(!entry["id"].is_string()){
            mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::GET_REQUEST_FAILED, {"https://launchermeta.mojang.com/mc/game/version_manifest.json", "Value \"id\" in vanilla server API request is not a string."});
            return tl::unexpected(err);
        }

        if(entry["id"] == ver){
            return entry.dump();
        }
    }

    // Entry not found
    return "";
}

mcsm::StringResult mcsm::VanillaServer::getServerJarURL(const std::string& ver) const {
    auto versionJson = getVersionObject(ver);
    if(!versionJson) return versionJson;
    if(mcsm::isWhitespaceOrEmpty(versionJson.value())){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::SERVER_UNSUPPORTED_VERSION, {ver});
        return tl::unexpected(err);
    }
    nlohmann::json version = nlohmann::json::parse(versionJson.value(), nullptr, false);
    if(version.is_discarded()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::GET_REQUEST_FAILED, {"https://launchermeta.mojang.com/mc/game/version_manifest.json", "Invalid API json responce"});
        return tl::unexpected(err);
    }
    if(version["type"] == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::GET_REQUEST_FAILED, {"https://launchermeta.mojang.com/mc/game/version_manifest.json", "Value \"type\" not found. (Vanilla version object)"});
        return tl::unexpected(err);
    }
    if(version["url"] == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::GET_REQUEST_FAILED, {"https://launchermeta.mojang.com/mc/game/version_manifest.json", "Value \"url\" not found. (Vanilla version object)"});
        return tl::unexpected(err);
    }
    if(!version["type"].is_string()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::GET_REQUEST_FAILED, {"https://launchermeta.mojang.com/mc/game/version_manifest.json", "Value \"type\" is not a string. (Vanilla version object)"});
        return tl::unexpected(err);
    }
    if(!version["url"].is_string()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::GET_REQUEST_FAILED, {"https://launchermeta.mojang.com/mc/game/version_manifest.json", "Value \"url\" is not a string. (Vanilla version object)"});
        return tl::unexpected(err);
    }

    if(version["type"] == "old_beta" || version["type"] == "old_alpha"){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::GET_REQUEST_FAILED, {"https://launchermeta.mojang.com/mc/game/version_manifest.json", "No plans to support beta and alpha versions."});
        return tl::unexpected(err);
    }

    std::string url = version["url"];
    auto serverJson = mcsm::get(url);
    if(!serverJson) return serverJson;

    if(mcsm::isWhitespaceOrEmpty(serverJson.value())){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::SERVER_UNSUPPORTED_VERSION, {ver});
        return tl::unexpected(err);
    }

    nlohmann::json serverData = nlohmann::json::parse(serverJson.value(), nullptr, false);
    if(serverData.is_discarded()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::GET_REQUEST_FAILED, {url, "Invalid API JSON responce"});
        return tl::unexpected(err);
    }
    nlohmann::json downloadsValue = serverData["downloads"];
    if(downloadsValue == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::GET_REQUEST_FAILED, {url, "Value \"downloads\" not found."});
        return tl::unexpected(err);
    }
    if(!downloadsValue.is_object()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::GET_REQUEST_FAILED, {url, "Value \"downloads\" not a json object."});
        return tl::unexpected(err);
    }

    nlohmann::json serverValue = downloadsValue["server"];
    if(serverValue == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::GET_REQUEST_FAILED, {url, "Value \"server\" not found."});
        return tl::unexpected(err);
    }
    if(!serverValue.is_object()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::GET_REQUEST_FAILED, {url, "Value \"server\" not a json object."});
        return tl::unexpected(err);
    }

    nlohmann::json serverJarURL = serverValue["url"];
    if(serverJarURL == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::GET_REQUEST_FAILED, {url, "Value \"url\" not found."});
        return tl::unexpected(err);
    }
    if(!serverJarURL.is_string()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::GET_REQUEST_FAILED, {url, "Value \"url\" not a string."});
        return tl::unexpected(err);
    }

    std::string finalReturnValue = serverJarURL;
    return finalReturnValue;
}

std::vector<std::string> mcsm::VanillaServer::getAvailableVersions(){
    std::vector<std::string> vector;
    vector.reserve(this->versions->size());
    return vector;
}

mcsm::VoidResult mcsm::VanillaServer::download(const std::string& version){
    auto path1 = mcsm::getCurrentPath();
    if(!path1) return tl::unexpected(path1.error());

    auto jar = getJarFile();
    if(!jar) return tl::unexpected(jar.error());

    return download(version, path1.value(), jar.value(), path1.value());
}

mcsm::VoidResult mcsm::VanillaServer::download(const std::string& version, const std::string& path){
    auto path1 = mcsm::getCurrentPath();
    if(!path1) return tl::unexpected(path1.error());

    auto jar = getJarFile();
    if(!jar) return tl::unexpected(jar.error());

    return download(version, path, jar.value(), path1.value());
}

mcsm::VoidResult mcsm::VanillaServer::download(const std::string& version, const std::string& path, const std::string& name){
    auto hasVer = hasVersion(version);
    if(!hasVer) return tl::unexpected(hasVer.error());

    if(!hasVer.value()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::SERVER_UNSUPPORTED_VERSION, {});
        return tl::unexpected(err);
    }
    auto url = getServerJarURL(version);
    if(!url) return tl::unexpected(url.error());
    if(mcsm::isWhitespaceOrEmpty(url.value())){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::SERVER_UNSUPPORTED_VERSION, {});
        return tl::unexpected(err);
    }

    mcsm::info("URL : " + url.value());
    return mcsm::download(name, url.value(), path);
}

mcsm::VoidResult mcsm::VanillaServer::download(const std::string& version, const std::string& path, const std::string& name, const std::string& /* optionPath */){
    return download(version, path, name);
}

mcsm::VoidResult mcsm::VanillaServer::obtainJarFile(const std::string& version, const std::string& path, const std::string& name, const std::string& /* optionPath */){
    return download(version, path, name);
}

mcsm::StringResult mcsm::VanillaServer::start(mcsm::ServerConfigLoader* loader, mcsm::JvmOption& option){
    // ServerOption class handles the data file stuff
    mcsm::StringResult cPath = mcsm::getCurrentPath();
    if(!cPath) return cPath;
    return start(loader, option, cPath.value(), cPath.value());
}

mcsm::StringResult mcsm::VanillaServer::start(mcsm::ServerConfigLoader* loader, mcsm::JvmOption& option, const std::string& path, const std::string& optionPath){
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
    }
    return Server::start(loader, option, path, optionPath);
}

mcsm::VoidResult mcsm::VanillaServer::generate(const std::string& name, mcsm::JvmOption& option, const std::string& path, const std::string& version, const bool& autoUpdate, const std::map<std::string, std::string>& /* extraValues */){
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
        if(!vExists) return tl::unexpected(vExists.error());
        if(!vExists.value()){
            mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::SERVER_UNSUPPORTED_VERSION, {});
            return tl::unexpected(err);
        }
    }
    mcsm::ServerDataOption opt(path);

    return configure(version, this, &opt, path, name, option, autoUpdate, "ignored");
}

mcsm::BoolResult mcsm::VanillaServer::hasVersion(const std::string& version) const {
    auto ver = getVersionObject(version);
    if(!ver) return tl::unexpected(ver.error());
    return !mcsm::isWhitespaceOrEmpty(ver.value());
}

const tl::expected<std::map<std::string, std::string>, mcsm::Error> mcsm::VanillaServer::getRequiredValues() const {
    return tl::expected<std::map<std::string, std::string>, mcsm::Error>{
        std::map<std::string, std::string>{
            {"name", "" },
            {"Minecraft version", ""},
            {"default JVM launch profile search path (current/global)", "current"},
            {"default JVM launch profile name", ""},
            {"server jarfile name", getTypeAsString() + ".jar"}
        }
    };
}

mcsm::ServerType mcsm::VanillaServer::getType() const {
    return ServerType::VANILLA;
}

std::string mcsm::VanillaServer::getSupportedVersions() const {
    return "1.0~";
}

std::string mcsm::VanillaServer::getBasedServer() const {
    return "none";
}

std::string mcsm::VanillaServer::getTypeAsString() const {
    return "vanilla";
}

std::string mcsm::VanillaServer::getWebSite() const {
    return "https://minecraft.net";
}

std::string mcsm::VanillaServer::getGitHub() const {
    return "none";
}