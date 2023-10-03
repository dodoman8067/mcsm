#include "jvm_test_command.h"

mcsm::JvmTestCommand::JvmTestCommand(const std::string& name, const std::string& description) : mcsm::Command(name, description) {}

mcsm::JvmTestCommand::~JvmTestCommand(){}

void mcsm::JvmTestCommand::execute(const std::vector<std::string>& args){
    if(args.empty()){
        std::cerr << "[mcsm] Invalid arguments.\n";
        std::cerr << "[mcsm] You must specify a name by --name option.\n";
        std::exit(1);
    }
    std::string name = getProfileName(args);
    std::unique_ptr<mcsm::JvmOption> option = searchOption(getSearchTarget(args), std::move(name));
    if(option == nullptr){
        std::cerr << "[mcsm] No JVM launch profile found in the given search target.\n";
        std::exit(1);
    }
    performTest(std::move(option));
}

std::unique_ptr<mcsm::JvmOption> mcsm::JvmTestCommand::searchOption(const mcsm::SearchTarget& target, const std::string& name){
    if(target == mcsm::SearchTarget::GLOBAL || target == mcsm::SearchTarget::ALL){
        std::unique_ptr<mcsm::JvmOption> opt = std::make_unique<mcsm::JvmOption>(name, mcsm::SearchTarget::GLOBAL);
        if(opt->exists()) return std::move(opt);
    }
    if(target == mcsm::SearchTarget::CURRENT || target == mcsm::SearchTarget::ALL){
        std::unique_ptr<mcsm::JvmOption> opt = std::make_unique<mcsm::JvmOption>(name, mcsm::SearchTarget::CURRENT);
        if(opt->exists()) return std::move(opt);
    }
    return nullptr;
}

mcsm::SearchTarget mcsm::JvmTestCommand::getSearchTarget(const std::vector<std::string>& args){
    if(args.empty()) return mcsm::SearchTarget::ALL;
    for(const std::string& arg : args) {
        if(arg == "--global" || arg == "-global" || arg == "--g" || arg == "-g") return mcsm::SearchTarget::GLOBAL;
        if(arg == "--current" || arg == "-current" || arg == "--c" || arg == "-c") return mcsm::SearchTarget::CURRENT;
    }
    return mcsm::SearchTarget::ALL;
}

std::string mcsm::JvmTestCommand::getProfileName(const std::vector<std::string>& args) const {
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

inline void mcsm::JvmTestCommand::performTest(std::unique_ptr<mcsm::JvmOption> option){
    std::string jvm = option->getJvmPath();
    std::cout << "[mcsm] Performing test with the following JVM path : " << jvm << "\n";
    if(mcsm::runCommandQuietly(jvm + " --version") != 0){
        std::cerr << "[mcsm] Test failed : " << jvm << "\n";
        std::exit(1);
    }
    std::cout << "[mcsm] Test success : " << jvm << "\n";
}