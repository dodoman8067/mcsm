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

std::string mcsm::VanillaServer::getVersionObject(const std::string& ver) const {
    if(!mcsm::isSafeString(ver)){
        return "";
    }
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

mcsm::Result mcsm::VanillaServer::obtainJarFile(const std::string& version, const std::string& path, const std::string& name, const std::string& /* optionPath */){
    return download(version, path, name);
}

mcsm::Result mcsm::VanillaServer::start(mcsm::ServerConfigLoader* loader, mcsm::JvmOption& option){
    std::string cPath = mcsm::getCurrentPath();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    return start(loader, option, cPath, cPath);
}

mcsm::Result mcsm::VanillaServer::start(mcsm::ServerConfigLoader* loader, mcsm::JvmOption& option, const std::string& path, const std::string& optionPath){
    // ServerOption class handles the data file stuff

    std::string jar = loader->getServerJarFile();
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
        std::string ver = loader->getServerVersion();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
            std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
            mcsm::Result res(resp.first, resp.second);
            return res;
        }

        mcsm::info("Downloading " + jar + "...");
        mcsm::Result res = download(ver, path, jar, optionPath);
        if(!res.isSuccess()) return res;
    }
    return Server::start(loader, option, path, optionPath);
}

mcsm::Result mcsm::VanillaServer::generate(const std::string& name, mcsm::JvmOption& option, const std::string& path, const std::string& version, const bool& autoUpdate){
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
    std::shared_ptr<mcsm::VanillaServer> server = shared_from_this();
    mcsm::ServerDataOption opt(path);
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    return configure(version, server, &opt, path, name, option, autoUpdate);
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