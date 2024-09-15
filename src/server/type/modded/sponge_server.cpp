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
std::string mcsm::SpongeServer::getVersion(const std::string& ver) const {
    mcsm::Option opt(mcsm::getCurrentPath(), "server");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";
    
    bool bRecommended = false;

    nlohmann::json nRecommended = opt.getValue("api_serch_recommended");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";
    if(nRecommended == nullptr){
        bRecommended = false;
    }else{
        if(!nRecommended.is_boolean()){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"api_serch_recommended\"", "boolean")});
            return "";
        }
        bRecommended = nRecommended;
    }

    std::string recommended = bRecommended ? "true" : "false";

    std::string res = mcsm::get("https://dl-api.spongepowered.org/v2/groups/org.spongepowered/artifacts/spongevanilla/versions?tags=,minecraft:" + ver + "&limit=1&recommended=" + recommended);
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";
    nlohmann::json json = nlohmann::json::parse(res, nullptr, false);
    if(json.is_discarded()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonParseFailedCannotBeModified()});
        return "";
    }

    nlohmann::json artifacts = json["artifacts"];
    if(artifacts == nullptr) return ""; // has to exist
    if(!artifacts.is_object()) return ""; // has to be an object

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
std::string mcsm::SpongeServer::getVersion(const std::string& ver, const std::string& build) const {
    if(!mcsm::is_number(build)) return "";

    int buildNo = std::stoi(build); 

    mcsm::Option opt(mcsm::getCurrentPath(), "server");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";
    
    nlohmann::json nRecommended = opt.getValue("api_serch_recommended");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";
    if(nRecommended == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFound("\"api_serch_recommended\"", opt.getName())});
        return "";
    }
    if(!nRecommended.is_boolean()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"api_serch_recommended\"", "boolean")});
        return "";
    }

    bool bRecommended = nRecommended;

    std::string recommended = bRecommended ? "true" : "false";

    std::string sizeRes = mcsm::get("https://dl-api.spongepowered.org/v2/groups/org.spongepowered/artifacts/spongevanilla/versions?tags=,minecraft:" + ver + "&limit=1&recommended=" + recommended);
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";
    nlohmann::json json1 = nlohmann::json::parse(sizeRes, nullptr, false);
    if(json1.is_discarded()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonParseFailedCannotBeModified()});
        return "";
    }

    nlohmann::json jSize = json1["size"];
    if(jSize == nullptr) return ""; // has to exist
    if(!jSize.is_number_integer()) return "";

    int totalSize = jSize;

    // 1 means limit in the request
    // if you send the request like &limit=25, 1 should be replaced with 25
    int offset = std::max(0, (totalSize - 1) - std::max(0, buildNo - 1));

    std::string res = mcsm::get("https://dl-api.spongepowered.org/v2/groups/org.spongepowered/artifacts/spongevanilla/versions?tags=,minecraft:" + ver + "&limit=1&offset=" + std::to_string(offset) + "&recommended=" + recommended);
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";
    
    nlohmann::json json = nlohmann::json::parse(res, nullptr, false);
    if(json.is_discarded()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonParseFailedCannotBeModified()});
        return "";
    }

    nlohmann::json artifacts = json["artifacts"];
    if(artifacts == nullptr) return ""; // has to exist
    if(!artifacts.is_object()) return ""; // has to be an object

    std::vector<std::string> keys;
    for(nlohmann::json::iterator it = artifacts.begin(); it != artifacts.end(); it++){
        keys.push_back(it.key());
    }

    if(keys.empty()) return "";
    return keys[0];
}

std::string mcsm::SpongeServer::getLatestVersion() const {
    std::string res = mcsm::get("https://api.papermc.io/v2/projects/velocity");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";
    nlohmann::json json = nlohmann::json::parse(res, nullptr, false);
    if(json.is_discarded()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonParseFailedCannotBeModified()});
        return "";
    }

    if(json["versions"] == nullptr || !json["versions"].is_array()){
        return "";
    }else{
        nlohmann::json builds = json["versions"];
        if(builds[json["versions"].size() - 1] == nullptr || !builds[json["versions"].size() - 1].is_string()) return "";
        mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
        return builds[json["versions"].size() - 1];
    }
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

