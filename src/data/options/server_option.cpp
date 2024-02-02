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

mcsm::ServerOption::ServerOption() : ServerOption(mcsm::getCurrentPath()){}

mcsm::ServerOption::ServerOption(const std::string& version, const std::string& path){
    if(!mcsm::fileExists(path)){
        if(!mcsm::mkdir(path)){
            mcsm::warning("Path mkdir failed : " + path);
            mcsm::warning("Task aborted.");
            std::exit(1);
        }
    }
    mcsm::Option option(path, "server");

    if(!option.exists()){
        mcsm::error("File server.json cannot be found.");
        mcsm::error("Task aborted.");
        std::exit(1);
    }
    if(option.getValue("type") == nullptr){
        mcsm::error("Option \"type\" cannot be found.");
        mcsm::error("Task aborted.");
        std::exit(1);    
    }
    if(!option.getValue("type").is_string()){
        mcsm::error("Value \"type\" has to be a string, but it's not.");
        mcsm::error("Manually editing the launch profile might have caused this issue.");
        mcsm::error("If you know what you're doing, I believe you that you know how to handle this issue.");
        mcsm::error("If you believe that this is a software issue, please report it to GitHub (https://github.com/dodoman8067/mcsm).");
        std::exit(1);
    }

    std::string server = option.getValue("type");
    std::shared_ptr<mcsm::Server> sPtr = mcsm::server::detectServerType(server);
    this->path = path;
    this->version = version;
    this->server = sPtr;
}

mcsm::ServerOption::ServerOption(const std::string& path){
    if(!mcsm::fileExists(path)){
        if(!mcsm::mkdir(path)){
            mcsm::warning("Path mkdir failed : " + path);
            mcsm::warning("Task aborted.");
            std::exit(1);
        }
    }
    mcsm::Option option(path, "server");

    if(!option.exists()){
        mcsm::error("File server.json cannot be found.");
        mcsm::error("Task aborted.");
        std::exit(1);
    }
    if(option.getValue("type") == nullptr){
        mcsm::error("Option \"type\" cannot be found.");
        mcsm::error("Task aborted.");
        std::exit(1);    
    }
    if(!option.getValue("type").is_string()){
        mcsm::error("Value \"type\" has to be a string, but it's not.");
        mcsm::error("Manually editing the launch profile might have caused this issue.");
        mcsm::error("If you know what you're doing, I believe you that you know how to handle this issue.");
        mcsm::error("If you believe that this is a software issue, please report it to GitHub (https://github.com/dodoman8067/mcsm).");
        std::exit(1);
    }

    if(option.getValue("version") == nullptr){
        mcsm::error("Option \"version\" cannot be found.");
        mcsm::error("Task aborted.");
        std::exit(1);    
    }
    if(!option.getValue("version").is_string()){
        mcsm::error("Value \"version\" has to be a string, but it's not.");
        mcsm::error("Manually editing the launch profile might have caused this issue.");
        mcsm::error("If you know what you're doing, I believe you that you know how to handle this issue.");
        mcsm::error("If you believe that this is a software issue, please report it to GitHub (https://github.com/dodoman8067/mcsm).");
        std::exit(1);
    }

    std::string server = option.getValue("type");
    std::string version = option.getValue("version");
    std::shared_ptr<mcsm::Server> sPtr = mcsm::server::detectServerType(server);
    this->path = path;
    this->server = sPtr;
    this->version = version;
}

mcsm::ServerOption::ServerOption(const std::string& version, std::shared_ptr<mcsm::Server> server){
    this->version = version;
    this->server = server;
    this->path = mcsm::getCurrentPath();
}

mcsm::ServerOption::ServerOption(const std::string& version, std::shared_ptr<mcsm::Server> server, const std::string& path){
    if(!mcsm::fileExists(path)){
        if(!mcsm::mkdir(path)){
            mcsm::warning("Path mkdir failed : " + path);
            mcsm::warning("Task aborted.");
            std::exit(1);
        }
    }

    this->version = version;
    this->server = server;
    this->path = path;
}

mcsm::ServerOption::~ServerOption(){
    
}

