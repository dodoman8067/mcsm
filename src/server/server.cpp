#include <mcsm/server/server.h>

void mcsm::Server::start(mcsm::JvmOption& option){
    std::string jvmOpt = " ";
    mcsm::ServerOption sOpt;
    for(auto& s : option.getJvmArguments()){
        jvmOpt = jvmOpt + s + " ";
    }

    std::string svrOpt = " ";
    for(auto& s : option.getServerArguments()){
        svrOpt = svrOpt + s + " ";
    }

    std::string command;
    if(sOpt.exists()){
        command = option.getJvmPath() + jvmOpt + sOpt.getServerJarName() + svrOpt;
    }else{
        command = option.getJvmPath() + jvmOpt + this->getDefaultJarFileName() + svrOpt;
    }
    mcsm::info("Running command : " + command);
    mcsm::runCommand(command.c_str());
}