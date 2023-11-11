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
    searchOption(args);
}

std::unique_ptr<mcsm::JvmOption> mcsm::StartServerCommand::searchOption(const std::vector<std::string>& args){
    if(!isConfigured()){
        std::cerr << "[mcsm] Server not configured.\n";
        std::cerr << "[mcsm] Task aborted.\n";
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
                    std::cerr << "[mcsm] The specified JVM launch profile " << pName << " does not exist.\n";
                    std::exit(1);
                }
                std::cout << "[mcsm] Found specified JVM launch profile " << pName << ".\n";
                std::cout << "[mcsm] Location : " << profile->getProfilePath() << "\n";
                return profile;
            }
        }
    }
    std::cout << "[mcsm] No JVM launch profile specified; Using default JVM launch profile.\n";
    mcsm::ServerOption option;
    std::unique_ptr<mcsm::JvmOption> jvmOpt = option.getDefaultOption();
    return jvmOpt;
}

std::unique_ptr<mcsm::JvmOption> mcsm::StartServerCommand::searchOption(const mcsm::SearchTarget& target, const std::string& name){
    if(!isConfigured()){
        std::cerr << "[mcsm] Server not configured.\n";
        std::cerr << "[mcsm] Task aborted.\n";
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