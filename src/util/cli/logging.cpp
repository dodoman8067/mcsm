#include <mcsm/util/cli/logging.h>

void mcsm::info(const std::string& message){
    std::cout << "[mcsm/INFO] " << message << "\n";
}

void mcsm::success(const std::string& message){
    std::cout << termcolor::bright_green << "[mcsm/INFO] " << message << termcolor::reset << "\n";
}

void mcsm::warning(const std::string& message){
    std::cout << termcolor::bright_yellow << "[mcsm/WARN] " << message << termcolor::reset << "\n";
}

void mcsm::error(const std::string& message){
    std::cerr << termcolor::bright_red << "[mcsm/ERROR] " << message << termcolor::reset << "\n";
}