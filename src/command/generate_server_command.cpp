#include "generate_server_command.h"

mcsm::GenerateServerCommand::GenerateServerCommand(const std::string& name, const std::string& description) : Command(name, description) {}

mcsm::GenerateServerCommand::~GenerateServerCommand(){}

void mcsm::GenerateServerCommand::execute(const std::vector<std::string>& args){
    if(args.size() <= 0) {
        std::string input;
        while(true){
            std::cout << "Type a string: ";
            std::getline(std::cin, input);
            
            if(!input.empty()){
                std::cout << input << std::endl;
                break;
            }else{
                std::cout << "Wrong input" << std::endl;
            }
        }
    }else{
        std::cout << "This command is not quite ready yet! :)" << std::endl;
    }
}