void mcsm::ServerOption::create(const std::string& name, mcsm::JvmOption& defaultOption){
    mcsm::Option option(this->path, "server");
    if(option.exists()){
        mcsm::error("Server is already configured in directory " + this->path + ".");
        mcsm::error("Please create a server.json file in other directories.");
        std::exit(1);
    }

    mcsm::ServerDataOption serverDataOpt(this->path);
    serverDataOpt.create("none");

    nlohmann::json profileObj;
    profileObj["name"] = defaultOption.getProfileName();
    if(defaultOption.getSearchTarget() == mcsm::SearchTarget::GLOBAL){
        profileObj["location"] = "global";
    }else{
        profileObj["location"] = "current";
    }
    option.setValue("name", name);
    option.setValue("version", this->version);
    option.setValue("default_launch_profile", profileObj);
    option.setValue("server_jar", this->server->getJarFile(this->path));
    option.setValue("server_build", "latest");
    option.setValue("type", this->server->getTypeAsString());
    serverDataOpt.updateServerTimeCreated();
}

void mcsm::ServerOption::start(){
    std::unique_ptr<mcsm::JvmOption> jvmOpt = getDefaultOption();
    start(std::move(jvmOpt));
}

void mcsm::ServerOption::start(std::unique_ptr<mcsm::JvmOption> option){
    mcsm::ServerDataOption serverDataOpt;

    if(!mcsm::fileExists(this->path + "/server.json")){
        mcsm::error("File server.json cannot be found.");
        mcsm::error("Task aborted.");
        std::exit(1);
    }

    if(this->server == nullptr){
        this->server.reset();
        this->server = mcsm::server::detectServerType(getServerType());
    }

    mcsm::success("Starting server..");
    mcsm::info("Server name : " + getServerName());
    mcsm::info("Server MC version : " + getServerVersion());
    mcsm::info("Server JVM launch profile : " + option->getProfileName());
    serverDataOpt.updateLastTimeLaunched();
    this->server->start(*option);
    mcsm::info("Server stopped.\n");
}

bool mcsm::ServerOption::exists(){
    mcsm::Option option(this->path, "server");
    return option.exists();
}

std::unique_ptr<mcsm::JvmOption> mcsm::ServerOption::getDefaultOption() const {
    mcsm::Option option(this->path, "server");
    if(!option.exists()){
        mcsm::error("Option does not exist; Task aborted.");
        std::exit(1);
    }
    nlohmann::json profileObj = option.getValue("default_launch_profile");
    if(profileObj == nullptr){
        mcsm::error("No default launch profile specified in file " + option.getName());
        mcsm::error("Manually editing the launch profile might have caused this issue.");
        mcsm::error("If you know what you're doing, I believe you that you know how to handle this issue.");
        mcsm::error("If you believe that this is a software issue, please report it to GitHub (https://github.com/dodoman8067/mcsm).");
        std::exit(1);       
    }
    if(profileObj["name"] == nullptr){
        mcsm::error("No default launch profile name specified in file " + option.getName());
        mcsm::error("Manually editing the launch profile might have caused this issue.");
        mcsm::error("If you know what you're doing, I believe you that you know how to handle this issue.");
        mcsm::error("If you believe that this is a software issue, please report it to GitHub (https://github.com/dodoman8067/mcsm).");
        std::exit(1);         
    }
    if(!profileObj["name"].is_string()){
        mcsm::error("Value \"name\" in \"default_launch_profile\" has to be a string, but it's not.");
        mcsm::error("Manually editing the launch profile might have caused this issue.");
        mcsm::error("If you know what you're doing, I believe you that you know how to handle this issue.");
        mcsm::error("If you believe that this is a software issue, please report it to GitHub (https://github.com/dodoman8067/mcsm).");
        std::exit(1);
    }
    if(profileObj["location"] == nullptr){
        mcsm::error("No default launch profile location specified in file " + option.getName());
        mcsm::error("Manually editing the launch profile might have caused this issue.");
        mcsm::error("If you know what you're doing, I believe you that you know how to handle this issue.");
        mcsm::error("If you believe that this is a software issue, please report it to GitHub (https://github.com/dodoman8067/mcsm).");
        std::exit(1);       
    }
    if(!profileObj["location"].is_string()){
        mcsm::error("Value \"location\" in \"default_launch_profile\" has to be a string, but it's not.");
        mcsm::error("Manually editing the launch profile might have caused this issue.");
        mcsm::error("If you know what you're doing, I believe you that you know how to handle this issue.");
        mcsm::error("If you believe that this is a software issue, please report it to GitHub (https://github.com/dodoman8067/mcsm).");
        std::exit(1);
    }
    mcsm::SearchTarget target;
    if(profileObj["location"] == "global"){
        target = mcsm::SearchTarget::GLOBAL;
    }else if(profileObj["location"] == "current"){
        target = mcsm::SearchTarget::CURRENT;
    }else{
        mcsm::error("Value \"location\" in \"default_launch_profile\" has to be \"global\" or \"current\", but it's not.");
        mcsm::error("Manually editing the launch profile might have caused this issue.");
        mcsm::error("If you know what you're doing, I believe you that you know how to handle this issue.");
        mcsm::error("If you believe that this is a software issue, please report it to GitHub (https://github.com/dodoman8067/mcsm).");
        std::exit(1);
    }
    std::unique_ptr<mcsm::JvmOption> jvmOption = std::make_unique<mcsm::JvmOption>(profileObj["name"], target);
    if(!jvmOption->exists() || jvmOption == nullptr){
        mcsm::error("Invalid default launch profile.");
        mcsm::error("File server.json might be corrupted or the profile is removed.");
        mcsm::error("Please change the profile or create a new server.json file.");
        std::exit(1);
    }
    return jvmOption;
}

