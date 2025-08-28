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

mcsm::IntResult mcsm::PurpurServer::getVersion(const std::string& ver) const {
    if(!mcsm::isSafeString(ver)){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::UNSAFE_STRING, {ver});
        return tl::unexpected(err);
    }
    auto res = mcsm::get("https://api.purpurmc.org/v2/purpur/" + ver);
    if(!res) return tl::unexpected(res.error());
    nlohmann::json json = nlohmann::json::parse(res.value(), nullptr, false);
    if(json.is_discarded()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::GET_REQUEST_FAILED, {"https://api.purpurmc.org/v2/purpur/" + ver, "Invalid API json responce"});
        return tl::unexpected(err);
    }
    if(json["builds"] != nullptr){
        nlohmann::json builds = json["builds"];
        if(builds["latest"] == nullptr) return -1;
        return std::stoi(builds["latest"].get<std::string>());
    }else{
        return -1; // keep it this way; otherwise it returns invalid get error instead of unsupported version error
    }
}

mcsm::IntResult mcsm::PurpurServer::getVersion(const std::string& ver, const std::string& build) const {
    if(!mcsm::isSafeString(build)){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::UNSAFE_STRING, {build});
        return tl::unexpected(err);
    }
    if(!mcsm::isSafeString(ver)){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::UNSAFE_STRING, {ver});
        return tl::unexpected(err);
    }
    auto res = mcsm::get("https://api.purpurmc.org/v2/purpur/" + ver + "/" +  build);
    if(!res) return tl::unexpected(res.error());
    nlohmann::json json = nlohmann::json::parse(res.value(), nullptr, false);
    if(json.is_discarded()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::GET_REQUEST_FAILED, {"https://api.purpurmc.org/v2/purpur/" + ver + "/" + build, "Invalid API json responce"});
        return tl::unexpected(err);
    }

    if(json["build"] == nullptr){
        return -1; // keep it this way; otherwise it returns invalid get error instead of unsupported version error
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

mcsm::VoidResult mcsm::PurpurServer::download(const std::string& version){
    auto path1 = mcsm::getCurrentPath();
    if(!path1) return tl::unexpected(path1.error());

    auto jar = getJarFile();
    if(!jar) return tl::unexpected(jar.error());

    return download(version, path1.value(), jar.value(), path1.value());
}

mcsm::VoidResult mcsm::PurpurServer::download(const std::string& version, const std::string& path){
    auto path1 = mcsm::getCurrentPath();
    if(!path1) return tl::unexpected(path1.error());

    auto jar = getJarFile();
    if(!jar) return tl::unexpected(jar.error());

    return download(version, path, jar.value(), path1.value());
}

mcsm::VoidResult mcsm::PurpurServer::download(const std::string& version, const std::string& path, const std::string& name){
    auto path1 = mcsm::getCurrentPath();
    if(!path1) return tl::unexpected(path1.error());

    return download(version, path, name, path1.value());
}

mcsm::VoidResult mcsm::PurpurServer::download(const std::string& version, const std::string& path, const std::string& name, const std::string& optionPath){
    mcsm::Option opt(optionPath, "server");
    auto optExists = opt.exists();
    if(!optExists) return tl::unexpected(optExists.error());
    if(!optExists.value()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::SERVER_NOT_CONFIGURED, {});
        return tl::unexpected(err);
    }

    auto optLRes1 = opt.load(mcsm::GeneralOption::getGeneralOption().advancedParseEnabled());
    if(!optLRes1) return optLRes1;

    mcsm::ServerDataOption sDataOpt(optionPath);

    mcsm::VoidResult sLoadRes = sDataOpt.load();
    if(!sLoadRes) return sLoadRes;

    auto tVRes = opt.getValue("type");
    if(!tVRes) return tl::unexpected(tVRes.error());

    nlohmann::json typeValue = tVRes.value();
    if(typeValue == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_NOT_FOUND_PLUS_FIX, {"\"type\"", opt.getName(), "change it into \"type\": \"[yourtype]\""});
        return tl::unexpected(err);
    }
    if(!typeValue.is_string()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_WRONG_TYPE_PLUS_FIX, {"\"type\"", opt.getName(), "string", "change it into \"type\": \"[yourtype]\""});
        return tl::unexpected(err);
    }
    if(typeValue != "purpur"){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::SERVER_WRONG_INSTANCE_GENERATED, {"Purpur"});
        return tl::unexpected(err);
    }

    auto sBVRes = opt.getValue("server_build");
    if(!sBVRes) return tl::unexpected(sBVRes.error());

    nlohmann::json serverBuildValue = sBVRes.value();

    if(serverBuildValue == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_NOT_FOUND_PLUS_FIX, {"\"server_build\"", opt.getName(), "add \"server_build\": \"latest\" to server.json for automatic download"});
        return tl::unexpected(err);
    }
    if(!serverBuildValue.is_string()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_WRONG_TYPE_PLUS_FIX, {"\"server_build\"", opt.getName(), "string", "change it into \"server_build\": \"latest\""});
        return tl::unexpected(err);
    }
    if(serverBuildValue != "latest"){
        std::string build = serverBuildValue.get<std::string>();
        auto ver = getVersion(version, build);
        if(!ver) return tl::unexpected(ver.error());

        if(ver.value() == -1){
            mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::SERVER_UNSUPPORTED_VERSION, {build});
            return tl::unexpected(err);
        }
        std::string strVer = std::to_string(ver.value());
        std::string url = "https://api.purpurmc.org/v2/purpur/" + version + "/" + strVer + "/download/";
        mcsm::info("URL : " + url);
        mcsm::VoidResult res = mcsm::download(name, url, path, true);
        if(!res) return res;
        return sDataOpt.updateLastDownloadedBuild(strVer);
    }else{
        auto ver = getVersion(version);
        if(!ver) return tl::unexpected(ver.error());

        if(ver.value() == -1){
            mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::SERVER_UNSUPPORTED_VERSION, {});
            return tl::unexpected(err);
        }
        std::string strVer = std::to_string(ver.value());
        std::string url = "https://api.purpurmc.org/v2/purpur/" + version + "/" + strVer + "/download/";
        mcsm::info("URL : " + url);
        mcsm::VoidResult res = mcsm::download(name, url, path, true);
        if(!res) return res;
        return sDataOpt.updateLastDownloadedBuild(strVer);
    }
}

