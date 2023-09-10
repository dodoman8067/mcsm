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