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

#include <mcsm/server/type/custom_server.h>

mcsm::CustomServer::CustomServer(){

}

mcsm::CustomServer::~CustomServer(){

}

mcsm::ServerType mcsm::CustomServer::getType() const {
    return mcsm::ServerType::CUSTOM;
}

std::string mcsm::CustomServer::getTypeAsString() const {
    return "custom";
}

std::string mcsm::CustomServer::getSupportedVersions() const {
    return "unknown";
}

std::string mcsm::CustomServer::getFileLocation(const std::string& optionPath) const {
    mcsm::Option option(optionPath, "server");
    bool exists = option.exists();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";
    if(!exists){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverNotConfigured()});
        return "";
    }
    if(option.getValue("jar_location") == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFound("\"jar_location\"", "server.json")});
        return "";
    }
    if(!option.getValue("jar_location").is_string()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"jar_location\"", "string")});
        return "";
    }
    std::string value = option.getValue("jar_location");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";
    if(mcsm::startsWith(value, "current") && mcsm::endsWith(value, "current")){
        return mcsm::getCurrentPath();
    }
    return value;
}

mcsm::Result mcsm::CustomServer::setFileLocation(const std::string& optionPath, const std::string& location) {
    mcsm::Option option(".", optionPath);
    return option.setValue("jar_location", location);
}

mcsm::Result mcsm::CustomServer::setupServerJarFile(const std::string& optionPath){
    std::string location = getFileLocation(optionPath);
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    bool url = isURL(location);
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    bool file = isFile(location);
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    
    if(url){
        std::string jar = getJarFile(optionPath);
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
            std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
            mcsm::Result res(resp.first, resp.second);
            return res;
        }
        std::string loc = getFileLocation(optionPath);
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
            std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
            mcsm::Result res(resp.first, resp.second);
            return res;
        }
        return mcsm::download(jar, location, loc, true);
    }else if(file){
        // TODO : Copy file
    }else{
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
            "The following server jarfile wasn't located in the vaild location : " + location,
            "Please report this to GitHub (https://github.com/dodoman8067/mcsm) if you think this is a software issue."
        }});
        return res;
    }

    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
    return res;
}

mcsm::Result mcsm::CustomServer::obtainJarFile(const std::string& /* version */, const std::string& /* path */, const std::string& /* name */, const std::string& optionPath){
    return setupServerJarFile(optionPath);
}

mcsm::Result mcsm::CustomServer::generate(const std::string& name, mcsm::JvmOption& option, const std::string& path, const std::string& version, const bool& autoUpdate){
    std::shared_ptr<mcsm::CustomServer> server = shared_from_this();
    /*
    TODO: Add mcsm::CustomServerOption, implement custom configure method since it doesn't include the server build system and review code

    bool vExists = server->hasVersion(version);
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }
    if(!vExists){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverUnsupportedVersion()});
        return res;
    }
    mcsm::ServerOption serverOption(version, server);
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }
    return configure(serverOption, name, option, autoUpdate);
    */
}

bool mcsm::CustomServer::isFile(const std::string& location) const {
    return mcsm::fileExists(location);
}

bool mcsm::CustomServer::isURL(const std::string& location) const {
    std::regex urlPattern(
        R"(https?:\/\/(www\.)?[-a-zA-Z0-9@:%._\+~#=]{1,256}\.[a-zA-Z0-9()]{1,6}\b"
        R"([-a-zA-Z0-9()@:%_\+.~#?&//=]*))"
    );
    return std::regex_match(location, urlPattern);
}

std::string mcsm::CustomServer::getBasedServer() const {
    return "unknown";
}

bool mcsm::CustomServer::isBasedAs(const std::string& /* input */) const {
    return false;
}

std::string mcsm::CustomServer::getWebSite() const {
    return "unknown";
}

std::string mcsm::CustomServer::getGitHub() const {
    return "unknown";
}