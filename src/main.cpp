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
#include "command/command_manager.h"
#include "command/base/command.h"
#include "command/util/version_command.h"
#include "command/util/help_command.h"
#include "command/server/generate_server_command.h"
#include "command/server/jvm/jvm_option_generator_command.h"
#include "command/server/jvm/jvm_option_search_command.h"
#include "command/server/jvm/jvm_test_command.h"
#include "data/option.h"
#include "data/options/jvm_option.h"
#include "util/string_utils.h"
#include "jvm/java_detection.h"
#include "http/curl_manager.h"
#include "http/get.h"

const std::string version = "0.0.1";

inline void initCommands();

int main(int argc, char *argv[]) {

    bool commandFound = false;

    mcsm::initCurl();
    initCommands();

    //Prints default message when no arguments
    if(argc < 2){
        std::cout << "Welcome to MCSM (Minecraft Server Manager).\n";
        std::cout << "Type \'mcsm help\' for list of commands.\n";
        mcsm::get("https://google.com");
        mcsm::clearCurl();
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
        std::cerr << "Unknown command \"" << argv[1] << "\". " << "Type \'mcsm help\' for list of commands.\n";
        mcsm::clearCurl();
        return 1;
    }
    mcsm::clearCurl();
    
    return 0;
}

inline void initCommands(){
    //Initializes CommandManager class
    mcsm::CommandManager::init();

    //Adds command to CommandManager
    std::unique_ptr<mcsm::VersionCommand> versionCommand = std::make_unique<mcsm::VersionCommand>("version", "Returns version information about this program.", version);
    versionCommand->addAliases("ver");
    mcsm::CommandManager::addCommand(std::move(versionCommand));

    std::unique_ptr<mcsm::HelpCommand> helpCommand = std::make_unique<mcsm::HelpCommand>("help", "Shows full list of commands.");
    mcsm::CommandManager::addCommand(std::move(helpCommand));

    std::unique_ptr<mcsm::GenerateServerCommand> generateServerCommand = std::make_unique<mcsm::GenerateServerCommand>("create", "Generates server.");
    mcsm::CommandManager::addCommand(std::move(generateServerCommand));

    std::unique_ptr<mcsm::JvmOptionGeneratorCommand> jvmOptionGeneratorCommand = std::make_unique<mcsm::JvmOptionGeneratorCommand>("genJvmOption", "Generates Java Virtual Machine config.");
    jvmOptionGeneratorCommand->addAliases("generateJvmOption");
    jvmOptionGeneratorCommand->addAliases("generateJavaVirtualMachineOption");
    jvmOptionGeneratorCommand->addAliases("genjvmoption");
    mcsm::CommandManager::addCommand(std::move(jvmOptionGeneratorCommand));

    std::unique_ptr<mcsm::JvmOptionSearchCommand> jvmProfileSearchCommand = std::make_unique<mcsm::JvmOptionSearchCommand>("searchJvmProfile", "Looks up for Java Virtual Machine profiles.");
    jvmProfileSearchCommand->addAliases("searchProfile");
    jvmProfileSearchCommand->addAliases("searchJavaVirtualMachineProfile");
    jvmProfileSearchCommand->addAliases("searchprofile");
    mcsm::CommandManager::addCommand(std::move(jvmProfileSearchCommand));

    std::unique_ptr<mcsm::JvmTestCommand> jvmTestCommand = std::make_unique<mcsm::JvmTestCommand>("performTest", "Tests Java Virtual Machine is valid.");
    jvmTestCommand->addAliases("performtest");
    mcsm::CommandManager::addCommand(std::move(jvmTestCommand));
}
