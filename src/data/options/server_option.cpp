#include <mcsm/data/options/server_option.h>

mcsm::ServerOption::ServerOption(const std::string& version){
    this->version = version;
}

mcsm::ServerOption::~ServerOption(){}

void mcsm::ServerOption::create(const std::string& name, mcsm::JvmOption& defaultOption, std::unique_ptr<mcsm::Server> server){
    this->server = std::move(server);
    mcsm::Option option(".", "server");
    if(option.exists()) return;

    nlohmann::json profileObj;
    profileObj["name"] = defaultOption.getProfilePath();
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
    if(!option->exists()){
        std::cerr << "File server.json cannot be found.\n";
        std::cerr << "Task aborted.\n";
        std::exit(1);
    }
    if(std::filesystem::exists(this->server->getJarFile())){
        std::cerr << "Server jarfile cannot be found.\n";
        std::cerr << "Task aborted.\n";
        std::exit(1);  
    }
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
        std::cerr << "Error: Invaild default launch profile.\n";
        std::cerr << "File server.json might be corrupted or the profile is removed.\n";
        std::cerr << "Please change the profile or create a new server.json file.\n";
        std::exit(1);
    }
    return jvmOption;
}

std::string mcsm::ServerOption::getFileName() const {
    mcsm::Option option(".", "server");
    return option.getValue("name");
}