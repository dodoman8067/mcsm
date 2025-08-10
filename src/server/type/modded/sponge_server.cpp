/*
MIT License

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

#include <mcsm/server/type/modded/sponge_server.h>

mcsm::SpongeServer::SpongeServer(){}

mcsm::SpongeServer::~SpongeServer(){}

// TODO: Use Velocity API (Instead of Paper)

// SpongeServer provides a method that returns the 'ver' required in getVersion method as users don't usually type versions like 3.3.0-SNAPSHOT
// it is something like a method that returns latest mc version

//ver: mc version

// Note: limit more than one will have to return keys[keys.size() - 1]
mcsm::StringResult mcsm::SpongeServer::getVersion(const std::string& ver) const {
    if(!mcsm::isSafeString(ver)){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::UNSAFE_STRING, {ver});
        return tl::unexpected(err);
    }
    auto cPath = mcsm::getCurrentPath();
    if(!cPath) return cPath;

    mcsm::Option opt(cPath.value(), "server");

    bool bRecommended = false;
    
    auto optExists = opt.exists();
    if(!optExists) return tl::unexpected(optExists.error());

    if(optExists.value()){
        bool advp = mcsm::GeneralOption::getGeneralOption().advancedParseEnabled();

        auto loadRes = opt.load(advp);
        if(!loadRes) return tl::unexpected(loadRes.error());
        
        auto nRecommended = opt.getValue("api_search_recommended");
        if(!nRecommended) return tl::unexpected(nRecommended.error());
        if(nRecommended.value() == nullptr){
            bRecommended = false;
        }else{
            if(!nRecommended.value().is_boolean()){
                mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_WRONG_TYPE, {"\"api_search_recommended\"", "boolean"});
                return tl::unexpected(err);
            }
            bRecommended = nRecommended.value();
        }
    }else{
        auto* property = mcsm::GeneralOption::getGeneralOption().getProperty("sponge_api_search_recommended_versions");
        if(property == nullptr){
            mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_NOT_FOUND, {"\"sponge_api_search_recommended_versions\"", "general config"});
            return tl::unexpected(err);
        }

        const nlohmann::json& v = property->getCurrentValue();
        if(!v.is_boolean()){
            mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_WRONG_TYPE, {"\"sponge_api_search_recommended_versions\"", "boolean"});
            return tl::unexpected(err);
        }

        bRecommended = property->getCurrentValue().get<bool>();
    }

    std::string recommended = bRecommended ? "true" : "false";

    auto res = mcsm::get("https://dl-api.spongepowered.org/v2/groups/org.spongepowered/artifacts/spongevanilla/versions?tags=,minecraft:" + ver + "&limit=1&recommended=" + recommended);
    if(!res) return res;

    nlohmann::json json = nlohmann::json::parse(res.value(), nullptr, false);
    if(json.is_discarded()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::GET_REQUEST_FAILED, {"https://dl-api.spongepowered.org/v2/groups/org.spongepowered/artifacts/spongevanilla/versions?tags=,minecraft:" + ver + "&limit=1&recommended=" + recommended, "Invalid API json responce"});
        return tl::unexpected(err);
    }

    nlohmann::json artifacts = json["artifacts"];
    if(artifacts == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::GET_REQUEST_FAILED, {"https://dl-api.spongepowered.org/v2/groups/org.spongepowered/artifacts/spongevanilla/versions?tags=,minecraft:" + ver + "&limit=1&recommended=" + recommended, "Invalid API json responce on property \"artifacts\""});
        return tl::unexpected(err);
    }
    if(!artifacts.is_object()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::GET_REQUEST_FAILED, {"https://dl-api.spongepowered.org/v2/groups/org.spongepowered/artifacts/spongevanilla/versions?tags=,minecraft:" + ver + "&limit=1&recommended=" + recommended, "Invalid API json responce on property \"artifacts\""});
        return tl::unexpected(err);
    }

    std::vector<std::string> keys;
    for(nlohmann::json::iterator it = artifacts.begin(); it != artifacts.end(); it++){
        keys.push_back(it.key());
    }

    if(keys.empty()) return "";
    return keys[0];
}

mcsm::StringResult mcsm::SpongeServer::getVersion(const std::string& ver, const bool& apiSearchRecommended) const {
    if(!mcsm::isSafeString(ver)){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::UNSAFE_STRING, {ver});
        return tl::unexpected(err);
    }
    auto cPath = mcsm::getCurrentPath();
    if(!cPath) return cPath;

    mcsm::Option opt(cPath.value(), "server");
    
    auto optExists = opt.exists();
    if(!optExists) return tl::unexpected(optExists.error());

    if(optExists.value()){
        bool advp = mcsm::GeneralOption::getGeneralOption().advancedParseEnabled();

        auto loadRes = opt.load(advp);
        if(!loadRes) return tl::unexpected(loadRes.error());
    }else{
        auto* property = mcsm::GeneralOption::getGeneralOption().getProperty("sponge_api_search_recommended_versions");
        if(property == nullptr){
            mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_NOT_FOUND, {"\"sponge_api_search_recommended_versions\"", "general config"});
            return tl::unexpected(err);
        }
    }

    std::string recommended = apiSearchRecommended ? "true" : "false";

    auto res = mcsm::get("https://dl-api.spongepowered.org/v2/groups/org.spongepowered/artifacts/spongevanilla/versions?tags=,minecraft:" + ver + "&limit=1&recommended=" + recommended);
    if(!res) return res;

    nlohmann::json json = nlohmann::json::parse(res.value(), nullptr, false);
    if(json.is_discarded()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::GET_REQUEST_FAILED, {"https://dl-api.spongepowered.org/v2/groups/org.spongepowered/artifacts/spongevanilla/versions?tags=,minecraft:" + ver + "&limit=1&recommended=" + recommended, "Invalid API json responce"});
        return tl::unexpected(err);
    }

    nlohmann::json artifacts = json["artifacts"];
    if(artifacts == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::GET_REQUEST_FAILED, {"https://dl-api.spongepowered.org/v2/groups/org.spongepowered/artifacts/spongevanilla/versions?tags=,minecraft:" + ver + "&limit=1&recommended=" + recommended, "Invalid API json responce on property \"artifacts\""});
        return tl::unexpected(err);
    }
    if(!artifacts.is_object()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::GET_REQUEST_FAILED, {"https://dl-api.spongepowered.org/v2/groups/org.spongepowered/artifacts/spongevanilla/versions?tags=,minecraft:" + ver + "&limit=1&recommended=" + recommended, "Invalid API json responce on property \"artifacts\""});
        return tl::unexpected(err);
    }

    std::vector<std::string> keys;
    for(nlohmann::json::iterator it = artifacts.begin(); it != artifacts.end(); it++){
        keys.push_back(it.key());
    }

    if(keys.empty()) return "";
    return keys[0];
}

// used for checking if versions with specific build exists
// @param build number of build

// Note: The actual list position and release numbers don't match.
// The offset calculation is based on the list position, not the release number.
mcsm::StringResult mcsm::SpongeServer::getVersion(const std::string& ver, const std::string& build, const bool& /* apiSearchRecommended */) const {
    if(!mcsm::isSafeString(ver)){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::UNSAFE_STRING, {ver});
        return tl::unexpected(err);
    }
    if(!mcsm::isSafeString(build)){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::UNSAFE_STRING, {build});
        return tl::unexpected(err);
    }
    if(!mcsm::is_number(build)) return "";

    int buildNo = std::stoi(build); 

    auto cPathRes = mcsm::getCurrentPath();
    if(!cPathRes) return cPathRes;
    auto cPath = cPathRes.value();

    mcsm::Option opt(cPath, "server");

    bool advp = mcsm::GeneralOption::getGeneralOption().advancedParseEnabled();

    auto opLoadRes = opt.load(advp);
    if(!opLoadRes) return tl::unexpected(opLoadRes.error());

    auto nRecommended = opt.getValue("api_search_recommended");
    if(!nRecommended) return tl::unexpected(nRecommended.error());
    if(nRecommended.value() == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_NOT_FOUND, {"\"api_search_recommended\"", opt.getName()});
        return tl::unexpected(err);
    }
    if(!nRecommended.value().is_boolean()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_WRONG_TYPE, {"\"api_search_recommended\"", "boolean"});
        return tl::unexpected(err);
    }

    bool bRecommended = nRecommended.value();

    std::string recommended = bRecommended ? "true" : "false";

    auto sizeRes = mcsm::get("https://dl-api.spongepowered.org/v2/groups/org.spongepowered/artifacts/spongevanilla/versions?tags=,minecraft:" + ver + "&limit=1&recommended=" + recommended);
    if(!sizeRes) return sizeRes;

    nlohmann::json json1 = nlohmann::json::parse(sizeRes.value(), nullptr, false);
    if(json1.is_discarded()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::GET_REQUEST_FAILED, {"https://dl-api.spongepowered.org/v2/groups/org.spongepowered/artifacts/spongevanilla/versions?tags=,minecraft:" + ver + "&limit=1&recommended=" + recommended, "Invalid API json responce"});
        return tl::unexpected(err);
    }

    nlohmann::json jSize = json1["size"];
    if(jSize == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::GET_REQUEST_FAILED, {"https://dl-api.spongepowered.org/v2/groups/org.spongepowered/artifacts/spongevanilla/versions?tags=,minecraft:" + ver + "&limit=1&recommended=" + recommended, "Invalid API json responce on property \"size\""});
        return tl::unexpected(err);
    }
    if(!jSize.is_number_integer()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::GET_REQUEST_FAILED, {"https://dl-api.spongepowered.org/v2/groups/org.spongepowered/artifacts/spongevanilla/versions?tags=,minecraft:" + ver + "&limit=1&recommended=" + recommended, "Invalid API json responce on property \"size\""});
        return tl::unexpected(err);
    }

    int totalSize = jSize;

    // 1 means limit in the request
    // if you send the request like &limit=25, 1 should be replaced with 25
    int offset = std::max(0, (totalSize - 1) - std::max(0, buildNo - 1));

    auto res = mcsm::get("https://dl-api.spongepowered.org/v2/groups/org.spongepowered/artifacts/spongevanilla/versions?tags=,minecraft:" + ver + "&limit=1&offset=" + std::to_string(offset) + "&recommended=" + recommended);
    if(!res) return res;

    nlohmann::json json = nlohmann::json::parse(res.value(), nullptr, false);
    if(json.is_discarded()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::GET_REQUEST_FAILED, {"https://dl-api.spongepowered.org/v2/groups/org.spongepowered/artifacts/spongevanilla/versions?tags=,minecraft:" + ver + "&limit=1&offset=" + std::to_string(offset) + "&recommended=" + recommended, "Invalid API json responce"});
        return tl::unexpected(err);
    }

    nlohmann::json artifacts = json["artifacts"];
    if(artifacts == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::GET_REQUEST_FAILED, {"https://dl-api.spongepowered.org/v2/groups/org.spongepowered/artifacts/spongevanilla/versions?tags=,minecraft:" + ver + "&limit=1&offset=" + std::to_string(offset) + "&recommended=" + recommended, "Invalid API json responce on property \"artifacts\""});
        return tl::unexpected(err);
    }
    if(!artifacts.is_object()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::GET_REQUEST_FAILED, {"https://dl-api.spongepowered.org/v2/groups/org.spongepowered/artifacts/spongevanilla/versions?tags=,minecraft:" + ver + "&limit=1&offset=" + std::to_string(offset) + "&recommended=" + recommended, "Invalid API json responce on property \"artifacts\""});
        return tl::unexpected(err);
    }

    std::vector<std::string> keys;
    for(nlohmann::json::iterator it = artifacts.begin(); it != artifacts.end(); it++){
        keys.push_back(it.key());
    }

    if(keys.empty()) return "";
    return keys[0];
}

