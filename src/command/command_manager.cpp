/*
Copyright (c) 2023 dodoman8067

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <mcsm/command/command_manager.h>

std::unique_ptr<std::vector<std::unique_ptr<mcsm::Command>>> mcsm::CommandManager::commands;
bool mcsm::CommandManager::initialized;

mcsm::CommandManager::CommandManager(){
    initialized = false;
}

mcsm::CommandManager::~CommandManager(){
    
}

void mcsm::CommandManager::init(){
    if(initialized){
        std::cerr << "Error: Program's command manager was initialized more than once.\n";
        std::cerr << "You're not supposed to see this message unless you're using development version.\n";
        std::cerr << "If you see this message, this might be a software issue or the file is corrupted.\n";
        std::cerr << "If you think this is a software issue, please open a issue to github https://github.com/dodoman8067/mcsm .\n";
        std::cerr << "Otherwise, reinstall the entire program.\n";
        std::exit(1);
    }
    commands = std::make_unique<std::vector<std::unique_ptr<mcsm::Command>>>();
    commands->shrink_to_fit();
    initialized = true;
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
        if(command->getName() != name && !command->hasAliases(name)) continue;
        return true;
    }
    return false;
}

bool mcsm::CommandManager::hasAliases(const std::string& command, const std::string& value){
    if(!hasCommand(command)) return false;
    std::unique_ptr<mcsm::Command> commandInstance = getCommand(command);
    return commandInstance->hasAliases(value);
}

bool mcsm::CommandManager::hasAliasesInGlobal(const std::string& value){
    for(std::unique_ptr<mcsm::Command>& command : mcsm::CommandManager::getCommands()){
        if(command->hasAliases(value)){
            return true;
        }
    }
    return false;
}

std::unique_ptr<mcsm::Command> mcsm::CommandManager::getCommand(const std::string& name){
    for(auto& command : *commands){
        if(command->getName() != name && !command->hasAliases(name)) continue;
        return std::move(command);
    }
    return nullptr;
}