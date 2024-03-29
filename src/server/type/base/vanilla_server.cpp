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

mcsm::Result mcsm::VanillaServer::init(){
    std::unique_ptr<std::map<const std::string, const std::string>>& map = this->versions;
    map->insert({"1.20.4", "https://piston-data.mojang.com/v1/objects/8dd1a28015f51b1803213892b50b7b4fc76e594d/server.jar"});
    map->insert({"1.20.3", "https://piston-data.mojang.com/v1/objects/4fb536bfd4a83d61cdbaf684b8d311e66e7d4c49/server.jar"});
    map->insert({"1.20.2", "https://piston-data.mojang.com/v1/objects/5b868151bd02b41319f54c8d4061b8cae84e665c/server.jar"});
    map->insert({"1.20.1", "https://piston-data.mojang.com/v1/objects/84194a2f286ef7c14ed7ce0090dba59902951553/server.jar"});
    map->insert({"1.20", "https://piston-data.mojang.com/v1/objects/15c777e2cfe0556eef19aab534b186c0c6f277e1/server.jar"});
    map->insert({"1.19.4", "https://piston-data.mojang.com/v1/objects/8f3112a1049751cc472ec13e397eade5336ca7ae/server.jar"});
    map->insert({"1.19.3", "https://piston-data.mojang.com/v1/objects/c9df48efed58511cdd0213c56b9013a7b5c9ac1f/server.jar"});
    map->insert({"1.19.2", "https://piston-data.mojang.com/v1/objects/f69c284232d7c7580bd89a5a4931c3581eae1378/server.jar"});
    map->insert({"1.19.1", "https://piston-data.mojang.com/v1/objects/8399e1211e95faa421c1507b322dbeae86d604df/server.jar"});
    map->insert({"1.19", "https://piston-data.mojang.com/v1/objects/e00c4052dac1d59a1188b2aa9d5a87113aaf1122/server.jar"});
    map->insert({"1.18.2", "https://piston-data.mojang.com/v1/objects/c8f83c5655308435b3dcf03c06d9fe8740a77469/server.jar"});
    map->insert({"1.18.1", "https://piston-data.mojang.com/v1/objects/125e5adf40c659fd3bce3e66e67a16bb49ecc1b9/server.jar"});
    map->insert({"1.18", "https://piston-data.mojang.com/v1/objects/3cf24a8694aca6267883b17d934efacc5e44440d/server.jar"});
    map->insert({"1.17.1", "https://piston-data.mojang.com/v1/objects/a16d67e5807f57fc4e550299cf20226194497dc2/server.jar"});
    map->insert({"1.17", "https://piston-data.mojang.com/v1/objects/0a269b5f2c5b93b1712d0f5dc43b6182b9ab254e/server.jar"});
    map->insert({"1.16.5", "https://piston-data.mojang.com/v1/objects/1b557e7b033b583cd9f66746b7a9ab1ec1673ced/server.jar"});
    map->insert({"1.16.4", "https://piston-data.mojang.com/v1/objects/35139deedbd5182953cf1caa23835da59ca3d7cd/server.jar"});
    map->insert({"1.16.3", "https://piston-data.mojang.com/v1/objects/f02f4473dbf152c23d7d484952121db0b36698cb/server.jar"});
    map->insert({"1.16.2", "https://piston-data.mojang.com/v1/objects/c5f6fb23c3876461d46ec380421e42b289789530/server.jar"});
    map->insert({"1.16.1", "https://piston-data.mojang.com/v1/objects/a412fd69db1f81db3f511c1463fd304675244077/server.jar"});
    map->insert({"1.16", "https://piston-data.mojang.com/v1/objects/a0d03225615ba897619220e256a266cb33a44b6b/server.jar"});
    map->insert({"1.15.2", "https://piston-data.mojang.com/v1/objects/bb2b6b1aefcd70dfd1892149ac3a215f6c636b07/server.jar"});
    map->insert({"1.15.1", "https://piston-data.mojang.com/v1/objects/4d1826eebac84847c71a77f9349cc22afd0cf0a1/server.jar"});
    map->insert({"1.15", "https://piston-data.mojang.com/v1/objects/e9f105b3c5c7e85c7b445249a93362a22f62442d/server.jar"});
    map->insert({"1.14.4", "https://piston-data.mojang.com/v1/objects/3dc3d84a581f14691199cf6831b71ed1296a9fdf/server.jar"});
    map->insert({"1.14.3", "https://piston-data.mojang.com/v1/objects/d0d0fe2b1dc6ab4c65554cb734270872b72dadd6/server.jar"});
    map->insert({"1.14.2", "https://piston-data.mojang.com/v1/objects/808be3869e2ca6b62378f9f4b33c946621620019/server.jar"});
    map->insert({"1.14.1", "https://piston-data.mojang.com/v1/objects/ed76d597a44c5266be2a7fcd77a8270f1f0bc118/server.jar"});
    map->insert({"1.14", "https://piston-data.mojang.com/v1/objects/f1a0073671057f01aa843443fef34330281333ce/server.jar"});
    mcsm::GlobalOption option("/server/url", "vanilla_server");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    bool hasValue = option.hasValue("versions");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }
    if(!hasValue){
        return option.setValue("versions", *map);
    }

    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
    return res;
}

