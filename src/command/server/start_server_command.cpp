#include <mcsm/command/server/start_server_command.h>

mcsm::StartServerCommand::StartServerCommand(const std::string& name, const std::string& description) : mcsm::Command(name, description) {}

mcsm::StartServerCommand::~StartServerCommand() {}

void mcsm::StartServerCommand::execute(const std::vector<std::string>& args){
    
}