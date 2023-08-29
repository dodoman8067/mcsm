#include "command_manager.h"

std::unique_ptr<std::vector<std::unique_ptr<mcsm::Command>>> mcsm::CommandManager::commands;

mcsm::CommandManager::CommandManager(){
}

mcsm::CommandManager::~CommandManager(){
    
}

void mcsm::CommandManager::init(){
    commands = std::make_unique<std::vector<std::unique_ptr<mcsm::Command>>>();
}

void mcsm::CommandManager::addCommand(std::unique_ptr<mcsm::Command> command){
   commands->push_back(std::move(command));
}

std::vector<std::unique_ptr<mcsm::Command>>& mcsm::CommandManager::getCommands(){
    return *commands;
}