std::string mcsm::VanillaServer::getVersionObject(const std::string& ver) const {
    std::string jsonData = mcsm::get("https://launchermeta.mojang.com/mc/game/version_manifest.json");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";
    
    nlohmann::json data = nlohmann::json::parse(jsonData);
    if(data.is_discarded()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
            "Parse of json failed. (Vanilla version manifest)",
            "Please report this to Github(https://github.com/dodoman8067/mcsm) if you believe that this is a software issue." 
        }});
        return "";  
    }

    // Check if "versions" array exists
    if(!data.contains("versions")){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
            "No \"versions\" array exists in vanilla server version manifest.",
            "Please report this to Github(https://github.com/dodoman8067/mcsm) if you believe that this is a software issue." 
        }});
        return "";
    }

    auto& versions = data["versions"];
    if(!versions.is_array()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
            "Value \"versions\" is not an array.",
            "Please report this to Github(https://github.com/dodoman8067/mcsm) if you believe that this is a software issue." 
        }});
        return "";
    }

    // Search for entry with given ID
    for(const auto& entry : versions){
        // Check if the entry is an object
        if(!entry.is_object()){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
                "Entry in vanilla server API request is not an object.",
                "Please report this to Github(https://github.com/dodoman8067/mcsm) if you believe that this is a software issue." 
            }});
            return "";
        }

        // Check if "id" field exists and matches the given ID
        if(!entry.contains("id")){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
                "Value \"id\" in vanilla server API request is not a string | not found.",
                "Please report this to Github(https://github.com/dodoman8067/mcsm) if you believe that this is a software issue." 
            }});
            return "";
        }

        if(!entry["id"].is_string()){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
                "Value \"id\" in vanilla server API request is not a string.",
                "Please report this to Github(https://github.com/dodoman8067/mcsm) if you believe that this is a software issue." 
            }});
            return "";
        }

        if(entry["id"] == ver){
            mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
            return entry.dump();
        }
    }

    // Entry not found
    return "";
}

