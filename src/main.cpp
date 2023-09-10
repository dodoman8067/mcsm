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

#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include "util/curl_stuff.h"
#include "command/command_manager.h"
#include "command/command.h"
#include "command/test_command.h"
#include "command/version_command.h"
#include "command/help_command.h"
#include "command/generate_server_command.h"
#include "data/option.h"

const std::string version = "0.0.1";

void initCommands(CURL* curl);
void clearCurl(CURL* curl);

int main(int argc, char *argv[]) {

    bool commandFound = false;

    curl_global_init(CURL_GLOBAL_DEFAULT);

    CURL* curl = curl_easy_init();
    //Checks if curl is properly initialized
    //If not, prints error message and exits
    if (curl == nullptr)
    {
        std::cerr << "Error: initializing cURL failed.\n";
        std::cerr << "Possible solutions are : \n";
        std::cerr << "\n";
        std::cerr << "1.Try re-running the program.\n";
        std::cerr << "2.Try rebooting the PC.\n";
        std::cerr << "3.Reinstall the libcurl.dll\n";
        std::cerr << "4.Reinstall the entire program.\n";
        std::cerr << "\n";
        std::cerr << "If none of these work, you may find the solution on the internet.\n";
        std::exit(1);
        return 1;
    }
    initCommands(curl);

    //Prints default message when no arguments
    if(argc < 2){
        std::cout << "Welcome to MCSM (Minecraft Server Manager).\n";
        std::cout << "Type \'mcsm help\' for list of commands.\n"; 
        clearCurl(curl);
        std::exit(0);
        return 0;
    }

    //If arguments exist, iterates through CommandManager::getCommands()
    for(auto& v : mcsm::CommandManager::getCommands()){
        if(std::string(argv[1]) == v.get()->getName()){
            std::vector<std::string> args;
            for(int i = 2; i < argc; i++){
                args.push_back(std::string(argv[i]));
            }
            v.get()->execute(args);
            commandFound = true;
            break;
        }
    }

    //If arguments exist but command is not found, prints message and exits
    if(!commandFound){
        std::cout << "Unknown command \"" << argv[1] << "\". " << "Type \'mcsm help\' for list of commands.\n";
        clearCurl(curl);
        std::exit(0);
        return 0;
    }
    clearCurl(curl);
    
    std::exit(0);
    return 0;
}

void initCommands(CURL* curl){
    //Initializes CommandManager class
    mcsm::CommandManager::init();

    //Adds command to CommandManager
    std::unique_ptr<mcsm::TestCommand> testCommand = std::make_unique<mcsm::TestCommand>("test", "hello");
    mcsm::CommandManager::addCommand(std::move(testCommand));

    std::unique_ptr<mcsm::VersionCommand> versionCommand = std::make_unique<mcsm::VersionCommand>("version", "Returns version information about this program.", version);
    mcsm::CommandManager::addCommand(std::move(versionCommand));

    std::unique_ptr<mcsm::HelpCommand> helpCommand = std::make_unique<mcsm::HelpCommand>("help", "Shows full list of commands.");
    mcsm::CommandManager::addCommand(std::move(helpCommand));

    std::unique_ptr<mcsm::GenerateServerCommand> generateServerCommand = std::make_unique<mcsm::GenerateServerCommand>("create", "Generates server.");
    mcsm::CommandManager::addCommand(std::move(generateServerCommand));
}

void clearCurl(CURL* curl){
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    curl = nullptr;
}