void mcsm::ServerOption::setDefaultOption(std::unique_ptr<mcsm::JvmOption> jvmOption){
    mcsm::Option option(this->path, "server");
    nlohmann::json profileObj;
    profileObj["name"] = jvmOption->getProfileName();
    if(jvmOption->getSearchTarget() == mcsm::SearchTarget::GLOBAL){
        profileObj["location"] = "global";
    }else{
        profileObj["location"] = "current";
    }
    option.setValue("default_launch_profile", profileObj);
}

std::string mcsm::ServerOption::getServerName() const {
    mcsm::Option option(this->path, "server");
    if(!option.exists()){
        mcsm::error("Option does not exist; Task aborted.");
        std::exit(1);
    }
    if(option.getValue("name") == nullptr){
        mcsm::error("No \"name\" value specified in file " + option.getName());
        mcsm::error("Manually editing the launch profile might have caused this issue.");
        mcsm::error("If you know what you're doing, I believe you that you know how to handle this issue.");
        mcsm::error("If you believe that this is a software issue, please report it to GitHub (https://github.com/dodoman8067/mcsm).");
        std::exit(1);
    }
    if(!option.getValue("name").is_string()){
        mcsm::error("Value \"name\" has to be a string, but it's not.");
        mcsm::error("Manually editing the launch profile might have caused this issue.");
        mcsm::error("If you know what you're doing, I believe you that you know how to handle this issue.");
        mcsm::error("If you believe that this is a software issue, please report it to GitHub (https://github.com/dodoman8067/mcsm).");
        std::exit(1);
    }
    return option.getValue("name");
}

void mcsm::ServerOption::setServerName(const std::string& name){
    mcsm::Option option(this->path, "server");
    option.setValue("name", name);
}

std::string mcsm::ServerOption::getServerVersion() const {
    mcsm::Option option(this->path, "server");
    if(!option.exists()){
        mcsm::error("Option does not exist; Task aborted.");
        std::exit(1);
    }
    if(option.getValue("version") == nullptr){
        mcsm::error("No \"version\" value specified in file " + option.getName());
        mcsm::error("Manually editing the launch profile might have caused this issue.");
        mcsm::error("If you know what you're doing, I believe you that you know how to handle this issue.");
        mcsm::error("If you believe that this is a software issue, please report it to GitHub (https://github.com/dodoman8067/mcsm).");
        std::exit(1);
    }
    if(!option.getValue("version").is_string()){
        mcsm::error("Value \"version\" has to be a string, but it's not.");
        mcsm::error("Manually editing the launch profile might have caused this issue.");
        mcsm::error("If you know what you're doing, I believe you that you know how to handle this issue.");
        mcsm::error("If you believe that this is a software issue, please report it to GitHub (https://github.com/dodoman8067/mcsm).");
        std::exit(1);
    }
    return option.getValue("version");
}