std::vector<std::string> mcsm::SpongeServer::getAvailableVersions(){
    std::vector<std::string> versions;
    for(const std::string& s : mcsm::getMinecraftVersions()){
        versions.push_back(s);
    }
    return versions;
}

std::string mcsm::SpongeServer::getSupportedVersions() const {
    return "1.8~";
}

std::string mcsm::SpongeServer::getBasedServer() const {
    return "vanilla";
}

std::string mcsm::SpongeServer::getWebSite() const {
    return "https://spongepowered.org/";
}

std::string mcsm::SpongeServer::getGitHub() const {
    return "https://github.com/SpongePowered/Sponge";
}

mcsm::VoidResult mcsm::SpongeServer::download(const std::string& version){
    auto path1 = mcsm::getCurrentPath();
    if(!path1) return tl::unexpected(path1.error());

    auto jar = getJarFile();
    if(!jar) return tl::unexpected(jar.error());

    return download(version, path1.value(), jar.value(), path1.value());
}

mcsm::VoidResult mcsm::SpongeServer::download(const std::string& version, const std::string& path){
    auto path1 = mcsm::getCurrentPath();
    if(!path1) return tl::unexpected(path1.error());

    auto jar = getJarFile();
    if(!jar) return tl::unexpected(jar.error());

    return download(version, path, jar.value(), path1.value());
}

