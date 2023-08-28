#include "command_manager.h"

mcsm::CommandManager::CommandManager(CURL* curl){
    this->curl = curl;
    this->commands = std::make_unique<std::vector<std::unique_ptr<mcsm::Command>>>();
}

mcsm::CommandManager::~CommandManager(){
    
}

void mcsm::CommandManager::addCommand(std::unique_ptr<mcsm::Command> command){
    commands->push_back(std::move(command));
}

std::vector<std::unique_ptr<mcsm::Command>>& mcsm::CommandManager::getCommands(){
    return *commands;
}