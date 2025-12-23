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

std::string prompt(const mcsm::ServerOptionSpec& spec) {
    if(!spec.defaultValue.empty()){
        std::cout << "Enter "
                  << mcsm::formatPrompt(spec.key)
                  << " (default: " << spec.defaultValue << ") : ";
    }else{
        std::cout << "Enter "
                  << mcsm::formatPrompt(spec.key)
                  << " : ";
    }

    std::string input;
    std::getline(std::cin, input);
    return input;
}


bool validate(const mcsm::ServerOptionSpec& spec, const std::string& value) {
    if(value.empty()) return !spec.required;

    switch (spec.type){
        case mcsm::OptionType::STRING:
            return true;

        case mcsm::OptionType::BOOL:
            return value == "true" || value == "false";

        case mcsm::OptionType::ENUM:
            return std::find(spec.enumValues.begin(), spec.enumValues.end(), value) != spec.enumValues.end();
    }

    return false;
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

    auto vecRes = sPtr->getRequiredOptions();
    if(!vecRes){
        mcsm::printError(vecRes.error());
        mcsm::exitIfFail(vecRes.error());
    }
    auto vec = vecRes.value();
    for(const auto& spec : vec){
        if(spec.resolveValue){
            auto resolved = spec.resolveValue(extras);
            if(resolved){
                extras[spec.key] = *resolved;
                continue;
            }
        }

        if(spec.visibleIf && !spec.visibleIf(extras)){
            extras[spec.key] = spec.defaultValue;
            continue;
        }

        bool isRequired = spec.required;
        if(spec.requiredIf){
            isRequired = spec.requiredIf(extras);
        }

        std::string value;
        while (true){
            value = prompt(spec);

            if(value.empty() && !isRequired){
                value = spec.defaultValue;
                break;
            }

            if(validate(spec, value)){
                break;
            }

            std::cout << "Invalid input \"" << value << "\". Please try again.\n";
        }

        extras[spec.key] = value;
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