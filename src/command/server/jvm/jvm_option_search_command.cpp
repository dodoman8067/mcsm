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

#include <mcsm/command/server/jvm/jvm_option_search_command.h>

mcsm::JvmOptionSearchCommand::JvmOptionSearchCommand(const std::string& name, const std::string& description) : mcsm::Command(name, description) {}

mcsm::JvmOptionSearchCommand::~JvmOptionSearchCommand() {}

void mcsm::JvmOptionSearchCommand::execute(const std::vector<std::string>& args){
    if(args.empty()){
        mcsm::warning("Name not provided; To continue, specify a name with --name option.");
        std::exit(1);
    }
    std::string name = getProfileName(args);
    std::vector<std::unique_ptr<mcsm::JvmOption>> options = search(getSearchTarget(args), std::move(name));
    if(options.empty()){
        mcsm::info("No JVM launch profile found in the given search target.");
        std::exit(1);
    }
    mcsm::info("Search result : ");
    std::cout << "\n";
    int i = 1;
    for(std::unique_ptr<mcsm::JvmOption>& option : options){
        std::cout << "Result " << i << " : "<< "\n";
        std::cout << "Name : " << option->getProfileName() << "\n";
        std::cout << "Path : " << option->getProfilePath() << "\n";
        std::cout << "JVM : " << option->getJvmPath() << "\n";
        if(!option->getJvmArguments().empty()){
            std::cout << "JVM arguments : ";
            for(const std::string& args : option->getJvmArguments()){
                std::cout << args << " ";
            }
            std::cout << "\n";
        }
        if(!option->getServerArguments().empty()){
            std::cout << "Server arguments : ";
            for(const std::string& sArgs : option->getServerArguments()){
                std::cout << sArgs << " ";
            }
            std::cout << "\n";
        }
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
                mcsm::warning("NOTE : \' and \" are not allowed in names; Name was modified to " + name + ".");
            }
            return name;
        }
    }
    mcsm::warning("Name not provided; To continue, specify a name with --name option.");
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
