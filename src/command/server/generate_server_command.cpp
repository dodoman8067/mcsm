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

#include <mcsm/command/server/generate_server_command.h>

const std::vector<std::string> availableOptions = {
    "-servertype",
    "--servertype",
    "-st",
    "--st",
    "--profile",
    "-profile",
    "-p",
    "--p",
    "--name",
    "-name",
    "--current",
    "-current",
    "--c",
    "-c",
    "--global",
    "-global",
    "-g",
    "--g",
    "--jarpath",
    "-jarpath",
    "--jp",
    "-jp",
    "--serverversion",
    "-serverversion",
    "--sversion",
    "-sversion",
    "--sver",
    "-sver",
    "--sv",
    "-sv",
    "--version",
    "-version",
    "--ver",
    "-ver",
    "--v",
    "-v",
    "--no-auto-update",
    "-no-auto-update",
    "--no-auto-updates",
    "-no-auto-updates",
    "--skip-auto-update",
    "-skip-auto-update",
    "--skip-auto-updates",
    "-skip-auto-updates"
};

mcsm::GenerateServerCommand::GenerateServerCommand(const std::string& name, const std::string& description) : Command(name, description) {}

mcsm::GenerateServerCommand::~GenerateServerCommand(){}

void mcsm::GenerateServerCommand::execute(const std::vector<std::string>& args){
    if(args.empty()){
        mcsm::warning("Name not provided; specify a name with --name option to continue.");
        std::exit(1);
    }
    if(isConfigured()){
        mcsm::warning("Server is already configured in this directory.");
        mcsm::warning("Please try again in other directories.");
        std::exit(1);
    }
    detectServer(args);
}

std::string mcsm::GenerateServerCommand::getProfileName(const std::vector<std::string>& args) const {
    std::string name;
    for(size_t i = 0; i < args.size(); ++i){
        const std::string& arg = args[i];
        if(std::find(availableOptions.begin(), availableOptions.end(), arg) != availableOptions.end()){
            if(!(arg == "-profile" || arg == "--profile" || arg == "-p" || arg == "--p")) continue;
            if(i + 1 < args.size() && !args[i + 1].empty() && args[i + 1][0] != '-') {
                name = args[i + 1];
                return mcsm::safeString(name);
            }
        }
    }
    mcsm::warning("Profile name not provided; specify a profile name with --profile option to coutinue.");
    std::exit(1);
}

std::string mcsm::GenerateServerCommand::getServerName(const std::vector<std::string>& args) const {
    std::string name;
    for(size_t i = 0; i < args.size(); ++i){
        const std::string& arg = args[i];
        if(std::find(availableOptions.begin(), availableOptions.end(), arg) != availableOptions.end()){
            if(!(arg == "-name" || arg == "--name")) continue;
            if(i + 1 < args.size() && !args[i + 1].empty() && args[i + 1][0] != '-') {
                name = args[i + 1];
                return mcsm::safeString(name);
            }
        }
    }
    mcsm::warning("Server name not provided; specify a name with --name option to continue.");
    std::exit(1);
}

std::string mcsm::GenerateServerCommand::getServerVersion(const std::vector<std::string>& args) const {
    std::string ver;
    for(size_t i = 0; i < args.size(); ++i){
        const std::string& arg = args[i];
        if(std::find(availableOptions.begin(), availableOptions.end(), arg) != availableOptions.end()){
            if(!(arg == "-version" || arg == "--version" || arg == "-ver" || arg == "--ver" || arg == "-v" || arg == "--v" || arg == "-serverversion" || arg == "--serverversion" || arg == "-sversion" || arg == "--sversion" || arg == "-sver" || arg == "--sver" || arg == "-sv" || arg == "--sv")) continue;
            if(i + 1 < args.size() && !args[i + 1].empty() && args[i + 1][0] != '-') {
                ver = args[i + 1];
                return mcsm::safeString(ver);
            }
        }
    }
    mcsm::warning("Server version not provided; specify a version with --version option to coutinue.");
    std::exit(1);
}