mcsm::VoidResult mcsm::SpongeServer::download(const std::string& version, const std::string& path, const std::string& name){
    auto path1 = mcsm::getCurrentPath();
    if(!path1) return tl::unexpected(path1.error());

    return download(version, path, name, path1.value());
}

mcsm::VoidResult mcsm::SpongeServer::download(const std::string& version, const std::string& path, const std::string& name, const std::string& optionPath){
    mcsm::Option opt(optionPath, "server");
    auto optExists = opt.exists();
    if(!optExists) return tl::unexpected(optExists.error());
    if(!optExists.value()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::SERVER_NOT_CONFIGURED, {});
        return tl::unexpected(err);
    }

    auto lRes = opt.load(mcsm::GeneralOption::getGeneralOption().advancedParseEnabled());
    if(!lRes) return lRes;

    mcsm::ServerDataOption sDataOpt(optionPath);

    auto tVGRes = opt.getValue("type");
    if(!tVGRes) return tl::unexpected(tVGRes.error());

    nlohmann::json typeValue = tVGRes.value();
    if(typeValue == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_NOT_FOUND_PLUS_FIX, {"\"type\"", opt.getName(), "change it into \"type\": \"[yourtype]\""});
        return tl::unexpected(err);
    }
    if(!typeValue.is_string()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_WRONG_TYPE_PLUS_FIX, {"\"type\"", opt.getName(), "string", "change it into \"type\": \"[yourtype]\""});
        return tl::unexpected(err);
    }
    if(typeValue != "sponge"){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::SERVER_WRONG_INSTANCE_GENERATED, {"Sponge"});
        return tl::unexpected(err);
    }

    auto sBVGRes = opt.getValue("server_build");
    if(!sBVGRes) return tl::unexpected(sBVGRes.error());

    nlohmann::json serverBuildValue = sBVGRes.value();
    
    if(serverBuildValue == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_NOT_FOUND_PLUS_FIX, {"\"server_build\"", opt.getName(), "add \"server_build\": \"latest\" to server.json for automatic download"});
        return tl::unexpected(err);
    }
    if(!serverBuildValue.is_string()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_WRONG_TYPE_PLUS_FIX, {"\"server_build\"", opt.getName(), "string", "change it into \"server_build\": \"latest\""});
        return tl::unexpected(err);
    }

    std::string url, build;
    if(serverBuildValue != "latest"){
        mcsm::warning("Sponge release version does not match the actual order of the file names. The program will NOT use the \"server_build\" value to match the release name.");
        mcsm::warning("Example: SpongeVanilla 1.12.2 build #2 is named '1.12.2-7.0.0-BETA-330', while the first build is named '1.12.2-7.0.0-BETA-1'.");
        auto bR = getVersion(version, serverBuildValue, false);
        if(!bR) return tl::unexpected(bR.error());
        build = bR.value();
        if(mcsm::isWhitespaceOrEmpty(build)){
            mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::SERVER_UNSUPPORTED_VERSION, {});
            return tl::unexpected(err);
        }

        auto gDLGRes = getDownloadLink(build);
        if(!gDLGRes) return tl::unexpected(gDLGRes.error());
        url = gDLGRes.value();
        if(mcsm::isWhitespaceOrEmpty(url)){
            auto customTemp = mcsm::errors::INTERNAL_FUNC_EXECUTION_FAILED;
            customTemp.message = "Failed to get sponge download url. It's likely a bug or the API has changed; open an issue at GitHub.";
            mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, customTemp, {});
            return tl::unexpected(err);
        }
    }else{
        auto bR = getVersion(version);
        if(!bR) return tl::unexpected(bR.error());
        build = bR.value();
        if(mcsm::isWhitespaceOrEmpty(build)){
            mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::SERVER_UNSUPPORTED_VERSION, {});
            return tl::unexpected(err);
        }

        auto gDLGRes = getDownloadLink(build);
        if(!gDLGRes) return tl::unexpected(gDLGRes.error());
        url = gDLGRes.value();
        if(mcsm::isWhitespaceOrEmpty(url)){
            auto customTemp = mcsm::errors::INTERNAL_FUNC_EXECUTION_FAILED;
            customTemp.message = "Failed to get sponge download url. It's likely a bug or the API has changed; open an issue at GitHub.";
            mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, customTemp, {});
            return tl::unexpected(err);
        }
    }
    
    mcsm::info("URL : " + url);
    mcsm::VoidResult res = mcsm::download(name, url, path, true);
    if(!res) return res;
    return sDataOpt.updateLastDownloadedBuild(build);
}