void mcsm::ServerOption::setServerVersion(const std::string& version){
    mcsm::Option option(this->path, "server");
    option.setValue("version", version);    
}

std::string mcsm::ServerOption::getServerType() const {
    mcsm::Option option(this->path, "server");
    if(!option.exists()){
        mcsm::error("Option does not exist; Task aborted.");
        std::exit(1);
    }
    if(option.getValue("type") == nullptr){
        mcsm::error("No \"type\" value specified in file " + option.getName());
        mcsm::error("Manually editing the launch profile might have caused this issue.");
        mcsm::error("If you know what you're doing, I believe you that you know how to handle this issue.");
        mcsm::error("If you believe that this is a software issue, please report it to GitHub (https://github.com/dodoman8067/mcsm).");
        std::exit(1);
    }
    if(!option.getValue("type").is_string()){
        mcsm::error("Value \"type\" has to be a string, but it's not.");
        mcsm::error("Manually editing the launch profile might have caused this issue.");
        mcsm::error("If you know what you're doing, I believe you that you know how to handle this issue.");
        mcsm::error("If you believe that this is a software issue, please report it to GitHub (https://github.com/dodoman8067/mcsm).");
        std::exit(1);
    }
    return option.getValue("type");
}

std::string mcsm::ServerOption::getServerJarFile() const{
    mcsm::Option option(this->path, "server");
    if(!option.exists()){
        mcsm::error("Option does not exist; Task aborted.");
        std::exit(1);
    }
    if(option.getValue("server_jar") == nullptr){
        mcsm::error("No \"server_jar\" value specified in file " + option.getName());
        mcsm::error("Manually editing the launch profile might have caused this issue.");
        mcsm::error("If you know what you're doing, I believe you that you know how to handle this issue.");
        mcsm::error("If you believe that this is a software issue, please report it to GitHub (https://github.com/dodoman8067/mcsm).");
        std::exit(1);
    }
    if(!option.getValue("server_jar").is_string()){
        mcsm::error("Value \"server_jar\" has to be a string, but it's not.");
        mcsm::error("Manually editing the launch profile might have caused this issue.");
        mcsm::error("If you know what you're doing, I believe you that you know how to handle this issue.");
        mcsm::error("If you believe that this is a software issue, please report it to GitHub (https://github.com/dodoman8067/mcsm).");
        std::exit(1);
    }
    return option.getValue("server_jar");
}

void mcsm::ServerOption::setServerJarFile(const std::string& name){
    mcsm::Option option(this->path, "server");
    option.setValue("server_jar", name);
}

std::string mcsm::ServerOption::getServerJarBuild() const {
    mcsm::Option option(this->path, "server");
    if(!option.exists()){
        mcsm::error("Option does not exist; Task aborted.");
        std::exit(1);
    }
    if(option.getValue("server_build") == nullptr){
        mcsm::error("No \"server_build\" value specified in file " + option.getName());
        mcsm::error("Manually editing the launch profile might have caused this issue.");
        mcsm::error("If you know what you're doing, I believe you that you know how to handle this issue.");
        mcsm::error("If you believe that this is a software issue, please report it to GitHub (https://github.com/dodoman8067/mcsm).");
        std::exit(1);
    }
    if(!option.getValue("server_build").is_string()){
        mcsm::error("Value \"server_build\" has to be a string, but it's not.");
        mcsm::error("Manually editing the launch profile might have caused this issue.");
        mcsm::error("If you know what you're doing, I believe you that you know how to handle this issue.");
        mcsm::error("If you believe that this is a software issue, please report it to GitHub (https://github.com/dodoman8067/mcsm).");
        std::exit(1);
    }
    return option.getValue("server_build");
}

void mcsm::ServerOption::setServerJarBuild(const std::string& build){
    mcsm::Option option(this->path, "server");
    option.setValue("server_build", build);
}

std::shared_ptr<mcsm::Server> mcsm::ServerOption::getServer() const {
    if(this->server != nullptr) return this->server;
    mcsm::error("Server instance null.");
    mcsm::error("There's a high chance to be a software issue. Please report it to GitHub (https://github.com/dodoman8067/mcsm).");
    std::exit(1);
}