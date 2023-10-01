#include "jvm_test_command.h"

mcsm::JvmTestCommand::JvmTestCommand(const std::string& name, const std::string& description) : mcsm::Command(name, description) {}

mcsm::JvmTestCommand::~JvmTestCommand(){}

void mcsm::JvmTestCommand::execute(const std::vector<std::string>& args){

}

std::unique_ptr<mcsm::JvmOption> mcsm::JvmTestCommand::searchOption(const mcsm::SearchTarget& target){
    
}