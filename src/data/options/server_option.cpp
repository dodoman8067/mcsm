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


#include <mcsm/data/options/server_option.h>

// See mcsm::FabricServerOption class if you want to see how Fabric server option works.

// You might be wondering why server option crating class would fail due to file non existent; It's because the first three constructors are for loading the existing config.
mcsm::ServerOption::ServerOption() : ServerOption(mcsm::getCurrentPath()){}

mcsm::ServerOption::ServerOption(const std::string& version, const std::string& path){
    // Result handle
    bool fileExists = mcsm::fileExists(path);
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return;

    if(!fileExists){
        // Create folders
        if(!mcsm::mkdir(path)){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
                "Path mkdir failed : " + path,
                "High chance to be a software issue, please report this to GitHub (https://github.com/dodoman8067/mcsm)."
            }});
            return;
        }
    }
    mcsm::Option option(path, "server");

    bool exists = option.exists();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return;

    if(!exists){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverNotConfigured()});
        return;
    }

    // Gets the server type from json file.
    nlohmann::json type = option.getValue("type");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return;

    if(type == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFound("\"type\"", "server.json")});
        return; 
    }
    if(!type.is_string()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"type\"", "string")});
        return; 
    }

    // Creates a server instance with the string obtained from from option.getValue("type").
    std::shared_ptr<mcsm::Server> sPtr = mcsm::server::detectServerType(type);
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return;

    this->path = path;
    this->version = version;
    this->server = sPtr;
    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
}

mcsm::ServerOption::ServerOption(const std::string& path){
    // Checks if the path exists. (not file)
    bool fileExists = mcsm::fileExists(path);
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return;

    if(!fileExists){
        if(!mcsm::mkdir(path)){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
                "Path mkdir failed : " + path,
                "High chance to be a software issue, please report this to GitHub (https://github.com/dodoman8067/mcsm)."
            }});
            return;
        }
    }
    mcsm::Option option(path, "server");

    bool exists = option.exists();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return;

    if(!exists){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverNotConfigured()});
        return;
    }

    nlohmann::json type = option.getValue("type");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return;

    if(type == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFound("\"type\"", "server.json")});
        return; 
    }
    if(!type.is_string()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"type\"", "string")});
        return; 
    }

    nlohmann::json version = option.getValue("version");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return;

    if(version == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFound("\"version\"", "server.json")});
        return;   
    }
    if(!version.is_string()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"version\"", "string")});
        return; 
    }

    std::shared_ptr<mcsm::Server> sPtr = mcsm::server::detectServerType(type);
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return;
    this->path = path;
    this->server = sPtr;
    this->version = version;
    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
}

mcsm::ServerOption::ServerOption(const std::string& version, std::shared_ptr<mcsm::Server> server){
    this->version = version;
    this->server = server;
    std::string path1 = mcsm::getCurrentPath();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return;
    this->path = path1;
    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
}

mcsm::ServerOption::ServerOption(const std::string& version, std::shared_ptr<mcsm::Server> server, const std::string& path){
    if(!mcsm::fileExists(path)){
        if(!mcsm::mkdir(path)){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
                "Path mkdir failed : " + path,
                "High chance to be a software issue, please report this to GitHub (https://github.com/dodoman8067/mcsm)."
            }});
            return;
        }
    }

    this->version = version;
    this->server = server;
    this->path = path;
    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
}

mcsm::ServerOption::~ServerOption(){
    
}

mcsm::Result mcsm::ServerOption::create(const std::string& name, mcsm::JvmOption& defaultOption){
    return create(name, defaultOption, true);
}

