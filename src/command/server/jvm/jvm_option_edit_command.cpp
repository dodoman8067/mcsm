#include <mcsm/command/server/jvm/jvm_option_edit_command.h>

const std::vector<std::string> availableOptions = {
    "-jvmpath",
    "--jvmpath",
    "-jp",
    "--jp",
    "--global"
    "-global",
    "-g",
    "--g",
    "--serverarguments",
    "-serverarguments",
    "--serverargs",
    "-serverargs",
    "--sargs",
    "-sargs",
    "--sa",
    "-sa",
    "-name",
    "--name",
    "-n",
    "--n",
    "-jargs",
    "--jargs",
    "-ja",
    "--ja" ,
    "--jvmargs",
    "-jvmargs"
};

mcsm::JvmOptionEditCommand::JvmOptionEditCommand(const std::string& name, const std::string& description) : Command(name, description) {}

mcsm::JvmOptionEditCommand::~JvmOptionEditCommand() {}

void mcsm::JvmOptionEditCommand::execute(const std::vector<std::string>& args){

}


mcsm::SearchTarget mcsm::JvmOptionEditCommand::getSaveTarget(const std::vector<std::string>& args){
    if(args.empty()) return mcsm::SearchTarget::ALL;
    for(const std::string& arg : args) {
        if(arg == "--global" || arg == "-global" || arg == "--g" || arg == "-g") return mcsm::SearchTarget::GLOBAL;
    }
    return mcsm::SearchTarget::CURRENT;
}

std::string mcsm::JvmOptionEditCommand::getJvmPath(const std::vector<std::string>& args) const {
    std::string jvmPath;
    for(size_t i = 0; i < args.size(); ++i){
        const std::string& arg = args[i];
        if(std::find(availableOptions.begin(), availableOptions.end(), arg) != availableOptions.end()){
            if(!(arg == "-jvmpath" || arg == "--jvmpath" || arg == "-jp" || arg == "--jp")) continue;
            if(i + 1 < args.size() && !args[i + 1].empty() && args[i + 1][0] != '-') {
                jvmPath = args[i + 1];
                if(!mcsm::startsWith(jvmPath, "\"") && !mcsm::startsWith(jvmPath, "\'")){
                    jvmPath = "\"" + jvmPath;
                }
                if(!mcsm::endsWith(jvmPath, "\"") && !mcsm::endsWith(jvmPath, "\'")){
                    jvmPath += "\"";
                }
                if (!mcsm::isValidJava(jvmPath)) continue;
                mcsm::success("Detected java from specified path : " + jvmPath);
                return jvmPath;
            }
        }
    }
    mcsm::warning("Java detection from -jp command arguments failed.");
    mcsm::warning("Please specify a valid JVM path.");
    std::exit(1);
}

std::string mcsm::JvmOptionEditCommand::getProfileName(const std::vector<std::string>& args, const mcsm::SearchTarget& target) const {
    std::string name;
    for(size_t i = 0; i < args.size(); ++i){
        const std::string& arg = args[i];
        if(std::find(availableOptions.begin(), availableOptions.end(), arg) != availableOptions.end()){
            if(!(arg == "-name" || arg == "--name" || arg == "-n" || arg == "--n")) continue;
            if(i + 1 < args.size() && !args[i + 1].empty() && args[i + 1][0] != '-') {
                name = args[i + 1];
                if(name.find("\"") == std::string::npos && name.find("\'") == std::string::npos){
                    mcsm::JvmOption option(name, target);
                    if(!option.exists()){
                        mcsm::warning("JVM launch profile " + name + " does not exist. run mcsm genJvmOption command to generate it first.");
                        std::exit(1); 
                    }
                }else{
                    mcsm::replaceAll(name, "\"", "");
                    mcsm::replaceAll(name, "\'", "");
                    mcsm::warning("NOTE : \' and \" are not allowed in names; Name was modified to " + name + ".");
                    mcsm::JvmOption option(name, target);
                    if(!option.exists()){
                        mcsm::warning("JVM launch profile " + name + " does not exist. run mcsm genJvmOption command to generate it first.");
                        std::exit(1); 
                    }
                }
                return name;
            }
        }
    }
    mcsm::warning("Name not provided; Specify a name with --name option to continue.");
    std::exit(1);
}

std::vector<std::string> mcsm::JvmOptionEditCommand::getServerArguments(const std::vector<std::string>& args) const {
    std::vector<std::string> result;
    bool foundServerArgsFlag = false;

    for(const std::string& arg : args){
        if(foundServerArgsFlag){
            if(std::find(availableOptions.begin(), availableOptions.end(), arg) != availableOptions.end()) break;
            std::istringstream iss(arg);
            std::string part;
            while (std::getline(iss, part, ' ')){
                if (!part.empty()) {
                    result.push_back(part);
                }
            }
        }else if(arg == "--serverarguments" || arg == "-serverarguments" || arg == "--serverargs" || arg == "-serverargs" || arg == "--sargs" || arg == "-sargs" || arg == "--sa" || arg == "-sa"){
            foundServerArgsFlag = true;
        }
    }

    return result;
}

std::vector<std::string> mcsm::JvmOptionEditCommand::getJvmArguments(const std::vector<std::string>& args) const {
    std::vector<std::string> result;
    bool foundServerArgsFlag = false;

    for(const std::string& arg : args){
        if(foundServerArgsFlag){
            if(std::find(availableOptions.begin(), availableOptions.end(), arg) != availableOptions.end()) break;
            std::istringstream iss(arg);
            std::string part;
            while (std::getline(iss, part, ' ')){
                if (!part.empty()) {
                    result.push_back(part);
                }
            }
        }else if(arg == "--jvmargs" || arg == "-jvmargs" || arg == "--jargs" || arg == "-jargs" || arg == "--ja" || arg == "-ja"){
            foundServerArgsFlag = true;
        }
    }

    return result;
}
