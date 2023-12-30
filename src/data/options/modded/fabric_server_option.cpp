#include <mcsm/data/options/modded/fabric_server_option.h>

mcsm::FabricServerOption::FabricServerOption(){
    mcsm::Option option(".", "server");

    if(!option.exists()){
        mcsm::error("File server.json cannot be found.");
        mcsm::error("Task aborted.");
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
    FabricServerOption(option.getValue("version"));
}

mcsm::FabricServerOption::FabricServerOption(const std::string& version){
    mcsm::Option option(".", "server");

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

    std::shared_ptr<mcsm::Server> sPtr = std::make_shared<mcsm::FabricServer>();
    this->version = version;
    this->server = sPtr;
}

mcsm::FabricServerOption::FabricServerOption(const std::string& version, std::shared_ptr<mcsm::Server> server) : mcsm::ServerOption::ServerOption(version, server) {}

mcsm::FabricServerOption::~FabricServerOption(){

}

void mcsm::FabricServerOption::create(const std::string& name, mcsm::JvmOption& defaultOption){
    mcsm::Option option(".", "server");
    if(option.exists()){
        mcsm::error("Server is already configured in this directory.");
        mcsm::error("Please create a server.json file in other directories.");
    }

    mcsm::ServerDataOption serverDataOpt;
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
    option.setValue("server_jar", this->server->getJarFile());
    option.setValue("loader_version", "latest");
    option.setValue("installer_version", "latest");
    option.setValue("type", this->server->getTypeAsString());
    serverDataOpt.updateServerTimeCreated();
}

void mcsm::FabricServerOption::start(){

}

void mcsm::FabricServerOption::start(std::unique_ptr<mcsm::JvmOption> option){

}

std::string mcsm::FabricServerOption::getServerJarBuild() const {

}

void mcsm::FabricServerOption::setServerJarBuild(const std::string& build){
    
}

std::string mcsm::FabricServerOption::getLoaderVersion() const {

}

void mcsm::FabricServerOption::setLoaderVersion(const std::string& version){

}

std::string mcsm::FabricServerOption::getInstallerVersion() const{

}

void mcsm::FabricServerOption::setInstallerVersion(const std::string& version){

}