std::unique_ptr<mcsm::JvmOption> mcsm::GenerateServerCommand::searchOption(const mcsm::SearchTarget& target, const std::string& name){
    if(target == mcsm::SearchTarget::GLOBAL || target == mcsm::SearchTarget::ALL){
        std::unique_ptr<mcsm::JvmOption> opt = std::make_unique<mcsm::JvmOption>(name, mcsm::SearchTarget::GLOBAL);
        bool profileExists = opt->exists();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
            mcsm::printResultMessage();
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_WARN_NOEXIT) std::exit(1);
        }
        if(profileExists) return opt;
    }
    if(target == mcsm::SearchTarget::CURRENT || target == mcsm::SearchTarget::ALL){
        std::unique_ptr<mcsm::JvmOption> opt = std::make_unique<mcsm::JvmOption>(name, mcsm::SearchTarget::CURRENT);
        bool profileExists = opt->exists();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
            mcsm::printResultMessage();
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_WARN_NOEXIT) std::exit(1);
        }
        if(profileExists) return opt;
    }
    return nullptr;
}

mcsm::SearchTarget mcsm::GenerateServerCommand::getSearchTarget(const std::vector<std::string>& args) const {
    if(args.empty()) return mcsm::SearchTarget::ALL;
    for(const std::string& arg : args) {
        if(arg == "--global" || arg == "-global" || arg == "--g" || arg == "-g") return mcsm::SearchTarget::GLOBAL;
        if(arg == "--current" || arg == "-current" || arg == "--c" || arg == "-c") return mcsm::SearchTarget::CURRENT;
    }
    return mcsm::SearchTarget::ALL;
}

std::string mcsm::GenerateServerCommand::getServerType(const std::vector<std::string>& args) const {
    std::string type;
    for(size_t i = 0; i < args.size(); ++i){
        const std::string& arg = args[i];
        if(std::find(availableOptions.begin(), availableOptions.end(), arg) != availableOptions.end()){
            if(!(arg == "-servertype" || arg == "--servertype" || arg == "-st" || arg == "--st")) continue;
            if(i + 1 < args.size() && !args[i + 1].empty() && args[i + 1][0] != '-') {
                type = args[i + 1];
                return mcsm::safeString(type);
            }
        }
    }
    mcsm::warning("Server type not provided; specify a type with --servertype option to continue.");
    std::exit(1);
}

bool mcsm::GenerateServerCommand::shouldSkipAutoUpdate(const std::vector<std::string>& args) const {
    for(size_t i = 0; i < args.size(); ++i){
        const std::string& arg = args[i];
        if(std::find(availableOptions.begin(), availableOptions.end(), arg) != availableOptions.end()){
            if(!(arg == "-no-auto-update" || arg == "-no-auto-updates" || arg == "--no-auto-update" || arg == "--no-auto-updates"
            || arg == "-skip-auto-update" || arg == "-skip-auto-updates" || arg == "--skip-auto-update" || arg == "--skip-auto-updates")) continue;
            return true;
        }
    }
    return false;
}

void mcsm::GenerateServerCommand::detectServer(const std::vector<std::string>& args){
    mcsm::SearchTarget target = getSearchTarget(args);
    std::unique_ptr<mcsm::JvmOption> option = searchOption(target, getProfileName(args));
    std::string version = getServerVersion(args);
    std::string name = getServerName(args);
    std::string type = getServerType(args);
    bool shouldSkipUpdate = !shouldSkipAutoUpdate(args);

    if(option == nullptr){
        mcsm::warning("JVM launch profile " + getProfileName(args) + " not found.");
        mcsm::warning("Maybe you have misspelled --c or --g option.");
        std::exit(1);
    }

    auto sPtr = mcsm::ServerRegistry::getServerRegistry().getServer(type);
    if((mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) || sPtr == nullptr){
        mcsm::printResultMessage();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_WARN_NOEXIT) std::exit(1);
    }

    mcsm::Result genRes = sPtr->generate(name, *option, mcsm::getCurrentPath(), version, shouldSkipUpdate);

    if(!genRes.isSuccess()){
        genRes.printMessage();
        if(genRes.getResult() != mcsm::ResultType::MCSM_WARN_NOEXIT) std::exit(1);
    }
    return;
    
    mcsm::error("Server type not supported : " + type);
    std::exit(1);
}

inline bool mcsm::GenerateServerCommand::isConfigured(){
    std::string path = mcsm::getCurrentPath();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        mcsm::printResultMessage();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_WARN_NOEXIT) std::exit(1);
    }
    bool fileExists = mcsm::fileExists(path + "/server.json");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        mcsm::printResultMessage();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_WARN_NOEXIT) std::exit(1);
    }
    return fileExists;
}