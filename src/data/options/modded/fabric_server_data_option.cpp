#include <mcsm/data/options/modded/fabric_server_data_option.h>

mcsm::FabricServerDataOption::FabricServerDataOption() : ServerDataOption() {}

mcsm::FabricServerDataOption::FabricServerDataOption(const std::string& path) : ServerDataOption(path) {}

mcsm::FabricServerDataOption::~FabricServerDataOption(){}

std::string mcsm::FabricServerDataOption::getLoaderVersion() const {
    if(!this->option->exists()){
        mcsm::error("Option does not exist; Task aborted.");
        std::exit(1);
    }
    if(this->option->getValue("last_downloaded_loader_version") == nullptr){
        mcsm::error("No \"last_downloaded_loader_version\" value specified in file " + this->option->getName());
        mcsm::error("Manually editing the launch profile might have caused this issue.");
        mcsm::error("If you know what you're doing, I believe you that you know how to handle this issue.");
        mcsm::error("If you believe that this is a software issue, please report it to GitHub (https://github.com/dodoman8067/mcsm).");
        std::exit(1);
    }
    if(!this->option->getValue("last_downloaded_loader_version").is_string()){
        mcsm::error("Value \"last_downloaded_loader_version\" has to be a string, but it's not.");
        mcsm::error("Manually editing the launch profile might have caused this issue.");
        mcsm::error("If you know what you're doing, I believe you that you know how to handle this issue.");
        mcsm::error("If you believe that this is a software issue, please report it to GitHub (https://github.com/dodoman8067/mcsm).");
        std::exit(1);
    }
    return this->option->getValue("last_downloaded_loader_version");
}

void mcsm::FabricServerDataOption::updateLoaderVersion(const std::string& version){
    this->option->setValue("last_downloaded_loader_version", version);
}

std::string mcsm::FabricServerDataOption::getInstallerVersion() const {
    if(!this->option->exists()){
        mcsm::error("Option does not exist; Task aborted.");
        std::exit(1);
    }
    if(this->option->getValue("last_downloaded_installer_version") == nullptr){
        mcsm::error("No \"last_downloaded_installer_version\" value specified in file " + this->option->getName());
        mcsm::error("Manually editing the launch profile might have caused this issue.");
        mcsm::error("If you know what you're doing, I believe you that you know how to handle this issue.");
        mcsm::error("If you believe that this is a software issue, please report it to GitHub (https://github.com/dodoman8067/mcsm).");
        std::exit(1);
    }
    if(!this->option->getValue("last_downloaded_installer_version").is_string()){
        mcsm::error("Value \"last_downloaded_installer_version\" has to be a string, but it's not.");
        mcsm::error("Manually editing the launch profile might have caused this issue.");
        mcsm::error("If you know what you're doing, I believe you that you know how to handle this issue.");
        mcsm::error("If you believe that this is a software issue, please report it to GitHub (https://github.com/dodoman8067/mcsm).");
        std::exit(1);
    }
    return this->option->getValue("last_downloaded_installer_version");
}

void mcsm::FabricServerDataOption::updateInstallerVersion(const std::string& version){
    this->option->setValue("last_downloaded_installer_version", version);
}