mcsm::Result mcsm::SpongeServer::download(const std::string& version){
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

mcsm::Result mcsm::SpongeServer::download(const std::string& version, const std::string& path){
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

mcsm::Result mcsm::SpongeServer::download(const std::string& version, const std::string& path, const std::string& name){
    std::string path1 = mcsm::getCurrentPath();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    return download(version, path, name, path1);
}

mcsm::Result mcsm::SpongeServer::download(const std::string& version, const std::string& path, const std::string& name, const std::string& optionPath){
    mcsm::Option opt(optionPath, "server");
    bool optExists = opt.exists();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    if(!optExists){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverNotConfigured()});
        return res;
    }

    mcsm::ServerDataOption sDataOpt(optionPath);
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    nlohmann::json typeValue = opt.getValue("type");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    if(typeValue == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFoundPlusFix("\"type\"", opt.getName(), "change it into \"type\": \"[yourtype]\"")});
        return res;
    }
    if(!typeValue.is_string()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongTypePlusFix("\"type\"", opt.getName(), "string", "change it into \"type\": \"[yourtype]\"")});
        return res;
    }
    if(typeValue != "sponge"){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverWrongInstanceGenerated("Sponge")});
        return res;
    }

    nlohmann::json serverBuildValue = opt.getValue("server_build");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }
    
    if(serverBuildValue == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFoundPlusFix("\"server_build\"", opt.getName(), "add \"server_build\": \"latest\" to server.json for automatic download")});
        return res;
    }
    if(!serverBuildValue.is_string()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongTypePlusFix("\"server_build\"", opt.getName(), "string", "change it into \"server_build\": \"latest\"")});
        return res;
    }

    std::string url, build;
    if(serverBuildValue != "latest"){
        mcsm::warning("Sponge release version does not match the actual order of the file names. The program will NOT use the \"server_build\" value to match the release name.");
        mcsm::warning("Example: SpongeVanilla 1.12.2 build #2 is named '1.12.2-7.0.0-BETA-330', while the first build is named '1.12.2-7.0.0-BETA-1'.");
        build = getVersion(version, serverBuildValue);
        if(mcsm::isWhitespaceOrEmpty(build)){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverUnsupportedVersion()});
            return res;
        }

        url = getDownloadLink(build);
        if(mcsm::isWhitespaceOrEmpty(url)){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
                "Failed to get download url.",
                "It's likely a bug or the API has changed; open an issue at GitHub."
            }});
            return res;
        }
    }else{
        build = getVersion(version);
        if(mcsm::isWhitespaceOrEmpty(build)){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverUnsupportedVersion()});
            return res;
        }

        url = getDownloadLink(build);
        if(mcsm::isWhitespaceOrEmpty(url)){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
                "Failed to get download url.",
                "It's likely a bug or the API has changed; open an issue at GitHub."
            }});
            return res;
        }
    }
    
    mcsm::info("URL : " + url);
    mcsm::Result res = mcsm::download(name, url, path, true);
    if(!res.isSuccess()) return res;
    sDataOpt.updateLastDownloadedBuild(build);
    return res;
}

mcsm::Result mcsm::SpongeServer::obtainJarFile(const std::string& version, const std::string& path, const std::string& name, const std::string& optionPath){
    return download(version, path, name, optionPath);
}

std::string mcsm::SpongeServer::getDownloadLink(const std::string& build) const {
    std::string res = mcsm::get("https://dl-api.spongepowered.org/v2/groups/org.spongepowered/artifacts/spongevanilla/versions/" + build);
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";
    nlohmann::json json = nlohmann::json::parse(res, nullptr, false);
    if(json.is_discarded()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonParseFailedCannotBeModified()});
        return "";
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

mcsm::Result mcsm::SpongeServer::start(mcsm::JvmOption& option){
    // ServerOption class handles the data file stuff
    std::string cPath = mcsm::getCurrentPath();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    return start(option, cPath, cPath);
}

mcsm::Result mcsm::SpongeServer::start(mcsm::JvmOption& option, const std::string& path, const std::string& optionPath){
    // ServerOption class handles the data file stuff
    mcsm::ServerOption sOpt(optionPath);
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
            mcsm::Result res = update(path, optionPath);
            if(!res.isSuccess()) return res;
        }
    }
    return Server::start(option, path, optionPath);
}

mcsm::Result mcsm::SpongeServer::update(){
    std::string path = mcsm::getCurrentPath();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    return update(path, path);
}

mcsm::Result mcsm::SpongeServer::update(const std::string& optionPath){
    std::string path = mcsm::getCurrentPath();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    return update(path, optionPath);
}

mcsm::Result mcsm::SpongeServer::update(const std::string& path, const std::string& optionPath){
    // Program won't downgrade server jarfiles automatically. This is an intented feature.
    mcsm::info("Checking updates...");
    mcsm::ServerDataOption sDataOpt(optionPath);
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    mcsm::Option opt(optionPath, "server");
    nlohmann::json sBuild = opt.getValue("server_build");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    if(sBuild == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFound("\"server_build\"", opt.getName())});
        return res;
    }
    if(!sBuild.is_string()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongTypePlusFix("\"server_build\"", opt.getName(), "string", "change it into \"server_build\": \"latest\"")});
        return res;
    }
    std::string build = sBuild.get<std::string>();
    if(build != "latest"){
        mcsm::warning("This server won't update to the latest build.");
        mcsm::warning("Change server.json into \"server_build\": \"latest\" for automatic download.");
        mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {
            "Update passed"
        }});
        return res;
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
    
    std::string version = sVer.get<std::string>();
    std::string ver = getVersion(version);
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    if(ver == ""){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverUnsupportedVersion()});
        return res;
    }
    std::string lastBuild = sDataOpt.getLastDownloadedBuild();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    if(lastBuild == ver){
        mcsm::success("Server is up to date.");
        mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
        return res;
    }
    mcsm::success("Update found : "  + ver + ". Current build : " + lastBuild);

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
    return download(version, path, jar, optionPath);
}

mcsm::Result mcsm::SpongeServer::generate(const std::string& name, mcsm::JvmOption& option, const std::string& version, const bool& autoUpdate){
    std::shared_ptr<mcsm::SpongeServer> server = shared_from_this();
    mcsm::ServerOption serverOption(version, server);
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    mcsm::Result res = configure(serverOption, name, option, autoUpdate);
    return res;
}

bool mcsm::SpongeServer::hasVersion(const std::string& version){
    return getVersion(version) != "";
}

mcsm::ServerType mcsm::SpongeServer::getType() const {
    return mcsm::ServerType::SPONGE_VANILLA;
}

std::string mcsm::SpongeServer::getTypeAsString() const {
    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
    return "sponge";
}