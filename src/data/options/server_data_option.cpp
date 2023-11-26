#include <mcsm/data/options/server_data_option.h>

mcsm::ServerDataOption::ServerDataOption(){
    this->option = std::make_unique<mcsm::Option>("./.mcsm/", "server_datas");
}

mcsm::ServerDataOption::~ServerDataOption(){

}

void mcsm::ServerDataOption::create(const std::string& lastTimeLaunched, const std::string& build){
    if(this->option->exists()){
        mcsm::error("Server data config already in place.");
        mcsm::error("Task aborted.");
        std::exit(1);
    }
    this->option->setValue("last_time_launched", lastTimeLaunched);
    this->option->setValue("current_build", build);
}

void mcsm::ServerDataOption::reset(){
    this->option->reset();
}

std::string mcsm::ServerDataOption::getLastTimeLaunched() const {
    if(!this->option->exists()){
        mcsm::error("Option does not exist; Task aborted.");
        std::exit(1);
    }
    if(this->option->getValue("last_time_launched") == nullptr){
        mcsm::error("No \"last_time_launched\" value specified in file " + this->option->getName());
        mcsm::error("Manually editing the launch profile might have caused this issue.");
        mcsm::error("If you know what you're doing, I believe you that you know how to handle this issue.");
        mcsm::error("If you believe that this is a software issue, please report it to GitHub (https://github.com/dodoman8067/mcsm).");
        std::exit(1);
    }
    return this->option->getValue("last_time_launched");
}

void mcsm::ServerDataOption::updateLastTimeLaunched(){
    char buffer[80];

    auto currentTimePoint = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(currentTimePoint);

    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&currentTime));

    this->option->setValue("last_time_launched", buffer);
}

std::string mcsm::ServerDataOption::getCurrentBuild() const {
    if(!this->option->exists()){
        mcsm::error("Option does not exist; Task aborted.");
        std::exit(1);
    }
    if(this->option->getValue("current_build") == nullptr){
        mcsm::error("No \"current_build\" value specified in file " + this->option->getName());
        mcsm::error("Manually editing the launch profile might have caused this issue.");
        mcsm::error("If you know what you're doing, I believe you that you know how to handle this issue.");
        mcsm::error("If you believe that this is a software issue, please report it to GitHub (https://github.com/dodoman8067/mcsm).");
        std::exit(1);
    }
    return this->option->getValue("current_build");
}

void mcsm::ServerDataOption::updateCurrentBuild(const std::string& build){
    this->option->setValue("current_build", build);
}

bool mcsm::ServerDataOption::exists() const {
    return this->option->exists();
}