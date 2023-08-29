#include "command.h"

mcsm::Command::Command(const std::string& name, const std::string& description){
    this->name = name;
    this->description = description;
}

mcsm::Command::~Command(){
}

std::string mcsm::Command::getName() const {
    return this->name;
}

std::string mcsm::Command::getDescription() const {
    return this->description;
}