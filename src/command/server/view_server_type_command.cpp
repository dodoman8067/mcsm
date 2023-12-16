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

#include <mcsm/command/server/view_server_type_command.h>

const std::vector<std::string> availableOptions = {
    "--all",
    "-all",
    "--a",
    "-a",
    "--type",
    "-type",
    "--t",
    "-t"
};

mcsm::ViewServerTypeCommand::ViewServerTypeCommand(const std::string& name, const std::string& description) : Command(name, description) {}

mcsm::ViewServerTypeCommand::~ViewServerTypeCommand(){}

void mcsm::ViewServerTypeCommand::execute(const std::vector<std::string>& args){

}

inline void mcsm::ViewServerTypeCommand::printServerTypeInfo(){

}

std::shared_ptr<mcsm::Server> mcsm::ViewServerTypeCommand::getServer(const std::vector<std::string>& args) const {

}