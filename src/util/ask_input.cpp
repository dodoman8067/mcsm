#include "ask_input.h"

void mcsm::askInput(std::vector<std::string>& arguments, std::string& input, const std::string &outputWhenNone){
    for(std::string& s : arguments){
        std::cout << s << std::endl;
    }
    std::cout << " " << std::endl;
    while(true){
        std::cout << ">> ";
        std::getline(std::cin, input);
            
        if(!mcsm::isWhitespaceOrEmpty(input)){
            break;
        }else{
            if(!outputWhenNone.empty()){
                input = outputWhenNone;
                break;
            }
            std::cout << "Not a valid input; Please try again." << std::endl;
        }
    }
}