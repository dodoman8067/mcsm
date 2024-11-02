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
        std::string_view arg = args[i];
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
        std::string_view arg = args[i];
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
        std::string_view arg = args[i];
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

mcsm::SearchTarget mcsm::GenerateServerCommand::getSearchTarget(const std::string& value) const {
    if(value == "global") return mcsm::SearchTarget::GLOBAL;
    if(value == "current") return mcsm::SearchTarget::CURRENT;
    return mcsm::SearchTarget::ALL;
}

std::string mcsm::GenerateServerCommand::getServerType(const std::vector<std::string>& args) const {
    std::string type;
    for(size_t i = 0; i < args.size(); ++i){
        std::string_view arg = args[i];
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
        std::string_view arg = args[i];
        if(std::find(availableOptions.begin(), availableOptions.end(), arg) != availableOptions.end()){
            if(!(arg == "-no-auto-update" || arg == "-no-auto-updates" || arg == "--no-auto-update" || arg == "--no-auto-updates"
            || arg == "-skip-auto-update" || arg == "-skip-auto-updates" || arg == "--skip-auto-update" || arg == "--skip-auto-updates")) continue;
            return true;
        }
    }
    return false;
}

bool mcsm::GenerateServerCommand::checkValid(const std::string& key, std::string& value, const std::string& defaultValue){
    if(key == "default JVM launch profile search path (current/global)"){
        if(mcsm::isWhitespaceOrEmpty(value)){
            value = defaultValue;
            return true;
        }
        return !(value != "global" && value != "current");
    }
    if(key == "if server should update the server jarfile automatically"){
        if(mcsm::isWhitespaceOrEmpty(value)){
            value = defaultValue;
            return true;
        }
        return !(value != "true" && value != "false");
    }
    if(key == "server jarfile name"){
        if(mcsm::isWhitespaceOrEmpty(value)){
            value = defaultValue;
            return true;
        }
        return !(value != "true" && value != "false");
    }
    if(key == "server build version"){
        if(mcsm::isWhitespaceOrEmpty(value)){
            value = defaultValue;
        }
        return true;
    }
    return !mcsm::isWhitespaceOrEmpty(value);
}

void mcsm::GenerateServerCommand::handle(const std::string& key, std::map<std::string, std::string>& extras, const std::string& defaultValue){
    std::string typedInput;
    while(true){
        if(!mcsm::isWhitespaceOrEmpty(defaultValue)){
            std::cout << "Enter " << key << "(default : " << defaultValue << ") : ";
        }else{
            std::cout << "Enter " << key << " : ";
        }
        
        std::getline(std::cin, typedInput);
            
        if(checkValid(key, typedInput, defaultValue)){
            extras[key] = typedInput;
            break;
        }else{
            std::cout << "Invalid input \"" << typedInput << "\". Please try again.\n";
        }
    }
}

void mcsm::GenerateServerCommand::detectServer(const std::vector<std::string>& /* args */){
    std::string type;
    while(true){
        std::cout << "Enter server type : ";
        
        std::getline(std::cin, type);
            
        if(mcsm::ServerRegistry::getServerRegistry().getServer(type) != nullptr){
            break;
        }else{
            mcsm::Result({mcsm::ResultType::MCSM_SUCCESS, {"Success"}}); // clear the result
            std::cout << "Server type \"" << type << "\" does not exist. Please try again.\n";
        }
    }

    std::map<std::string, std::string> extras;

    auto sPtr = mcsm::ServerRegistry::getServerRegistry().getServer(type);
    if((mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) || sPtr == nullptr){
        mcsm::printResultMessage();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_WARN_NOEXIT) std::exit(1);
    }

    for(auto&[name, defaultValue] : sPtr->getRequiredValues()){
        handle(name, extras, defaultValue);
    }

    std::string name = extras["name"];
    std::string version = extras["Minecraft version"];
    std::string update = extras["if server should update the server jarfile automatically"];
    bool bUpdate = false;
    if(!mcsm::isWhitespaceOrEmpty(update)){
        bUpdate = update == "true" ? true : false;
    }

    mcsm::SearchTarget t = getSearchTarget(extras["default JVM launch profile search path (current/global)"]);
    if(t == mcsm::SearchTarget::ALL){
        mcsm::error("default JVM launch profile search path (current/global) invalid value detected");
        std::exit(1);
    }
 
    mcsm::JvmOption defaultProfile(extras["default JVM launch profile name"], t);
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        mcsm::printResultMessage();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_WARN_NOEXIT) std::exit(1);
    }


    mcsm::Result genRes = sPtr->generate(name, defaultProfile, mcsm::getCurrentPath(), version, bUpdate, extras);

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