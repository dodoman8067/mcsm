#include "get_server_command.h"

mcsm::GetServerCommand::GetServerCommand(const std::string& name, const std::string& description) : mcsm::Command(name, description) {};

mcsm::GetServerCommand::~GetServerCommand() {

}

void mcsm::GetServerCommand::execute(const std::vector<std::string>& args){
    if(args.size() < 1){
        std::cout << "args is empty" << std::endl;
    }else{
        for(const std::string& s : args){
            std::cout << s << std::endl;
        }
    }
    std::cout << this->getDescription() << std::endl;
}