mcsm::Result mcsm::ServerOption::create(const std::string& name, mcsm::JvmOption& defaultOption, const bool& update){
    mcsm::Option option(this->path, "server");
    
    bool optExists = option.exists();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    if(optExists){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverAlreadyConfigured()});
        return res;
    }

    mcsm::ServerDataOption serverDataOpt(this->path);
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    mcsm::Result res1 = serverDataOpt.create("none");
    if(!res1.isSuccess()) return res1;

    nlohmann::json profileObj;
    profileObj["name"] = defaultOption.getProfileName();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    if(defaultOption.getSearchTarget() == mcsm::SearchTarget::GLOBAL){
        profileObj["location"] = "global";
    }else{
        profileObj["location"] = "current";
    }
    
    mcsm::Result res2 = option.setValue("name", name);
    if(!res2.isSuccess()) return res2;
    
    mcsm::Result res3 = option.setValue("version", this->version);
    if(!res3.isSuccess()) return res3;
    
    mcsm::Result res4 = option.setValue("default_launch_profile", profileObj);
    if(!res4.isSuccess()) return res4;
    
    std::string jarFile = this->server->getJarFile(this->path);
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        jarFile = this->server->getTypeAsString() + ".jar";
    }

    mcsm::Result res5 = option.setValue("server_jar", jarFile);
    if(!res5.isSuccess()) return res5;

    mcsm::Result res6 = option.setValue("server_build", "latest");
    if(!res6.isSuccess()) return res6;

    mcsm::Result res10 = option.setValue("auto_update", update);
    if(!res10.isSuccess()) return res10;

    mcsm::Result res7 = option.setValue("type", this->server->getTypeAsString());
    if(!res7.isSuccess()) return res7;

    mcsm::Result res8 = serverDataOpt.updateServerTimeCreated();
    if(!res8.isSuccess()) return res8;

    mcsm::Result res9({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
    return res9;
}

mcsm::Result mcsm::ServerOption::start(){
    std::unique_ptr<mcsm::JvmOption> jvmOpt = getDefaultOption();
    return start(std::move(jvmOpt), this->path, this->path);
}

mcsm::Result mcsm::ServerOption::start(std::unique_ptr<mcsm::JvmOption> option){
    return start(std::move(option), this->path, this->path);
}

mcsm::Result mcsm::ServerOption::start(std::unique_ptr<mcsm::JvmOption> option, const std::string& path, const std::string& optionPath){
    mcsm::ServerDataOption serverDataOpt(optionPath);
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    bool fileExists = mcsm::fileExists(optionPath + "/server.json");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    if(!fileExists){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverNotConfigured()});
        return res;
    }

    if(this->server == nullptr){
        this->server.reset();
        this->server = mcsm::server::detectServerType(getServerType());
        
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
            std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
            mcsm::Result res(resp.first, resp.second);
            return res;
        }
    }

    std::string name = getServerName();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    std::string version = getServerVersion();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    std::string profileName = option->getProfileName();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    mcsm::success("Starting server..");
    mcsm::info("Server name : " + name);
    mcsm::info("Server MC version : " + version);
    mcsm::info("Server JVM launch profile : " + profileName);
    mcsm::Result res = serverDataOpt.updateLastTimeLaunched();
    if(!res.isSuccess()) return res;
    mcsm::Result res2 = this->server->start(*option, path, optionPath);
    return res2;
}

//WIP
mcsm::Result mcsm::ServerOption::update(const std::string& /* path */, const std::string& optionPath){
    bool fileExists = mcsm::fileExists(optionPath + "/server.json");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    if(!fileExists){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverNotConfigured()});
        return res;
    }

    if(this->server == nullptr){
        this->server.reset();
        this->server = mcsm::server::detectServerType(getServerType());
        
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
            std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
            mcsm::Result res(resp.first, resp.second);
            return res;
        }
    }
    
    mcsm::Result res1({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
    return res1;
}

bool mcsm::ServerOption::exists(){
    mcsm::Option option(this->path, "server");
    bool rt = option.exists();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return false;
    return rt;
}

std::unique_ptr<mcsm::JvmOption> mcsm::ServerOption::getDefaultOption() const {
    mcsm::Option option(this->path, "server");
    bool optExists = option.exists();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return nullptr;
    if(!optExists){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverNotConfigured()});
        return nullptr;
    }

    nlohmann::json profileObj = option.getValue("default_launch_profile");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return nullptr;

    if(profileObj == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverDefaultProfileNotFound(option.getName())});
        return nullptr;
    }
    if(profileObj["name"] == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
            "No default launch profile name specified in file " + option.getName(),
            "Manually editing the launch profile might have caused this issue.",
            "If you know what you're doing, I believe you that you know how to handle this issue.",
            "Please report this to GitHub (https://github.com/dodoman8067/mcsm) if you think this is a software issue."
        }});
        return nullptr;      
    }
    if(!profileObj["name"].is_string()){
        // Don't use jsonWrongType
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
            "Value \"name\" in \"default_launch_profile\" has to be a string, but it's not.",
            "Manually editing the launch profile might have caused this issue.",
            "If you know what you're doing, I believe you that you know how to handle this issue.",
            "Please report this to GitHub (https://github.com/dodoman8067/mcsm) if you think this is a software issue."
        }});
        return nullptr;
    }
    if(profileObj["location"] == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
            "No default launch profile location specified in file " + option.getName(),
            "Manually editing the launch profile might have caused this issue.",
            "If you know what you're doing, I believe you that you know how to handle this issue.",
            "Please report this to GitHub (https://github.com/dodoman8067/mcsm) if you think this is a software issue."
        }});
        return nullptr;    
    }
    if(!profileObj["location"].is_string()){
        // Don't use jsonWrongType
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
            "Value \"location\" in \"default_launch_profile\" has to be a string, but it's not.",
            "Manually editing the launch profile might have caused this issue.",
            "If you know what you're doing, I believe you that you know how to handle this issue.",
            "Please report this to GitHub (https://github.com/dodoman8067/mcsm) if you think this is a software issue."
        }});
        return nullptr;
    }
    mcsm::SearchTarget target;
    if(profileObj["location"] == "global"){
        target = mcsm::SearchTarget::GLOBAL;
    }else if(profileObj["location"] == "current"){
        target = mcsm::SearchTarget::CURRENT;
    }else{
        // Don't use jsonWrongType
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
            "Value \"location\" in \"default_launch_profile\" has to be \"global\" or \"current\", but it's not.",
            "Manually editing the launch profile might have caused this issue.",
            "If you know what you're doing, I believe you that you know how to handle this issue.",
            "Please report this to GitHub (https://github.com/dodoman8067/mcsm) if you think this is a software issue."
        }});
        return nullptr;
    }
    std::unique_ptr<mcsm::JvmOption> jvmOption = std::make_unique<mcsm::JvmOption>(profileObj["name"], target);
    if(!jvmOption->exists() || jvmOption == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
            "Invalid default launch profile.",
            "File server.json might be corrupted or the profile is removed.",
            "Please change the profile or create a new server.json file."
        }});
        return nullptr;
    }

    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
    return jvmOption;
}

