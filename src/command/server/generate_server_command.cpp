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
    "--version",
    "-version",
    "--ver",
    "-ver",
    "--v",
    "-v"
};

mcsm::GenerateServerCommand::GenerateServerCommand(const std::string& name, const std::string& description) : Command(name, description) {}

mcsm::GenerateServerCommand::~GenerateServerCommand(){}

void mcsm::GenerateServerCommand::execute(const std::vector<std::string>& args){
    if(args.empty()){
        mcsm::warning("Name not provided; Specify a name with --name option to continue.");
        std::exit(1);
    }
    detectServer(args);
}

void mcsm::GenerateServerCommand::askServer(std::string& value){
    while(true){
        std::cout << "Type a string: ";
        std::getline(std::cin, value);
            
        if(!value.empty()){
            break;
        }else{
            std::cout << "Wrong input" << std::endl;
        }
    }
}

std::string mcsm::GenerateServerCommand::getProfileName(const std::vector<std::string>& args) const {
    std::string name;
    for(size_t i = 0; i < args.size(); ++i){
        const std::string& arg = args[i];
        if(std::find(availableOptions.begin(), availableOptions.end(), arg) != availableOptions.end()){
            if(!(arg == "-profile" || arg == "--profile" || arg == "-p" || arg == "--p")) continue;
            if(i + 1 < args.size() && !args[i + 1].empty() && args[i + 1][0] != '-') {
                name = args[i + 1];
                if(name.find("\"") == std::string::npos && name.find("\'") == std::string::npos){
                    return name;
                }else{
                    mcsm::replaceAll(name, "\"", "");
                    mcsm::replaceAll(name, "\'", "");
                    mcsm::warning("NOTE : \' and \" are not allowed in profile names; The profile name was modified to " + name + ".");
                }
                return name;
            }
        }
    }
    mcsm::warning("Profile name not provided; Specify a profile name with --profile option to continue.");
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
                if(name.find("\"") == std::string::npos && name.find("\'") == std::string::npos){
                    return name;
                }else{
                    mcsm::replaceAll(name, "\"", "");
                    mcsm::replaceAll(name, "\'", "");
                    std::cout << "[mcsm] NOTE : \' and \" are not allowed in profile names; The profile name was modified to " << name << ".\n";
                }
                return name;
            }
        }
    }
    mcsm::warning("Server name not provided; Specify a name with --name option to continue.");
    std::exit(1);
}

std::string mcsm::GenerateServerCommand::getServerVersion(const std::vector<std::string>& args) const {
    std::string ver;
    for(size_t i = 0; i < args.size(); ++i){
        const std::string& arg = args[i];
        if(std::find(availableOptions.begin(), availableOptions.end(), arg) != availableOptions.end()){
            if(!(arg == "-version" || arg == "--version" || arg == "-ver" || arg == "--ver" || arg == "-v" || arg == "--v")) continue;
            if(i + 1 < args.size() && !args[i + 1].empty() && args[i + 1][0] != '-') {
                ver = args[i + 1];
                return ver;
            }
        }
    }
    mcsm::warning("Server version not provided; Specify a version with --version option to continue.");
    std::exit(1);
}

std::unique_ptr<mcsm::JvmOption> mcsm::GenerateServerCommand::searchOption(const mcsm::SearchTarget& target, const std::string& name){
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

mcsm::SearchTarget mcsm::GenerateServerCommand::getSearchTarget(const std::vector<std::string>& args){
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
                return type;
            }
        }
    }
    mcsm::warning("Server type not provided; Specify a type with --servertype option to continue.");
    std::exit(1);
}

void mcsm::GenerateServerCommand::detectServer(const std::vector<std::string>& args){
    mcsm::SearchTarget target = getSearchTarget(args);
    std::unique_ptr<mcsm::JvmOption> option = searchOption(target, getProfileName(args));
    std::string version = getServerVersion(args);
    std::string name = getServerName(args);
    std::string type = getServerType(args);

    if(option == nullptr){
        mcsm::warning("JVM launch profile " + getProfileName(args) + " not found.");
        mcsm::warning("Maybe you have misspelled --c or --g option.");
        std::exit(1);
    }

    if(type == "bukkit" || type == "craftbukkit"){
        mcsm::server::generateBukkit(name, *option, version, mcsm::BukkitServerType::CRAFTBUKKIT);
        return;
    }
    if(type == "spigot"){
        mcsm::server::generateBukkit(name, *option, version, mcsm::BukkitServerType::SPIGOT);
        return;
    }
    if(type == "paper" || type == "paperspigot"){
        mcsm::server::generateBukkit(name, *option, version, mcsm::BukkitServerType::PAPER);
        return;
    }
    if(type == "purpur"){
        mcsm::server::generateBukkit(name, *option, version, mcsm::BukkitServerType::PURPUR);
        return;
    }
    if(type == "bukkit" || type == "craftbukkit"){
        mcsm::server::generateBukkit(name, *option, version, mcsm::BukkitServerType::PUFFERFISH);
        return;
    }
    mcsm::error("Server type not supported : " + type);
    std::exit(1);
}