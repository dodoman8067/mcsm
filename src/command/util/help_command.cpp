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

#include <mcsm/command/util/help_command.h>

mcsm::HelpCommand::HelpCommand(const std::string& name, const std::string& description) : mcsm::Command(name, description) {};

mcsm::HelpCommand::~HelpCommand(){};

void mcsm::HelpCommand::execute(const std::vector<std::string>& args){
    if(args.empty()){
        std::cout << "List of commands : " << std::endl;
        for(auto& command : mcsm::CommandManager::getCommands()){
            std::cout << "    " << command->getName() << " - " << command->getDescription() << "\n";
        }
    }else{
        if(!mcsm::CommandManager::hasCommand(args[0])){
            std::cout << "Unknown command \"" << args[0] << "\". Type 'mcsm help' for list of commands." << "\n";
            std::exit(1);
        }
        std::unique_ptr<mcsm::Command> command = mcsm::CommandManager::getCommand(args[0]);
        std::cout << command->getName() << " - " << command->getDescription() << "\n";
    }
}