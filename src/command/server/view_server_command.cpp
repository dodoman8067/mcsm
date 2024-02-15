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
    return mcsm::fileExists(mcsm::getCurrentPath() + "/server.json");
}

inline void mcsm::ViewServerCommand::printServerInfo(){
    mcsm::ServerOption sOpt;
    mcsm::info("Server information : ");
    mcsm::info("Server name : " + sOpt.getServerName());
    mcsm::info("Server version : " + sOpt.getServerVersion());
    mcsm::info("Server type : "  + sOpt.getServerType());

    std::unique_ptr<mcsm::JvmOption> jvmOpt = sOpt.getDefaultOption();
    mcsm::info("Server default launch profile : " + jvmOpt->getProfileName());

    mcsm::ServerDataOption* sDataOpt;
    if(sOpt.getServerType() == "fabric"){
        //if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK) { mcsm::printMessage(); std::exit(1); }
        sDataOpt = new mcsm::FabricServerDataOption();
    }else{
        sDataOpt = new mcsm::ServerDataOption();
    }
    mcsm::info("Server creation date : " + sDataOpt->getServerTimeCreated());
    mcsm::info("Last server launch date : " + sDataOpt->getLastTimeLaunched());
    delete sDataOpt;
    sDataOpt = nullptr;
}
