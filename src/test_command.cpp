#include "test_command.h"

mcsm::TestCommand::TestCommand(const std::string& name, const std::string& description) : mcsm::Command(name, description) {};

mcsm::TestCommand::~TestCommand() {

}

void mcsm::TestCommand::execute(const std::vector<std::string>& args){
    if(args.size() < 1){
        std::cout << "args is empty" << std::endl;
    }else{
        for(const std::string& s : args){
            std::cout << s << std::endl;
        }
    }
    std::cout << this->getDescription() << std::endl;
}