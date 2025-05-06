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

#include <mcsm/command/server/start_server_command.h>
#include <mcsm/data/options/server_group_loader.h>

const std::vector<std::string> availableOptions = {
    "--profile",
    "-profile",
    "-p",
    "--p",
    "--jvmprofile",
    "-jvmprofile",
    "--jp",
    "-jp"
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
    if(!isConfigured()){
        mcsm::warning("Server not configured.");
        mcsm::warning("Task aborted.");
        std::exit(1);
    }

    bool isGroup = false;
    std::string groupPath;
    for(size_t i = 0; i < args.size(); ++i){
        std::string_view arg = args[i];
        if(std::find(availableOptions.begin(), availableOptions.end(), arg) != availableOptions.end()){
            if(!(arg == "-__mcsm__Internal_Group_Start")) continue;
            if(i + 1 < args.size() && !args[i + 1].empty() && args[i + 1][0] != '-'){
                groupPath = mcsm::safeString(args[i + 1]);
                if(!std::filesystem::exists(groupPath)){
                    mcsm::warning("Invalid group path: " + groupPath);
                    std::exit(1);
                }
                mcsm::ServerGroupLoader gLoader(groupPath);
                mcsm::Result gLoadRes = gLoader.load();
                if(gLoadRes.getResult() != mcsm::ResultType::MCSM_OK && gLoadRes.getResult() != mcsm::ResultType::MCSM_SUCCESS){
                    gLoadRes.printMessage();
                    if(gLoadRes.getResult() != mcsm::ResultType::MCSM_WARN_NOEXIT) std::exit(1);
                }

                std::vector<const mcsm::ServerConfigLoader*> gServers = gLoader.getServers();
                if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
                    mcsm::printResultMessage();
                    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_WARN_NOEXIT) std::exit(1);
                }

                for(const mcsm::ServerConfigLoader* gServer : gServers){
                    if(gServer == nullptr) continue;
                    if(gServer->getHandle()->getPath() != mcsm::getCurrentPath()) continue;
                    isGroup = true;
                    break;
                }
                mcsm::warning("Group validation failed. If you're seeing this it's likely a bug. Report this at Github.");
                break;
            }
        }
    }

    mcsm::ServerConfigLoader loader(mcsm::getCurrentPath());
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        mcsm::printResultMessage();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_WARN_NOEXIT) std::exit(1);
    }

    loader.loadConfig();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        mcsm::printResultMessage();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_WARN_NOEXIT) std::exit(1);
    }

    std::unique_ptr<mcsm::JvmOption> jvmOpt = searchOption(args, &loader);

    mcsm::ServerStarter starter(&loader);
    if(isGroup){
        starter.startServer(*jvmOpt, mcsm::getCurrentPath(), mcsm::getCurrentPath(), groupPath);
    }else{
        starter.startServer(*jvmOpt, mcsm::getCurrentPath(), mcsm::getCurrentPath());
    }
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        mcsm::printResultMessage();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_WARN_NOEXIT) std::exit(1);
    }
}

std::unique_ptr<mcsm::JvmOption> mcsm::StartServerCommand::searchOption(const std::vector<std::string>& args, mcsm::ServerConfigLoader* loader){
    if(!isConfigured()){
        mcsm::warning("Server not configured.");
        mcsm::warning("Task aborted.");
        std::exit(1);
    }
    for(size_t i = 0; i < args.size(); ++i){
        std::string_view arg = args[i];
        if(std::find(availableOptions.begin(), availableOptions.end(), arg) != availableOptions.end()){
            if(!(arg == "-profile" || arg == "--profile" || arg == "-p" || arg == "--p" || arg == "-jvmprofile" || arg == "--jvmprofile" || arg == "-jp" || "--jp")) continue;
            if(i + 1 < args.size() && !args[i + 1].empty() && args[i + 1][0] != '-'){
                std::string pName = mcsm::safeString(args[i + 1]);
                mcsm::SearchTarget target = getSearchTarget(args);
                std::unique_ptr<mcsm::JvmOption> profile = searchOption(target, pName);
                if(profile == nullptr){
                    mcsm::warning("The specified JVM launch profile " + pName + " does not exist.");
                    mcsm::warning("Task aborted.");
                    std::exit(1);
                }
                mcsm::info("Found JVM launch profile " + pName + ".");
                mcsm::info("Location : " + profile->getProfilePath());
                return profile;
            }
        }
    }
    mcsm::info("No specified JVM launch profile detected; using default JVM launch profile.");
    
    std::unique_ptr<mcsm::JvmOption> jvmOpt = loader->getDefaultOption();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        mcsm::printResultMessage();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_WARN_NOEXIT) std::exit(1);
    }
    return jvmOpt;
}

std::unique_ptr<mcsm::JvmOption> mcsm::StartServerCommand::searchOption(const mcsm::SearchTarget& target, const std::string& name){
    if(!isConfigured()){
        mcsm::warning("Server not configured.");
        mcsm::warning("Task aborted.");
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
    for(std::string_view arg : args) {
        if(arg == "--global" || arg == "-global" || arg == "--g" || arg == "-g") return mcsm::SearchTarget::GLOBAL;
        if(arg == "--current" || arg == "-current" || arg == "--c" || arg == "-c") return mcsm::SearchTarget::CURRENT;
    }
    return mcsm::SearchTarget::ALL;
}


inline bool mcsm::StartServerCommand::isConfigured(){
    std::string path = mcsm::getCurrentPath();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        mcsm::printResultMessage();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_WARN_NOEXIT) std::exit(1);
    }
    bool fileExists = mcsm::fileExists(path + "/server.json");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        mcsm::printResultMessage();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_WARN_NOEXIT) std::exit(1);
    }
    return fileExists;
}