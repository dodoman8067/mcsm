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

#include <mcsm/command/server/start_server_command.h>

const std::vector<std::string> availableOptions = {
    "--profile",
    "-profile",
    "-p",
    "--p",
    "--current",
    "-current",
    "--c",
    "-c",
    "--global",
    "-global",
    "-g",
    "--g"
};

mcsm::StartServerCommand::StartServerCommand(const std::string& name, const std::string& description) : mcsm::Command(name, description) {}

mcsm::StartServerCommand::~StartServerCommand() {}

void mcsm::StartServerCommand::execute(const std::vector<std::string>& args){
    if(!isConfigured()){
        mcsm::warning("Server not configured.");
        mcsm::warning("Task aborted.");
        std::exit(1);
    }
    std::unique_ptr<mcsm::JvmOption> jvmOpt = searchOption(args);
    mcsm::ServerOption sOpt;
    sOpt.start(std::move(jvmOpt));
}

std::unique_ptr<mcsm::JvmOption> mcsm::StartServerCommand::searchOption(const std::vector<std::string>& args){
    if(!isConfigured()){
        mcsm::error("Server not configured.");
        mcsm::error("Task aborted.");
        std::exit(1);
    }
    for(size_t i = 0; i < args.size(); ++i){
        const std::string& arg = args[i];
        if(std::find(availableOptions.begin(), availableOptions.end(), arg) != availableOptions.end()){
            if(!(arg == "-profile" || arg == "--profile" || arg == "-p" || arg == "--p")) continue;
            if(i + 1 < args.size() && !args[i + 1].empty() && args[i + 1][0] != '-'){
                std::string pName = args[i + 1];
                mcsm::SearchTarget target = getSearchTarget(args);
                std::unique_ptr<mcsm::JvmOption> profile = searchOption(target, pName);
                if(profile == nullptr){
                    mcsm::error("The specified JVM launch profile " + pName + " does not exist.");
                    std::exit(1);
                }
                mcsm::info("Found specified JVM launch profile " + pName + ".");
                mcsm::info("Location : " + profile->getProfilePath());
                return profile;
            }
        }
    }
    mcsm::info("No JVM launch profile specified; Using default JVM launch profile.");
    mcsm::ServerOption option;
    std::unique_ptr<mcsm::JvmOption> jvmOpt = option.getDefaultOption();
    return jvmOpt;
}

std::unique_ptr<mcsm::JvmOption> mcsm::StartServerCommand::searchOption(const mcsm::SearchTarget& target, const std::string& name){
    if(!isConfigured()){
        mcsm::error("Server not configured.");
        mcsm::error("Task aborted.");
        std::exit(1);
    }
    if(target == mcsm::SearchTarget::GLOBAL || target == mcsm::SearchTarget::ALL){
        std::unique_ptr<mcsm::JvmOption> opt = std::make_unique<mcsm::JvmOption>(name, mcsm::SearchTarget::GLOBAL);
        if(opt->exists()) return opt;
    }
    if(target == mcsm::SearchTarget::CURRENT || target == mcsm::SearchTarget::ALL){
        std::unique_ptr<mcsm::JvmOption> opt = std::make_unique<mcsm::JvmOption>(name, mcsm::SearchTarget::CURRENT);
        if(opt->exists()) return opt;
    }
    return nullptr;
}

mcsm::SearchTarget mcsm::StartServerCommand::getSearchTarget(const std::vector<std::string>& args){
    if(args.empty()) return mcsm::SearchTarget::ALL;
    for(const std::string& arg : args) {
        if(arg == "--global" || arg == "-global" || arg == "--g" || arg == "-g") return mcsm::SearchTarget::GLOBAL;
        if(arg == "--current" || arg == "-current" || arg == "--c" || arg == "-c") return mcsm::SearchTarget::CURRENT;
    }
    return mcsm::SearchTarget::ALL;
}


inline bool mcsm::StartServerCommand::isConfigured(){
    return std::filesystem::exists("server.json");
}