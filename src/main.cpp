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
#include <mcsm/command/command_manager.h>
#include <mcsm/command/base/command.h>
#include <mcsm/command/util/version_command.h>
#include <mcsm/command/util/help_command.h>
#include <mcsm/command/server/generate_server_command.h>
#include <mcsm/command/server/jvm/jvm_option_generator_command.h>
#include <mcsm/command/server/jvm/jvm_option_search_command.h>
#include <mcsm/command/server/jvm/jvm_test_command.h>
#include <mcsm/data/option.h>
#include <mcsm/data/options/jvm_option.h>
#include <mcsm/data/options/server_option.h>
#include <mcsm/server/type/bukkit/paper_server.h>
#include <mcsm/util/string_utils.h>
#include <mcsm/jvm/java_detection.h>

const std::string version = "0.0.1";

inline void initCommands();

int main(int argc, char *argv[]) {

    bool commandFound = false;

    initCommands();

    //Prints default message when no arguments
    if(argc < 2){
        std::cout << "Welcome to MCSM (Minecraft Server Manager).\n";
        std::cout << "Type \'mcsm help\' for list of commands.\n";
        std::shared_ptr<mcsm::PaperServer> server = std::make_shared<mcsm::PaperServer>();
        mcsm::JvmOption jvmOpt("111", mcsm::SearchTarget::CURRENT);
        mcsm::ServerOption option("1.19.4", server);
        if(!option.exists()){
            option.create("example", jvmOpt);
        }
        if(!std::filesystem::exists("paper.jar")){
            server->download(option.getServerVersion());
        }
        option.start();
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
        return 1;
    }
    
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

    std::unique_ptr<mcsm::JvmTestCommand> jvmTestCommand = std::make_unique<mcsm::JvmTestCommand>("performTest", "Tests if launch profile's Java Virtual Machine is valid.");
    jvmTestCommand->addAliases("performtest");
    mcsm::CommandManager::addCommand(std::move(jvmTestCommand));
}