mcsm::VoidResult mcsm::SpongeServer::obtainJarFile(const std::string& version, const std::string& path, const std::string& name, const std::string& optionPath){
    return download(version, path, name, optionPath);
}

mcsm::StringResult mcsm::SpongeServer::getDownloadLink(const std::string& build) const {
    auto res = mcsm::get("https://dl-api.spongepowered.org/v2/groups/org.spongepowered/artifacts/spongevanilla/versions/" + build);
    if(!res) return res;
    nlohmann::json json = nlohmann::json::parse(res.value(), nullptr, false);
    if(json.is_discarded()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_NOT_FOUND, {});
        return tl::unexpected(err);
    }

    nlohmann::json assets = json["assets"];
    if(assets == nullptr) return "";
    if(!assets.is_array()) return "";

    for(nlohmann::json& obj : assets){
        if(!obj.is_object()) return "";
        nlohmann::json classifier = obj["classifier"];
        if(classifier == nullptr || !classifier.is_string()) return "";
        nlohmann::json downloadUrl = obj["downloadUrl"];
        if(downloadUrl == nullptr || !downloadUrl.is_string()) return "";

        if((classifier == "universal" || classifier == "") && mcsm::endsWith(downloadUrl, ".jar")) return downloadUrl;
    }
    return "";
}

