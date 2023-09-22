#include "jvm_option_generator_command.h"

mcsm::JvmOptionGeneratorCommand::JvmOptionGeneratorCommand(const std::string& name, const std::string& description) : mcsm::Command(name, description) {}

mcsm::JvmOptionGeneratorCommand::~JvmOptionGeneratorCommand() {}

void mcsm::JvmOptionGeneratorCommand::execute(const std::vector<std::string>& args) {
    if(args.empty()){
        std::cout << "Invalid arguments.\n";
        std::cout << "You must provide alteast profile name.\n";
        std::exit(1);
    }
    if(getSaveTarget(args) == mcsm::SearchTarget::GLOBAL){
        mcsm::JvmOption option("a", mcsm::SearchTarget::GLOBAL);
        option.create(getJvmPath(args), mcsm::SearchTarget::GLOBAL);
        return;
    }
    mcsm::JvmOption option("a", mcsm::SearchTarget::CURRENT);
    option.create(getJvmPath(args), mcsm::SearchTarget::CURRENT);
    return;
}

mcsm::SearchTarget mcsm::JvmOptionGeneratorCommand::getSaveTarget(const std::vector<std::string>& args){
    if(args.empty()) return mcsm::SearchTarget::ALL;
    for(const std::string& arg : args) {
        if(arg == "--global" || arg == "-global") return mcsm::SearchTarget::GLOBAL;
    }
    return mcsm::SearchTarget::CURRENT;
}

std::string mcsm::JvmOptionGeneratorCommand::getJvmPath(const std::vector<std::string>& args) const {
    for (size_t i = 0; i < args.size(); ++i) {
        const std::string& arg = args[i];
        if (arg == "--jvmpath" || arg == "-jvmpath" || arg == "--jp" || arg == "-jp") {
            if (i + 1 < args.size() && !args[i + 1].empty() && args[i + 1][0] != '-') {
                std::string jvmPath = args[i + 1];
                if (!mcsm::startsWith(jvmPath, "\"")) {
                    // Add double quotes to the beginning of the path
                    jvmPath = "\"" + jvmPath;
                }
                if (!mcsm::endsWith(jvmPath, "\"")) {
                    // Add double quotes to the end of the path
                    jvmPath += "\"";
                }
                if (!mcsm::isValidJava(jvmPath)) continue;
                
                return jvmPath;
            }
        }
    }
    std::cout << "[mcsm] Java detection from -jp command arguments failed; Automatically detecting java..\n";
    return mcsm::detectJava();
}