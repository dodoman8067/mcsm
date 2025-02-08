#include <mcsm/command/server/group/subcommands/init.h>

mcsm::GroupInitSubCommand::GroupInitSubCommand(mcsm::ServerGroupGenerator* loader){
    if(loader){
        this->loader = loader;
    }
}

mcsm::GroupInitSubCommand::~GroupInitSubCommand(){

}

void mcsm::GroupInitSubCommand::execute(const std::vector<std::string>& args){
    
}
