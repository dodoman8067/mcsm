#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include "curl_stuff.h"
#include "command_manager.h"
#include "command.h"
#include "test_command.h"
#include "version_command.h"
#include "help_command.h"

const std::string version = "0.0.1";

void initCommands(CURL* curl);

int main(int argc, char *argv[]) {

    bool commandFound = false;

    curl_global_init(CURL_GLOBAL_DEFAULT);

    CURL* curl = curl_easy_init();
    initCommands(curl);
    if (curl == nullptr)
    {
        std::cerr << "Error: initializing cURL failed." << std::endl;
        std::cerr << "Possible solutions are : " << std::endl;
        std::cerr << " " << std::endl;
        std::cerr << "1.Try re-running the program." << std::endl;
        std::cerr << "2.Try rebooting the PC." << std::endl;
        std::cerr << "3.Reinstall the libcurl.dll" << std::endl;
        std::cerr << "4.Reinstall the entire program." << std::endl;
        std::cerr << " " << std::endl;
        std::cerr << "If none of these work, you may find the solution on the internet." << std::endl;
        std::exit(1);
        return 1;
    }

    if(argc < 2){
        std::cout << "Welcome to MCSM (Minecraft Server Manager)." << std::endl;
        std::cout << "Type \'mcsm help\' for list of commands." << std::endl; 
        std::exit(0);
    }

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

    if(!commandFound){
        std::cout << "Unknown command \"" << argv[1] << "\". " << "Type \'mcsm help\' for list of commands." << std::endl;
        std::exit(0); 
    }

    curl_easy_cleanup(curl);
    curl_global_cleanup();
    
    std::exit(0);
    return 0;
}

void initCommands(CURL* curl){
    mcsm::CommandManager::init();

    std::unique_ptr<mcsm::TestCommand> testCommand = std::make_unique<mcsm::TestCommand>("test", "hello");
    mcsm::CommandManager::addCommand(std::move(testCommand));

    std::unique_ptr<mcsm::VersionCommand> versionCommand = std::make_unique<mcsm::VersionCommand>("version", "Returns version information about this program.", version);
    mcsm::CommandManager::addCommand(std::move(versionCommand));

    std::unique_ptr<mcsm::HelpCommand> helpCommand = std::make_unique<mcsm::HelpCommand>("help", "Shows full list of commands.");
    mcsm::CommandManager::addCommand(std::move(helpCommand));
}
