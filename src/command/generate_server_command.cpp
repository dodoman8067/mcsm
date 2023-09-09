#include "generate_server_command.h"

mcsm::GenerateServerCommand::GenerateServerCommand(const std::string& name, const std::string& description) : Command(name, description) {}

mcsm::GenerateServerCommand::~GenerateServerCommand(){}

void mcsm::GenerateServerCommand::execute(const std::vector<std::string>& args){
    if(args.size() <= 0) {
        std::string input;
        std::vector<std::string>* description = new std::vector<std::string>();
        description->push_back("Type a input.");
        description->push_back("");
        description->push_back("1.paper");
        description->push_back("2.spigot");
        description->push_back("3.bukkit");
        delete description;
    }else{
        std::cout << "This command is not quite ready yet! :)" << std::endl;
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