#include <mcsm/util/cli/console.h>

mcsm::Console::Console(){
    this->running = false;
    this->promptDisplayed = false;
}

mcsm::Console::~Console(){

}

void mcsm::Console::start(){
    this->running = true;

    std::string input;
    while (this->running){
        displayPrompt();
        if(!std::getline(std::cin, input)){
            break;
        }
        handleInput(input);
    }
}

void mcsm::Console::stop(){
    this->running = false;
}

void mcsm::Console::log(const std::string& message){
    std::lock_guard<std::mutex> lock(outputMutex);

    if(this->promptDisplayed){
        std::cout << "\033[K";
        std::cout << "\r";
    }

    std::cout << message << std::endl;

    if(this->promptDisplayed){
        std::cout << "> " << std::flush;
    }
}

void mcsm::Console::input(const std::string& str){
    if(str == "exit"){
        stop();
    }else{
        log("You entered: " + str);
    }
}

void mcsm::Console::displayPrompt(){
    std::lock_guard<std::mutex> lock(outputMutex);
    std::cout << "> " << std::flush;
    this->promptDisplayed = true;
}

void mcsm::Console::handleInput(const std::string& input){
    {
        std::lock_guard<std::mutex> lock(outputMutex);
        this->promptDisplayed = false;
    }
    this->input(input);
}