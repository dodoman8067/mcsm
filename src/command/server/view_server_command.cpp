#include <mcsm/command/server/view_server_command.h>

mcsm::ViewServerCommand::ViewServerCommand(const std::string& name, const std::string& description) : mcsm::Command(name, description) {}

mcsm::ViewServerCommand::~ViewServerCommand() {}

void mcsm::ViewServerCommand::execute(const std::vector<std::string>& /* args */){
    if(!isConfigured()){
        mcsm::warning("Server not configured.");
        mcsm::warning("Task aborted.");
        std::exit(1);
    }
    printServerInfo();
}

inline bool mcsm::ViewServerCommand::isConfigured(){
    return std::filesystem::exists("server.json");
}

inline void mcsm::ViewServerCommand::printServerInfo(){
    mcsm::ServerOption sOpt;
    mcsm::info("Server information : ");
    mcsm::info("Server name : " + sOpt.getServerName());
    mcsm::info("Server version : " + sOpt.getServerVersion());
    mcsm::info("Server type : "  + sOpt.getServerType());

    std::unique_ptr<mcsm::JvmOption> jvmOpt = sOpt.getDefaultOption();
    mcsm::info("Server default launch profile : " + jvmOpt->getProfileName());
}
