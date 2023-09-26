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

#include "generate_server_command.h"

mcsm::GenerateServerCommand::GenerateServerCommand(const std::string& name, const std::string& description) : Command(name, description) {}

mcsm::GenerateServerCommand::~GenerateServerCommand(){}

void mcsm::GenerateServerCommand::execute(const std::vector<std::string>& args){
    if(args.size() <= 0) {
        std::string input;
        std::string output;
        std::vector<std::string> description = {
            "Type an input.",
            "",
            "1.paper",
            "2.spigot",
            "3.bukkit"
        };

        mcsm::askInput(description, input, output);
        std::cout << input << std::endl;
    }else{
        std::cout << "This command is not quite ready yet! :)" << "\n";
    }
}

void mcsm::GenerateServerCommand::stuff(std::string& value){
    while(true){
        std::cout << "Type a string: ";
        std::getline(std::cin, value);
            
        if(!value.empty()){
            break;
        }else{
            std::cout << "Wrong input" << std::endl;
        }
    }
}