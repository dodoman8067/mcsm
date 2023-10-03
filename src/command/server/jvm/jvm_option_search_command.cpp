#include "jvm_option_search_command.h"

mcsm::JvmOptionSearchCommand::JvmOptionSearchCommand(const std::string& name, const std::string& description) : mcsm::Command(name, description) {}

mcsm::JvmOptionSearchCommand::~JvmOptionSearchCommand() {}

void mcsm::JvmOptionSearchCommand::execute(const std::vector<std::string>& args){
    if(args.empty()){
        std::cerr << "[mcsm] Invalid arguments.\n";
        std::cerr << "[mcsm] You must specify a name by --name option.\n";
        std::exit(1);
    }
    std::string name = getProfileName(args);
    std::vector<std::unique_ptr<mcsm::JvmOption>> options = search(getSearchTarget(args), std::move(name));
    if(options.empty()){
        std::cerr << "[mcsm] No JVM launch profile found in the given search target.\n";
        std::exit(1);
    }
    std::cout << "[mcsm] Search result : \n";
    std::cout << "\n";
    int i = 1;
    for(std::unique_ptr<mcsm::JvmOption>& option : options){
        std::cout << "Result " << i << " : "<< "\n";
        std::cout << "Name : " << option->getProfileName() << "\n";
        std::cout << "Path : " << option->getProfilePath() << "\n";
        std::cout << "\n";
        i++;
    }
}

std::vector<std::unique_ptr<mcsm::JvmOption>> mcsm::JvmOptionSearchCommand::search(const mcsm::SearchTarget& target, const std::string& name){
    std::vector<std::unique_ptr<mcsm::JvmOption>> result;
    if(target == mcsm::SearchTarget::GLOBAL || target == mcsm::SearchTarget::ALL){
        std::unique_ptr<mcsm::JvmOption> opt = std::make_unique<mcsm::JvmOption>(name, mcsm::SearchTarget::GLOBAL);
        if(opt->exists()) result.push_back(std::move(opt));
    }
    if(target == mcsm::SearchTarget::CURRENT || target == mcsm::SearchTarget::ALL){
        std::unique_ptr<mcsm::JvmOption> opt = std::make_unique<mcsm::JvmOption>(name, mcsm::SearchTarget::CURRENT);
        if(opt->exists()) result.push_back(std::move(opt));
    }
    return result;
}

std::string mcsm::JvmOptionSearchCommand::getProfileName(const std::vector<std::string>& args) const {
    std::string name;
    for(size_t i = 0; i < args.size(); ++i){
        const std::string& arg = args[i];
            if(!(arg == "-name" || arg == "--name" || arg == "-n" || arg == "--n")) continue;
            if(i + 1 < args.size() && !args[i + 1].empty() && args[i + 1][0] != '-') {
                name = args[i + 1];
                if(name.find("\"") == std::string::npos && name.find("\'") == std::string::npos){
                    return name;
                }else{
                    mcsm::replaceAll(name, "\"", "");
                    mcsm::replaceAll(name, "\'", "");
                    std::cout << "[mcsm] NOTE : \' and \" are not allowed in names; Name was modified to " << name << ".\n";
                }
                return name;
            }
        
    }
    std::cerr << "[mcsm] Name not provided; Specify a name to continune.\n";
    std::exit(1);
}

mcsm::SearchTarget mcsm::JvmOptionSearchCommand::getSearchTarget(const std::vector<std::string>& args){
    if(args.empty()) return mcsm::SearchTarget::ALL;
    for(const std::string& arg : args) {
        if(arg == "--global" || arg == "-global" || arg == "--g" || arg == "-g") return mcsm::SearchTarget::GLOBAL;
        if(arg == "--current" || arg == "-current" || arg == "--c" || arg == "-c") return mcsm::SearchTarget::CURRENT;
    }
    return mcsm::SearchTarget::ALL;
}
