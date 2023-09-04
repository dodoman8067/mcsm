#include "help_command.h"

mcsm::HelpCommand::HelpCommand(const std::string& name, const std::string& description) : mcsm::Command(name, description) {};

mcsm::HelpCommand::~HelpCommand(){};

void mcsm::HelpCommand::execute(const std::vector<std::string>& args){
    if(args.empty()){
        std::cout << "List of commands : " << std::endl;
        for(auto& command : mcsm::CommandManager::getCommands()){
            std::cout << "    " << command->getName() << " - " << command->getDescription() <<  std::endl;
        }
    }else{
        if(!mcsm::CommandManager::hasCommand(args[0])){
            std::cout << "Unknown command \"" << args[0] << "\". Type 'mcsm help' for list of commands." << std::endl;
            std::exit(0);
        }
        std::unique_ptr<mcsm::Command> command = mcsm::CommandManager::getCommand(args[0]);
        std::cout << command->getName() << " - " << command->getDescription() <<  std::endl;
    }
}