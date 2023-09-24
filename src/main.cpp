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

#include <curl/curl.h>
#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include "command/command_manager.h"
#include "command/command.h"
#include "command/test_command.h"
#include "command/version_command.h"
#include "command/help_command.h"
#include "command/generate_server_command.h"
#include "command/jvm_option_generator_command.h"
#include "data/option.h"
#include "data/options/jvm_option.h"
#include "util/string_utils.h"
#include "jvm/java_detection.h"

const std::string version = "0.0.1";

inline void initCommands();
inline void clearCurl(CURL* curl);

int main(int argc, char *argv[]) {

    bool commandFound = false;

    curl_global_init(CURL_VERSION_THREADSAFE);

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
        std::cerr << "3.Try reinstalling the entire program.\n";
        std::cerr << "\n";
        std::cerr << "If none of these work, you may find the solution on the internet.\n";
        std::exit(1);
        return 1;
    }
    initCommands();

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
        if(argv[1] == v.get()->getName() || v.get()->hasAliases(argv[1])){
            std::vector<std::string> args;
            for(int i = 2; i < argc; i++){
                args.push_back(std::string(argv[i]));
            }
            v.get()->execute(std::move(args));
            commandFound = true;
            break;
        }
    }

    //If arguments exist but command is not found, prints message and exits
    if(!commandFound){
        std::cout << "Unknown command \"" << argv[1] << "\". " << "Type \'mcsm help\' for list of commands.\n";
        clearCurl(curl);
        std::exit(1);
        return 1;
    }
    clearCurl(curl);
    
    std::exit(0);
    return 0;
}

inline void initCommands(){
    //Initializes CommandManager class
    mcsm::CommandManager::init();

    //Adds command to CommandManager
    std::unique_ptr<mcsm::TestCommand> testCommand = std::make_unique<mcsm::TestCommand>("test", "hello");
    mcsm::CommandManager::addCommand(std::move(testCommand));

    std::unique_ptr<mcsm::VersionCommand> versionCommand = std::make_unique<mcsm::VersionCommand>("version", "Returns version information about this program.", version);
    versionCommand->addAliases("ver");
    mcsm::CommandManager::addCommand(std::move(versionCommand));

    std::unique_ptr<mcsm::HelpCommand> helpCommand = std::make_unique<mcsm::HelpCommand>("help", "Shows full list of commands.");
    mcsm::CommandManager::addCommand(std::move(helpCommand));

    std::unique_ptr<mcsm::GenerateServerCommand> generateServerCommand = std::make_unique<mcsm::GenerateServerCommand>("create", "Generates server.");
    mcsm::CommandManager::addCommand(std::move(generateServerCommand));

    std::unique_ptr<mcsm::JvmOptionGeneratorCommand> jvmOptionGeneratorCommand = std::make_unique<mcsm::JvmOptionGeneratorCommand>("jvm", "Generates jvm config.");
    mcsm::CommandManager::addCommand(std::move(jvmOptionGeneratorCommand));
}

inline void clearCurl(CURL* curl){
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    curl = nullptr;
}
