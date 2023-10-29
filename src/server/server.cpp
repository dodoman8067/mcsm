#include <mcsm/server/server.h>

void mcsm::Server::start(std::unique_ptr<mcsm::JvmOption> option){
    std::string jvmOpt = " ";
    for(auto& s : option->getJvmArguments()){
        jvmOpt = jvmOpt + s + " ";
    }

    std::string svrOpt = " ";
    for(auto& s : option->getServerArguments()){
        svrOpt = svrOpt + s + " ";
    }

    std::string command = option->getJvmPath() + jvmOpt + " " + this->getJarFile() + " " + svrOpt;
    mcsm::runCommand(command.c_str());
}