std::string mcsm::VanillaServer::getServerJarURL(const std::string& ver) const {
    std::string versionJson = getVersionObject(ver);
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";
    if(mcsm::isWhitespaceOrEmpty(versionJson)){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverUnsupportedVersion(ver)});
        return "";
    }
    nlohmann::json version = nlohmann::json::parse(versionJson, nullptr, false);
    if(version.is_discarded()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
            "Parse of json failed. (Vanilla version object)",
            "Please report this to Github(https://github.com/dodoman8067/mcsm) if you believe that this is a software issue." 
        }});
        return "";
    }
    if(version["type"] == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
            "Value \"type\" not found. (Vanilla version object)",
            "Please report this to Github(https://github.com/dodoman8067/mcsm) if you believe that this is a software issue." 
        }});
        return "";
    }
    if(version["url"] == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
            "Value \"url\" not found. (Vanilla version object)",
            "Please report this to Github(https://github.com/dodoman8067/mcsm) if you believe that this is a software issue." 
        }});
        return "";
    }
    if(!version["type"].is_string()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
            "Value \"type\" is not string. (Vanilla version object)",
            "Please report this to Github(https://github.com/dodoman8067/mcsm) if you believe that this is a software issue." 
        }});
        return "";
    }
    if(!version["url"].is_string()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
            "Value \"url\" is not string. (Vanilla version object)",
            "Please report this to Github(https://github.com/dodoman8067/mcsm) if you believe that this is a software issue." 
        }});
        return "";
    }

    if(version["type"] == "old_beta" || version["type"] == "old_alpha"){
        mcsm::Result res({mcsm::ResultType::MCSM_WARN, {
            "No plans to support beta and alpha versions.",
            "Please report this to Github(https://github.com/dodoman8067/mcsm) if you believe that this is a software issue."
        }});
        return "";
    }

    std::string url = version["url"];
    std::string serverJson = mcsm::get(url);
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";
    if(mcsm::isWhitespaceOrEmpty(versionJson)){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverUnsupportedVersion(ver)});
        return "";
    }

    nlohmann::json serverData = nlohmann::json::parse(serverJson);
    if(serverData.is_discarded()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
            "Parse of json failed. (Vanilla version.json file).",
            "Please report this to Github(https://github.com/dodoman8067/mcsm) if you believe that this is a software issue." 
        }});
        return "";  
    }
    nlohmann::json downloadsValue = serverData["downloads"];
    if(downloadsValue == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
            "Value \"downloads\" not found. (Vanilla version.json file)",
            "Please report this to Github(https://github.com/dodoman8067/mcsm) if you believe that this is a software issue." 
        }});
        return "";
    }
    if(!downloadsValue.is_object()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
            "Value \"downloads\" not a json object. (Vanilla version.json file)",
            "Please report this to Github(https://github.com/dodoman8067/mcsm) if you believe that this is a software issue." 
        }});
        return "";
    }

    nlohmann::json serverValue = downloadsValue["server"];
    if(serverValue == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
            "Value \"server\" not found. (Vanilla version.json file)",
            "Please report this to Github(https://github.com/dodoman8067/mcsm) if you believe that this is a software issue." 
        }});
        return "";
    }
    if(!serverValue.is_object()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
            "Value \"server\" not a json object. (Vanilla version.json file)",
            "Please report this to Github(https://github.com/dodoman8067/mcsm) if you believe that this is a software issue." 
        }});
        return "";
    }

    nlohmann::json serverJarURL = serverValue["url"];
    if(serverJarURL == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
            "Value \"url\" not found. (Vanilla version.json file)",
            "Please report this to Github(https://github.com/dodoman8067/mcsm) if you believe that this is a software issue." 
        }});
        return "";
    }
    if(!serverJarURL.is_string()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
            "Value \"url\" not a string. (Vanilla version.json file)",
            "Please report this to Github(https://github.com/dodoman8067/mcsm) if you believe that this is a software issue." 
        }});
        return "";
    }

    std::string finalReturnValue = serverJarURL;
    return finalReturnValue;
}

std::vector<std::string> mcsm::VanillaServer::getAvailableVersions(){
    std::vector<std::string> vector;
    vector.reserve(this->versions->size());
    return vector;
}

mcsm::Result mcsm::VanillaServer::download(const std::string& version){
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

mcsm::Result mcsm::VanillaServer::download(const std::string& version, const std::string& path){
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

mcsm::Result mcsm::VanillaServer::download(const std::string& version, const std::string& path, const std::string& name){
    bool hasVer = hasVersion(version);
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    if(!hasVer){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverUnsupportedVersion()});
        return res;
    }
    std::string url = getServerJarURL(version);
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }
    if(mcsm::isWhitespaceOrEmpty(url)){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverUnsupportedVersion()});
        return res;
    }

    mcsm::info("URL : " + url);
    return mcsm::download(name, url, path);
}

mcsm::Result mcsm::VanillaServer::download(const std::string& version, const std::string& path, const std::string& name, const std::string& /* optionPath */){
    return download(version, path, name);
}

mcsm::Result mcsm::VanillaServer::start(mcsm::JvmOption& option){
    std::string cPath = mcsm::getCurrentPath();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    return start(option, cPath, cPath);
}

mcsm::Result mcsm::VanillaServer::start(mcsm::JvmOption& option, const std::string& path, const std::string& optionPath){
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
        std::string ver = sOpt.getServerVersion();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
            std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
            mcsm::Result res(resp.first, resp.second);
            return res;
        }

        mcsm::info("Downloading " + jar + "...");
        mcsm::Result res = download(ver, path, jar, optionPath);
        if(!res.isSuccess()) return res;
    }
    return Server::start(option, path, optionPath);
}

bool mcsm::VanillaServer::hasVersion(const std::string& version){
    std::string ver = getVersionObject(version);
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return false;
    return !mcsm::isWhitespaceOrEmpty(ver);
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