mcsm::StringResult mcsm::SpongeServer::start(mcsm::ServerConfigLoader* loader, mcsm::JvmOption& option){
    // ServerOption class handles the data file stuff
    mcsm::StringResult cPath = mcsm::getCurrentPath();
    if(!cPath) return cPath;
    return start(loader, option, cPath.value(), cPath.value());
}

mcsm::StringResult mcsm::SpongeServer::start(mcsm::ServerConfigLoader* loader, mcsm::JvmOption& option, const std::string& path, const std::string& optionPath){
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

mcsm::VoidResult mcsm::SpongeServer::update(){
    auto path1 = mcsm::getCurrentPath();
    if(!path1) return tl::unexpected(path1.error());

    return update(path1.value(), path1.value());
}

mcsm::VoidResult mcsm::SpongeServer::update(const std::string& optionPath){
    auto path1 = mcsm::getCurrentPath();
    if(!path1) return tl::unexpected(path1.error());

    return update(path1.value(), optionPath);
}

mcsm::VoidResult mcsm::SpongeServer::update(const std::string& path, const std::string& optionPath){
    // Program won't downgrade server jarfiles automatically. This is an intented feature.
    mcsm::info("Checking updates...");
    mcsm::ServerDataOption sDataOpt(optionPath);

    mcsm::VoidResult sLoadRes = sDataOpt.load();
    if(!sLoadRes) return sLoadRes;

    mcsm::ServerConfigLoader loader(optionPath);
    mcsm::VoidResult loadRes = loader.loadConfig();
    if(!loadRes) return loadRes;

    auto build = loader.getServerJarBuild();
    if(!build) return tl::unexpected(build.error());

    if(build.value() != "latest"){
        mcsm::warning("This server won't update to the latest build.");
        mcsm::warning("Change server.json into \"server_build\": \"latest\" for automatic download.");
        return {};
    }

    auto version = loader.getServerVersion();
    if(!version) return tl::unexpected(version.error());

    auto ver = getVersion(version.value());
    if(!ver) return tl::unexpected(ver.error());

    if(ver.value() == ""){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::SERVER_UNSUPPORTED_VERSION, {});
        return tl::unexpected(err);
    }
    auto lastBuild = sDataOpt.getLastDownloadedBuild();
    if(!lastBuild) return tl::unexpected(lastBuild.error());

    if(lastBuild.value() == ver.value()){
        mcsm::success("Server is up to date.");
        return {};
    }
    mcsm::success("Update found : "  + ver.value() + ". Current build : " + lastBuild.value());

    auto jar = getJarFile(optionPath);
    if(!jar) return tl::unexpected(jar.error());

    auto fileExists = mcsm::fileExists(path + "/" + jar.value());
    if(!fileExists) return tl::unexpected(fileExists.error());

    if(fileExists.value()){
        auto rmRes = mcsm::removeFile(path + "/" + jar.value());
        if(!rmRes) return tl::unexpected(rmRes.error());
    }
    return download(version.value(), path, jar.value(), optionPath);
}

