#include "command_manager.h"

std::unique_ptr<std::vector<std::unique_ptr<mcsm::Command>>> mcsm::CommandManager::commands;

mcsm::CommandManager::CommandManager(){
}

mcsm::CommandManager::~CommandManager(){
    
}

void mcsm::CommandManager::init(){
    commands = std::make_unique<std::vector<std::unique_ptr<mcsm::Command>>>();
    commands->shrink_to_fit();
}

void mcsm::CommandManager::addCommand(std::unique_ptr<mcsm::Command> command){
    if(hasCommand(command->getName())) return;
   commands->push_back(std::move(command));
}

std::vector<std::unique_ptr<mcsm::Command>>& mcsm::CommandManager::getCommands(){
    return *commands;
}

bool mcsm::CommandManager::hasCommand(const std::string& name){
    for(auto& command : *commands){
        if(command->getName() != name) continue;
        return true;
    }
    return false;
}

std::unique_ptr<mcsm::Command> mcsm::CommandManager::getCommand(const std::string& name){
    for(auto& command : *commands){
        if(command->getName() != name) continue;
        return std::move(command);
    }
    return nullptr;
}