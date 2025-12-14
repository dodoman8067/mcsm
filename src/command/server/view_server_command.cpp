/*
Copyright (c) 2023 dodoman8067

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <mcsm/command/server/view_server_command.h>
#include <mcsm/data/options/general_option.h>

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
    std::string path = mcsm::unwrapOrExit(mcsm::getCurrentPath());
    bool fileExists = mcsm::unwrapOrExit(mcsm::fileExists(path + "/server.json"));
    return fileExists;
}

inline void mcsm::ViewServerCommand::printServerInfo(){
    mcsm::ServerConfigLoader sOpt(mcsm::unwrapOrExit(mcsm::getCurrentPath()));

    mcsm::unwrapOrExit(sOpt.loadConfig());

    std::string name, version, type;
    name = mcsm::unwrapOrExit(sOpt.getServerName());

    version = mcsm::unwrapOrExit(sOpt.getServerVersion());

    type = mcsm::unwrapOrExit(sOpt.getServerType());

    mcsm::ServerDataOption* sDataOpt = nullptr;
    if(type == "fabric"){
        sDataOpt = new mcsm::FabricServerDataOption();
    }else{
        sDataOpt = new mcsm::ServerDataOption();
    }

    bool advp = mcsm::GeneralOption::getGeneralOption().advancedParseEnabled();

    mcsm::unwrapOrExit(sDataOpt->load(advp));

    mcsm::info("Server information : ");
    mcsm::info("Server name : " + name);
    mcsm::info("Server version : " + version);
    mcsm::info("Server type : "  + type);

    std::unique_ptr<mcsm::JvmOption> jvmOpt = mcsm::unwrapOrExit(sOpt.getDefaultOption());
    mcsm::info("Server default launch profile : " + jvmOpt->getProfileName());

    std::string timeCreated, timeLaunched;
    timeCreated = mcsm::unwrapOrExit(sDataOpt->getServerTimeCreated());

    timeLaunched = mcsm::unwrapOrExit(sDataOpt->getLastTimeLaunched());

    mcsm::info("Server creation date : " + timeCreated);
    mcsm::info("Last server launch date : " + timeLaunched);
    delete sDataOpt;
    sDataOpt = nullptr;
}