mcsm::VoidResult mcsm::SpongeServer::generate(const std::string& name, mcsm::JvmOption& option, const std::string& path, const std::string& version, const bool& autoUpdate, const std::map<std::string, std::string>& extraValues){
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

    std::string apib = extraValues.find("sponge_api_search_recommended_versions")->second;
    if(apib != "false" && apib != "true"){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, {700, "Invalid parameter value on \"sponge_api_search_recommended_versions\"", ""});
        return tl::unexpected(err);
    }

    bool bApi = apib == "true" ? true : false;

    if(!skipCheck){
        mcsm::StringResult vRes = getVersion(version, bApi);
        if(!vRes) return tl::unexpected(vRes.error());
        bool vExists = vRes.value() != "";
        if(!vExists){
            mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::SERVER_UNSUPPORTED_VERSION, {});
            return tl::unexpected(err);
        }
    }
    mcsm::ServerDataOption opt(path);

    // No need to call opt.load() here. create() in ServerDataOption will call it eventually

    mcsm::VoidResult res = configure(version, this, &opt, path, name, option, autoUpdate, extraValues.find("server_build_version")->second);
    if(!res) return res;

    auto cPath = mcsm::getCurrentPath();
    if(!cPath) return tl::unexpected(cPath.error());

    mcsm::Option sOpt(cPath.value(), "server");

    auto sOptLoadRes = sOpt.load();
    if(!sOptLoadRes) return sOptLoadRes;

    auto setRes = sOpt.setValue("api_search_recommended", bApi);
    if(!setRes) return setRes;

    return sOpt.save();
}

const tl::expected<std::map<std::string, std::string>, mcsm::Error> mcsm::SpongeServer::getRequiredValues() const {
    auto* property1 = mcsm::GeneralOption::getGeneralOption().getProperty("sponge_api_search_recommended_versions");
    if(property1 == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_NOT_FOUND, {"\"sponge_api_search_recommended_versions\"", "general config"});
        return tl::unexpected(err);
    }

    const nlohmann::json& v = property1->getCurrentValue();
    if(!v.is_boolean()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_WRONG_TYPE, {"\"sponge_api_search_recommended_versions\"", "boolean"});
        return tl::unexpected(err);
    }

    std::string strV = v.get<bool>() ? "true" : "false";
    return tl::expected<std::map<std::string, std::string>, mcsm::Error>{
        std::map<std::string, std::string>{
                {"name", "" },
                {"minecraft_version", ""},
                {"default_jvm_launch_profile_search_path", "current"},
                {"default_jvm_launch_profile_name", ""},
                {"server_jarfile_name", getTypeAsString() + ".jar"},
                {"server_build_version", "latest"},
                {"auto_server_jar_update", "true"},
                {"sponge_api_search_recommended_versions", strV}
        }
    };
}

mcsm::BoolResult mcsm::SpongeServer::hasVersion(const std::string& version) const {
    auto verR = getVersion(version);
    if(!verR) return tl::unexpected(verR.error());
    return verR.value() != "";
}

mcsm::ServerType mcsm::SpongeServer::getType() const {
    return mcsm::ServerType::SPONGE_VANILLA;
}

std::string mcsm::SpongeServer::getTypeAsString() const {
    return "sponge";
}