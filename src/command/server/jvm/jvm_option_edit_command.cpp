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
    editProfile(std::move(args));
}


mcsm::SearchTarget mcsm::JvmOptionEditCommand::getSearchTarget(const std::vector<std::string>& args){
    if(args.empty()) return mcsm::SearchTarget::ALL;
    for(const std::string& arg : args) {
        if(arg == "--global" || arg == "-global" || arg == "--g" || arg == "-g") return mcsm::SearchTarget::GLOBAL;
        if(arg == "--current" || arg == "-current" || arg == "--c" || arg == "-c") return mcsm::SearchTarget::CURRENT;
    }
    return mcsm::SearchTarget::ALL;
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
    mcsm::info("JVM not detected or not a valid JVM. Ignoring..");
    return "";
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
                        mcsm::warning("JVM launch profile " + name + " does not exist. run \"mcsm genJvmProfile --name " + name + "\" command to generate it first.");
                        std::exit(1); 
                    }
                }else{
                    mcsm::replaceAll(name, "\"", "");
                    mcsm::replaceAll(name, "\'", "");
                    mcsm::warning("NOTE : \' and \" are not allowed in names; Name was modified to " + name + ".");
                    mcsm::JvmOption option(name, target);
                    if(!option.exists()){
                        mcsm::warning("JVM launch profile " + name + " does not exist. run \"mcsm genJvmProfile --name " + name + "\" command to generate it first.");
                        std::exit(1); 
                    }
                }
                return name;
            }
        }
    }
    mcsm::warning("Name not provided; To continue, specify a name with --name option.");
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


inline void mcsm::JvmOptionEditCommand::editProfile(const std::vector<std::string>& args) {
    mcsm::SearchTarget target = getSearchTarget(args);
    mcsm::JvmOption option(getProfileName(args, target), target);

    if(!option.exists()){
        mcsm::error("Profile does not exist; Task aborted.");
        std::exit(1);
    }

    std::vector<std::string> jvmArgs = getJvmArguments(args);
    std::vector<std::string> sArgs = getServerArguments(args);

    std::vector<std::string> optJvmArgs = option.getJvmArguments();
    std::vector<std::string> optServerArgs = option.getServerArguments();
    std::string jvm = getJvmPath(args);

    mcsm::info("Java Virtual Machine launch profile edited : ");
    mcsm::info("Profile name : " + option.getProfileName());

    if (option.getJvmPath() != jvm && !mcsm::isWhitespaceOrEmpty(jvm)) {
        mcsm::info("JVM path : " + option.getJvmPath() + " -> " + jvm);
        option.setJvmPath(jvm);
    }

    if(!jvmArgs.empty() && optJvmArgs != jvmArgs){
        option.setJvmArguments(jvmArgs);
        printDifference("JVM arguments", optJvmArgs, jvmArgs);
    }
    if(!sArgs.empty()){
        option.setServerArguments(sArgs);
        printDifference("Server arguments", optServerArgs, sArgs);
    }
}

inline void mcsm::JvmOptionEditCommand::printDifference(const std::string& category, const std::vector<std::string>& from, const std::vector<std::string>& to) const {
    if(from != to){
        std::cout << "[mcsm/INFO] " << category << " : \n";
        if(!from.empty()){
            std::cout << "[mcsm/INFO] From : ";
            for (const std::string& arg : from) {
                std::cout << arg << " ";
            }
        }

        if(!to.empty()){
            std::cout << "\n[mcsm/INFO] To : ";
            for(const std::string& arg : to){
                std::cout << arg << " ";
            }
        }
        std::cout << "\n";
    }
}
