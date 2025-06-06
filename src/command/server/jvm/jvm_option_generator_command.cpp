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

#include <mcsm/command/server/jvm/jvm_option_generator_command.h>

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

mcsm::JvmOptionGeneratorCommand::JvmOptionGeneratorCommand(const std::string& name, const std::string& description) : mcsm::Command(name, description) {}

mcsm::JvmOptionGeneratorCommand::~JvmOptionGeneratorCommand() {}

void mcsm::JvmOptionGeneratorCommand::execute(const std::vector<std::string>& args) {
    if(args.empty()){
        mcsm::warning("Name not provided; specify a name with --name option to coutinue.");
        std::exit(1);
    }
    if(getSaveTarget(args) == mcsm::SearchTarget::GLOBAL){
        createProfile(args, mcsm::SearchTarget::GLOBAL);
        return;
    }
    createProfile(args, mcsm::SearchTarget::CURRENT);
    return;
}

mcsm::SearchTarget mcsm::JvmOptionGeneratorCommand::getSaveTarget(const std::vector<std::string>& args){
    if(args.empty()) return mcsm::SearchTarget::ALL;
    for(std::string_view arg : args) {
        if(arg == "--global" || arg == "-global" || arg == "--g" || arg == "-g") return mcsm::SearchTarget::GLOBAL;
    }
    return mcsm::SearchTarget::CURRENT;
}

std::string mcsm::JvmOptionGeneratorCommand::getJvmPath(const std::vector<std::string>& args) const {
    std::string jvmPath;
    for(size_t i = 0; i < args.size(); ++i){
        std::string_view arg = args[i];
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
    mcsm::info("Java detection from -jp command arguments failed; Automatically detecting java..");
    std::string java =  mcsm::detectJava();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        mcsm::printResultMessage();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_WARN_NOEXIT) std::exit(1);
    }

    return java;
}

std::string mcsm::JvmOptionGeneratorCommand::getProfileName(const std::vector<std::string>& args, const mcsm::SearchTarget& target) const {
    std::string name;
    for(size_t i = 0; i < args.size(); ++i){
        std::string_view arg = args[i];
        if(std::find(availableOptions.begin(), availableOptions.end(), arg) != availableOptions.end()){
            if(!(arg == "-name" || arg == "--name" || arg == "-n" || arg == "--n")) continue;
            if(i + 1 < args.size() && !args[i + 1].empty() && args[i + 1][0] != '-') {
                name = mcsm::safeString(args[i + 1]);
                mcsm::JvmOption option(name, target);
                if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
                    mcsm::printResultMessage();
                    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_WARN_NOEXIT) std::exit(1);
                }
                    
                bool exists = option.exists();
                if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
                    mcsm::printResultMessage();
                    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_WARN_NOEXIT) std::exit(1);
                }

                if(exists){
                    mcsm::warning("JVM launch profile " + name + " already exists. Command failed.");
                    std::exit(1); 
                }
                return name;
            }
        }
    }
    mcsm::warning("Name not provided; specify a name with --name option to continue.");
    std::exit(1);
}

std::vector<std::string> mcsm::JvmOptionGeneratorCommand::getServerArguments(const std::vector<std::string>& args) const {
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

std::vector<std::string> mcsm::JvmOptionGeneratorCommand::getJvmArguments(const std::vector<std::string>& args) const {
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

inline void mcsm::JvmOptionGeneratorCommand::createProfile(const std::vector<std::string>& args, const mcsm::SearchTarget& target){
    mcsm::JvmOption option(getProfileName(args, target), target);
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        mcsm::printResultMessage();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_WARN_NOEXIT) std::exit(1);
    }
    std::vector<std::string> jvmArgs = getJvmArguments(args);
    if(jvmArgs.empty()){
        jvmArgs.push_back("-Xms2G");
        jvmArgs.push_back("-Xmx2G");
        jvmArgs.push_back("-jar");
        mcsm::info("No JVM arguments specified; Defaulting to -Xms2G, -Xmx2G and -jar.");
    }
    std::vector<std::string> sargs = getServerArguments(args);
    if(sargs.empty()) {
        mcsm::info("No server arguments specified; Defaulting to nogui.");
        sargs.push_back("nogui");
    }
    std::string jvm = getJvmPath(args);
    mcsm::Result createResult = option.create(std::move(jvm), std::move(jvmArgs), std::move(sargs), target);
    if(!createResult.isSuccess()){
        createResult.printMessage();
        if(createResult.getResult() != mcsm::ResultType::MCSM_WARN_NOEXIT) std::exit(1);
    }

    auto name = option.getProfileName();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        mcsm::printResultMessage();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_WARN_NOEXIT) std::exit(1);
    }

    auto path = option.getProfilePath();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        mcsm::printResultMessage();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_WARN_NOEXIT) std::exit(1);
    }

    auto jArgs = option.getJvmArguments();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        mcsm::printResultMessage();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_WARN_NOEXIT) std::exit(1);
    }

    auto serverArgs = option.getServerArguments();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        mcsm::printResultMessage();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_WARN_NOEXIT) std::exit(1);
    }

    mcsm::info("Java Virtual Machine launch profile generated : ");
    mcsm::info("Profile name : " + name);
    mcsm::info("Profile location : " + path);
    mcsm::info("JVM path : " + jvm);
    
    if(!jArgs.empty()){
        std::cout << "[mcsm/INFO] JVM arguments : ";
        for(std::string_view args : jArgs){
            std::cout << args << " ";
        }
        std::cout << "\n";
    }
    if(!serverArgs.empty()){
        std::cout << "[mcsm/INFO] Server arguments : ";
        for(const std::string& sArgs : serverArgs){
            std::cout << sArgs << " ";
        }
        std::cout << "\n";
    }
}