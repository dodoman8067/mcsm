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
    "-servertype",
    "--servertype",
    "-st",
    "--st",
    "--type",
    "-type",
    "--t",
    "-t"
};

mcsm::ViewServerTypeCommand::ViewServerTypeCommand(const std::string& name, const std::string& description) : Command(name, description) {}

mcsm::ViewServerTypeCommand::~ViewServerTypeCommand(){}

void mcsm::ViewServerTypeCommand::execute(const std::vector<std::string>& args){
    printServerTypeInfo(args);
}

inline void mcsm::ViewServerTypeCommand::printServerTypeInfo(const std::vector<std::string>& args){
    std::shared_ptr<mcsm::Server> server = getServer(args);
    mcsm::info("Server implementation info : ");
    mcsm::info("Implementation's name : " + server->getTypeAsString());
    mcsm::info("Implementation is based on : " + server->getBasedServer());
    mcsm::info("Implementation's supported versions : " + server->getSupportedVersions());
    mcsm::info("Implementation's website : " + server->getWebSite());
    mcsm::info("Implementation's GitHub repository : " + server->getGitHub());
}

inline std::shared_ptr<mcsm::Server> mcsm::ViewServerTypeCommand::getServer(const std::vector<std::string>& args) const {
    for(size_t i = 0; i < args.size(); ++i){
        const std::string& arg = args[i];
        if(std::find(availableOptions.begin(), availableOptions.end(), arg) != availableOptions.end()){
            if(!(arg == "-type" || arg == "--type" || arg == "-t" || arg == "--t" || arg == "-servertype" || arg == "--servertype" || arg == "-st" || "--st")) continue;
            if(i + 1 < args.size() && !args[i + 1].empty() && args[i + 1][0] != '-'){
                std::string server = args[i + 1];
                return mcsm::server::detectServerType(server);
            }
        }
    }
    mcsm::warning("Server type not provided; Specify a type with --type option to continue.");
    std::exit(1);
}