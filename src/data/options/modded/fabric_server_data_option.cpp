#include <mcsm/data/options/modded/fabric_server_data_option.h>

mcsm::FabricServerDataOption::FabricServerDataOption() : ServerDataOption() {}

mcsm::FabricServerDataOption::FabricServerDataOption(const std::string& path) : ServerDataOption(path) {}

mcsm::FabricServerDataOption::~FabricServerDataOption(){}

std::string mcsm::FabricServerDataOption::getLoaderVersion() const {
    if(!this->option->exists()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::fileNotFound(this->option->getName())});
        return "";
    }
    if(this->option->getValue("last_downloaded_loader_version") == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFound("\"last_downloaded_loader_version\"", this->option->getName())});
        return "";
    }
    if(!this->option->getValue("last_downloaded_loader_version").is_string()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"last_downloaded_loader_version\"", "string")});
        return "";
    }
    return this->option->getValue("last_downloaded_loader_version");
}

mcsm::Result mcsm::FabricServerDataOption::updateLoaderVersion(const std::string& version){
    return this->option->setValue("last_downloaded_loader_version", version);
}

std::string mcsm::FabricServerDataOption::getInstallerVersion() const {
    if(!this->option->exists()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::fileNotFound(this->option->getName())});
        return "";
    }
    if(this->option->getValue("last_downloaded_installer_version") == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFound("\"last_downloaded_installer_version\"", this->option->getName())});
        return "";
    }
    if(!this->option->getValue("last_downloaded_installer_version").is_string()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"last_downloaded_installer_version\"", "string")});
        return "";
    }
    return this->option->getValue("last_downloaded_installer_version");
}

mcsm::Result mcsm::FabricServerDataOption::updateInstallerVersion(const std::string& version){
    return this->option->setValue("last_downloaded_installer_version", version);
}