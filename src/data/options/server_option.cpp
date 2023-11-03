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

mcsm::ServerOption::ServerOption(const std::string& version, std::shared_ptr<mcsm::Server> server){
    this->version = version;
    this->server = server;
}

mcsm::ServerOption::~ServerOption(){}

void mcsm::ServerOption::create(const std::string& name, mcsm::JvmOption& defaultOption){
    mcsm::Option option(".", "server");
    if(option.exists()) return;

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
    option.setValue("server_jar", this->server->getJarFile());
}

void mcsm::ServerOption::start(){
    std::unique_ptr<mcsm::JvmOption> jvmOpt = getDefaultOption();
    start(std::move(jvmOpt));
}

void mcsm::ServerOption::start(std::unique_ptr<mcsm::JvmOption> option){
    if(!std::filesystem::exists("server.json")){
        std::cerr << "[mcsm] File server.json cannot be found.\n";
        std::cerr << "[mcsm] Task aborted.\n";
        std::exit(1);
    }
    if(!std::filesystem::exists(this->server->getJarFile())){
        std::cerr << "[mcsm] Server jarfile cannot be found.\n";
        std::cerr << "[mcsm] Task aborted.\n";
        std::exit(1);  
    }
    std::cout << "[mcsm] Starting server..\n";
    std::cout << "[mcsm] Server name : " << getServerName() << "\n";
    std::cout << "[mcsm] Server MC version : " << getServerVersion() << "\n";
    std::cout << "[mcsm] Server JVM launch profile : " << option->getProfileName() << "\n";
    this->server->start(*option);
}

bool mcsm::ServerOption::exists(){
    mcsm::Option option(".", "server");
    return option.exists();
}

std::unique_ptr<mcsm::JvmOption> mcsm::ServerOption::getDefaultOption() const{
    mcsm::Option option(".", "server");
    nlohmann::json profileObj = option.getValue("default_launch_profile");
    mcsm::SearchTarget target;
    if(profileObj["location"] == "global"){
        target = mcsm::SearchTarget::GLOBAL;
    }else{
        target = mcsm::SearchTarget::CURRENT;
    }
    std::unique_ptr<mcsm::JvmOption> jvmOption = std::make_unique<mcsm::JvmOption>(profileObj["name"], target);
    if(!jvmOption->exists()){
        std::cerr << "Error: Invalid default launch profile.\n";
        std::cerr << "File server.json might be corrupted or the profile is removed.\n";
        std::cerr << "Please change the profile or create a new server.json file.\n";
        std::exit(1);
    }
    return jvmOption;
}

std::string mcsm::ServerOption::getServerName() const {
    mcsm::Option option(".", "server");
    return option.getValue("name");
}

std::string mcsm::ServerOption::getServerVersion() const {
    mcsm::Option option(".", "server");
    return option.getValue("version");  
}