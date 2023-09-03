#include "version_command.h"

mcsm::VersionCommand::VersionCommand(const std::string& name, const std::string& description, const std::string& version) : mcsm::Command(name, description) {
    this->version = version;
}

mcsm::VersionCommand::~VersionCommand(){

}

void mcsm::VersionCommand::execute(const std::vector<std::string>& args){
    std::cout << "MCSM version : " << version << std::endl;
    std::cout << "cURL version : " << curl_version() << std::endl;
}