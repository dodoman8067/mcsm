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

mcsm::SearchTarget mcsm::GenerateServerCommand::getSearchTarget(const std::string& value) const {
    if(value == "global") return mcsm::SearchTarget::GLOBAL;
    if(value == "current") return mcsm::SearchTarget::CURRENT;
    return mcsm::SearchTarget::ALL;
}

bool mcsm::GenerateServerCommand::checkValid(const std::string& key, std::string& value, const std::string& defaultValue){
    if(key == "default_jvm_launch_profile_search_path"){
        if(mcsm::isWhitespaceOrEmpty(value)){
            value = defaultValue;
            return true;
        }
        return !(value != "global" && value != "current");
    }
    if(key == "auto_server_jar_update"){
        if(mcsm::isWhitespaceOrEmpty(value)){
            value = defaultValue;
            return true;
        }
        return !(value != "true" && value != "false");
    }
    if(key == "server_jarfile"){
        if(mcsm::isWhitespaceOrEmpty(value)){
            value = defaultValue;
            return true;
        }
        return true;
    }
    if(key == "server_build_version"){
        if(mcsm::isWhitespaceOrEmpty(value)){
            value = defaultValue;
        }
        return true;
    }
    if(key == "server_installer_version"){
        if(mcsm::isWhitespaceOrEmpty(value)){
            value = defaultValue;
        }
        return true;
    }
    if(key == "server_loader_version"){
        if(mcsm::isWhitespaceOrEmpty(value)){
            value = defaultValue;
        }
        return true;
    }
    if(key == "custom_run_command"){
        if(mcsm::isWhitespaceOrEmpty(value)){
            value = defaultValue;
        }
        return true;
    }
    if(key == "sponge_api_search_recommended_versions"){
        if(mcsm::isWhitespaceOrEmpty(value)){
            value = defaultValue;
            return true;
        }
        return !(value != "true" && value != "false");
    }
    if(key == "if_paper_should_generate_recommended_profile"){
        if(mcsm::isWhitespaceOrEmpty(value)){
            value = defaultValue;
            return true;
        }
        return !(value != "true" && value != "false");
    }

    return !mcsm::isWhitespaceOrEmpty(value);
}

void mcsm::GenerateServerCommand::handle(const std::string& key, std::map<std::string, std::string>& extras, const std::string& defaultValue){
    std::string typedInput;
    while(true){
        if(!mcsm::isWhitespaceOrEmpty(defaultValue)){
            std::cout << "Enter " << mcsm::formatPrompt(key) << "(default : " << defaultValue << ") : ";
        }else{
            std::cout << "Enter " << mcsm::formatPrompt(key) << " : ";
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

        auto serv = mcsm::ServerRegistry::getServerRegistry().getServer(type);
        if(serv){
            break;
        }else{
            std::cout << "Server type \"" << type << "\" does not exist. Please try again.\n";
        }
    }

    std::map<std::string, std::string> extras;

    auto sPtr = mcsm::unwrapOrExit(mcsm::ServerRegistry::getServerRegistry().getServer(type));
    auto sv = sPtr->getRequiredValues();
    if(!sv) {
        mcsm::printError(sv);
        mcsm::exitIfFail(sv);
    }

    for(auto&[name, defaultValue] : sv.value()){
        handle(name, extras, defaultValue);
    }

    std::string name = extras["name"];
    std::string version = extras["minecraft_version"];
    std::string update = extras["auto_server_jar_update"];
    bool bUpdate = false;
    if(!mcsm::isWhitespaceOrEmpty(update)){
        bUpdate = update == "true" ? true : false;
    }

    mcsm::SearchTarget t = getSearchTarget(extras["default_jvm_launch_profile_search_path"]);
    if(t == mcsm::SearchTarget::ALL){
        mcsm::error("default JVM launch profile search path (current/global) invalid value detected");
        std::exit(1);
    }
    mcsm::JvmOption defaultProfile(mcsm::unwrapOrExit(mcsm::jvmProfileFromSearchTarget(extras["default_jvm_launch_profile_name"], t, mcsm::unwrapOrExit(mcsm::getCurrentPath()))));
    mcsm::unwrapOrExit(defaultProfile.init());

    mcsm::unwrapOrExit(sPtr->generate(name, defaultProfile, mcsm::unwrapOrExit(mcsm::getCurrentPath()), version, bUpdate, extras));
    return;
    
    mcsm::error("Server type not supported : " + type);
    std::exit(1);
}

inline bool mcsm::GenerateServerCommand::isConfigured(){
    std::string path = mcsm::unwrapOrExit(mcsm::getCurrentPath());
    bool fileExists = mcsm::unwrapOrExit(mcsm::fileExists(path + "/server.json"));
    return fileExists;
}