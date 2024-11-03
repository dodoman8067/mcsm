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

#include <mcsm/command/server/jvm/jvm_test_command.h>

mcsm::JvmTestCommand::JvmTestCommand(const std::string& name, const std::string& description) : mcsm::Command(name, description) {}

mcsm::JvmTestCommand::~JvmTestCommand(){}

void mcsm::JvmTestCommand::execute(const std::vector<std::string>& args){
    if(args.empty()){
        mcsm::warning("Name not provided; To continue, specify a name with --name option.");
        std::exit(1);
    }
    std::string name = getProfileName(args);
    std::unique_ptr<mcsm::JvmOption> option = searchOption(getSearchTarget(args), std::move(name));
    if(option == nullptr){
        mcsm::warning("No JVM launch profile found in the given search target.");
        std::exit(1);
    }
    performTest(std::move(option));
}

std::unique_ptr<mcsm::JvmOption> mcsm::JvmTestCommand::searchOption(const mcsm::SearchTarget& target, const std::string& name){
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

mcsm::SearchTarget mcsm::JvmTestCommand::getSearchTarget(const std::vector<std::string>& args){
    if(args.empty()) return mcsm::SearchTarget::ALL;
    for(std::string_view arg : args) {
        if(arg == "--global" || arg == "-global" || arg == "--g" || arg == "-g") return mcsm::SearchTarget::GLOBAL;
        if(arg == "--current" || arg == "-current" || arg == "--c" || arg == "-c") return mcsm::SearchTarget::CURRENT;
    }
    return mcsm::SearchTarget::ALL;
}

std::string mcsm::JvmTestCommand::getProfileName(const std::vector<std::string>& args) const {
    std::string name;
    for(size_t i = 0; i < args.size(); ++i){
        std::string_view arg = args[i];
        if(!(arg == "-name" || arg == "--name" || arg == "-n" || arg == "--n")) continue;
        if(i + 1 < args.size() && !args[i + 1].empty() && args[i + 1][0] != '-') {
            name = args[i + 1];
            if(name.find("\"") == std::string::npos && name.find("\'") == std::string::npos){
                return name;
            }else{
                mcsm::replaceAll(name, "\"", "");
                mcsm::replaceAll(name, "\'", "");
                mcsm::warning("NOTE : \' and \" are not allowed in names; Name was modified to " + name + ".");
            }
            return name;
        }
    }
    mcsm::warning("Name not provided; To continue, specify a name.");
    std::exit(1);
}

inline void mcsm::JvmTestCommand::performTest(std::unique_ptr<mcsm::JvmOption> option){
    std::string jvm = option->getJvmPath();
    mcsm::info("Profile name : " + option->getProfileName());
    mcsm::info("Profile path : " + option->getProfilePath());
    mcsm::info("Performing test with the following command : " + jvm + " -version");
    if(mcsm::runCommandQuietly(jvm + " --version") != 0){
        mcsm::warning("Test failed : " + jvm);
        std::exit(1);
    }
    mcsm::success("Test success : " + jvm);
}