mcsm::Result mcsm::ServerOption::setDefaultOption(std::unique_ptr<mcsm::JvmOption> jvmOption){
    mcsm::Option option(this->path, "server");
    nlohmann::json profileObj;
    profileObj["name"] = jvmOption->getProfileName();
    if(jvmOption->getSearchTarget() == mcsm::SearchTarget::GLOBAL){
        profileObj["location"] = "global";
    }else{
        profileObj["location"] = "current";
    }
    return option.setValue("default_launch_profile", profileObj);
}

std::string mcsm::ServerOption::getServerName() const {
    mcsm::Option option(this->path, "server");
    bool optExists = option.exists();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";
    if(!optExists){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverNotConfigured()});
        return "";
    }

    nlohmann::json value = option.getValue("name");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";

    if(value == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFound("\"name\"", option.getName())});
        return "";
    }
    if(!value.is_string()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"name\"", "string")});
        return "";
    }

    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
    return value;
}

mcsm::Result mcsm::ServerOption::setServerName(const std::string& name){
    mcsm::Option option(this->path, "server");
    return option.setValue("name", name);
}

std::string mcsm::ServerOption::getServerVersion() const {
    mcsm::Option option(this->path, "server");
    bool optExists = option.exists();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";
    if(!optExists){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverNotConfigured()});
        return "";
    }

    nlohmann::json value = option.getValue("version");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";

    if(value == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFound("\"version\"", option.getName())});
        return "";
    }
    if(!value.is_string()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"version\"", "string")});
        return "";
    }

    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
    return value;
}

mcsm::Result mcsm::ServerOption::setServerVersion(const std::string& version){
    mcsm::Option option(this->path, "server");
    return option.setValue("version", version);    
}

std::string mcsm::ServerOption::getServerType() const {
    mcsm::Option option(this->path, "server");
    bool optExists = option.exists();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";
    if(!optExists){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverNotConfigured()});
        return "";
    }

    nlohmann::json value = option.getValue("type");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";

    if(value == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFound("\"type\"", option.getName())});
        return "";
    }
    if(!value.is_string()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"type\"", "string")});
        return "";
    }

    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
    return value;
}

std::string mcsm::ServerOption::getServerJarFile() const{
    mcsm::Option option(this->path, "server");
    bool optExists = option.exists();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";
    if(!optExists){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverNotConfigured()});
        return "";
    }

    nlohmann::json value = option.getValue("server_jar");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";

    if(value == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFound("\"server_jar\"", option.getName())});
        return "";
    }
    if(!value.is_string()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"server_jar\"", "string")});
        return "";
    }

    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
    return value;
}

mcsm::Result mcsm::ServerOption::setServerJarFile(const std::string& name){
    mcsm::Option option(this->path, "server");
    return option.setValue("server_jar", name);
}

std::string mcsm::ServerOption::getServerJarBuild() const {
    mcsm::Option option(this->path, "server");
    bool optExists = option.exists();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";
    if(!optExists){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverNotConfigured()});
        return "";
    }

    nlohmann::json value = option.getValue("server_build");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";

    if(value == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFound("\"server_build\"", option.getName())});
        return "";
    }
    if(!value.is_string()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"server_build\"", "string")});
        return "";
    }

    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
    return value;
}

mcsm::Result mcsm::ServerOption::setServerJarBuild(const std::string& build){
    mcsm::Option option(this->path, "server");
    return option.setValue("server_build", build);
}

bool mcsm::ServerOption::doesAutoUpdate() const {
    mcsm::Option option(this->path, "server");
    bool optExists = option.exists();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";
    if(!optExists){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverNotConfigured()});
        return "";
    }

    nlohmann::json value = option.getValue("auto_update");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";

    if(value == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFound("\"auto_update\"", option.getName())});
        return "";
    }
    if(!value.is_boolean()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"auto_update\"", "boolean")});
        return "";
    }

    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
    return value;
}

mcsm::Result mcsm::ServerOption::setAutoUpdate(const bool& update){
    mcsm::Option option(this->path, "server");
    return option.setValue("auto_update", update);
}

std::shared_ptr<mcsm::Server> mcsm::ServerOption::getServer() const {
    if(this->server != nullptr){ mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}}); return this->server; }
    mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
        "Server instance null.",
        "There's a high chance to be a software issue. please report this to GitHub (https://github.com/dodoman8067/mcsm)."
    }});
    return nullptr;
}