mcsm::VoidResult mcsm::PurpurServer::obtainJarFile(const std::string& version, const std::string& path, const std::string& name, const std::string& optionPath){
    return download(version, path, name, optionPath);
}

mcsm::StringResult mcsm::PurpurServer::start(mcsm::ServerConfigLoader* loader, mcsm::JvmOption& option){
    // ServerOption class handles the data file stuff

    mcsm::StringResult cJarPath = loader->getServerJarPath();
    if(!cJarPath) return cJarPath;
    return start(loader, option, cJarPath.value(), loader->getHandle()->getPath());
}

mcsm::StringResult mcsm::PurpurServer::start(mcsm::ServerConfigLoader* loader, mcsm::JvmOption& option, const std::string& path, const std::string& optionPath){
    // ServerOption class handles the data file stuff
    
    mcsm::StringResult jar = loader->getServerJarFile();
    if(!jar) return jar;

    mcsm::BoolResult fileExists = mcsm::fileExists(path + "/" + jar.value());
    if(!fileExists) return tl::unexpected(fileExists.error());

    if(!fileExists.value()){
        mcsm::info("Downloading " + mcsm::joinPath(path, jar.value()) + "...");
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

mcsm::VoidResult mcsm::PurpurServer::update(){
    auto path = mcsm::getCurrentPath();
    if(!path) return tl::unexpected(path.error());

    return update(path.value(), path.value());
}

mcsm::VoidResult mcsm::PurpurServer::update(const std::string& optionPath){
    auto path = mcsm::getCurrentPath();
    if(!path) return tl::unexpected(path.error());

    return update(path.value(), optionPath);
}

mcsm::VoidResult mcsm::PurpurServer::update(const std::string& path, const std::string& optionPath){
    // Program won't downgrade server jarfiles automatically. This is an intented feature.
    mcsm::info("Checking updates...");
    mcsm::ServerDataOption sDataOpt(optionPath);

    mcsm::VoidResult sLoadRes = sDataOpt.load();
    if(!sLoadRes) return sLoadRes;

    mcsm::ServerConfigLoader loader(optionPath);
    mcsm::VoidResult loadRes = loader.loadConfig();
    if(!loadRes) return loadRes;

    auto bGRes1 = loader.getServerJarBuild();
    if(!bGRes1) return tl::unexpected(bGRes1.error());
    std::string build = bGRes1.value();

    if(build != "latest"){
        mcsm::warning("This server won't update to the latest build.");
        mcsm::warning("Change server.json into \"server_build\": \"latest\" for automatic download.");
        return {};
    }

    auto versionGetResult = loader.getServerVersion();
    if(!versionGetResult) return tl::unexpected(versionGetResult.error());
    std::string version = versionGetResult.value();

    auto verRes = getVersion(version);
    if(!verRes) return tl::unexpected(verRes.error());
    int ver = verRes.value();
    if(ver == -1){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::SERVER_UNSUPPORTED_VERSION, {});
        return tl::unexpected(err);
    }

    auto lastBuildGetRes = sDataOpt.getLastDownloadedBuild();
    if(!lastBuildGetRes) return tl::unexpected(lastBuildGetRes.error());
    std::string lastBuild = lastBuildGetRes.value();

    if(lastBuild == std::to_string(ver)){
        mcsm::success("Server is up to date.");
        return {};
    }
    mcsm::success("Update found : "  + std::to_string(ver) + ". Current build : " + lastBuild);

    auto jarRes = loader.getServerJarFile();
    if(!jarRes) return tl::unexpected(jarRes.error());
    std::string jar = jarRes.value();

    auto fileExistsResult = mcsm::fileExists(path + "/" + jar);
    if(!fileExistsResult) return tl::unexpected(fileExistsResult.error());
    bool fileExists = fileExistsResult.value();

    if(fileExists){
        auto rmRes = mcsm::removeFile(path + "/" + jar);
        if(!rmRes) return tl::unexpected(rmRes.error());
    }
    return download(version, path, jar, optionPath);
}

mcsm::VoidResult mcsm::PurpurServer::generate(const std::string& name, mcsm::JvmOption& option, const std::string& path, const std::string& version, const bool& autoUpdate, const std::map<std::string, std::string>& extraValues){
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

    // No need to call opt.load() here. create() in ServerDataOption will call it eventually

    return configure(version, this, &opt, path, name, option, autoUpdate, extraValues.find("server_build_version")->second, extraValues.find("server_jarfile")->second);
}

mcsm::BoolResult mcsm::PurpurServer::hasVersion(const std::string& version) const {
    return getVersion(version) != -1;
}

std::string mcsm::PurpurServer::getTypeAsString() const {
    return "purpur";
}