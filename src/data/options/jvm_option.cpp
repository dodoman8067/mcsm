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

#include <mcsm/data/options/jvm_option.h>

mcsm::JvmOption::JvmOption(const std::string& name) : JvmOption(name, mcsm::SearchTarget::ALL) {}

mcsm::JvmOption::JvmOption(const std::string& name, const mcsm::SearchTarget& target){
    this->name = name;
    switch (target) {
        case mcsm::SearchTarget::ALL: {
            mcsm::GlobalOption globalOption("/jvm/profiles", name);
            if(globalOption.exists()){
                this->option.reset(new mcsm::GlobalOption("/jvm/profiles", name));
            } else {
                this->option.reset(new mcsm::Option("./.mcsm/jvm/profiles", name));
            }
            break;
        }
        case mcsm::SearchTarget::GLOBAL: {
            mcsm::GlobalOption globalOption("/jvm/profiles", name);
            if(globalOption.exists()){
                this->option.reset(new mcsm::GlobalOption("/jvm/profiles", name));
            }
            break;
        }
        case mcsm::SearchTarget::CURRENT: {
            mcsm::Option option("./.mcsm/jvm/profiles", name);
            if(option.exists()){
                this->option.reset(new mcsm::Option("./.mcsm/jvm/profiles", name));
            }
            break;
        }
    }
}

void mcsm::JvmOption::create(){
    std::string jvm = mcsm::detectJava();
    std::vector<std::string> jvmArgs = {
        "-Xms2G",
        "-Xmx2G",
        "-jar"
    };
    std::vector<std::string> serverArgs = {
        "nogui"
    };
    if(mcsm::isWhitespaceOrEmpty(jvm)){
        mcsm::error("JVM detection failed.");
        mcsm::error("This error can be fixed by setting JAVA_HOME enviroment variable.");
        mcsm::error("Or if you believe this is a software issue, please report it to GitHub. (https://github.com/dodoman8067/mcsm)");
        std::exit(1);
    }
    create(jvm, jvmArgs, serverArgs, mcsm::SearchTarget::GLOBAL);
}

void mcsm::JvmOption::create(const std::string& jvmPath, const mcsm::SearchTarget& target){
    std::vector<std::string> jvmArgs = {
        "-Xms2G",
        "-Xmx2G",
        "-jar"
    };
    std::vector<std::string> serverArgs = {
        "nogui"
    };
    create(jvmPath, jvmArgs, serverArgs, target);
}

void mcsm::JvmOption::create(const std::string& jvmPath, const std::vector<std::string>& jvmOptions, const mcsm::SearchTarget& target){
    std::vector<std::string> serverArgs = {
        "nogui"
    };
    create(jvmPath, jvmOptions, serverArgs, target);
}

void mcsm::JvmOption::create(const std::string& jvmPath, const std::vector<std::string>& jvmOptions, const std::vector<std::string>& serverOptions, const mcsm::SearchTarget& target){
    std::string filePath;
    std::string optionName;

    if(target == mcsm::SearchTarget::CURRENT){
        mcsm::Option option("./.mcsm/jvm/profiles", this->name);
        filePath = option.getPath();
        optionName = option.getName();
    }else{
        mcsm::GlobalOption globalOption("./jvm/profiles", this->name);
        filePath = globalOption.getPath();
        optionName = globalOption.getName();
    }

    if(std::filesystem::exists(filePath + "/" + optionName)){
        std::filesystem::remove(filePath + "/" + optionName);
    }

    if(target == mcsm::SearchTarget::ALL) return;
    if(target == mcsm::SearchTarget::CURRENT){
        mcsm::Option* cOpt = new mcsm::Option("./.mcsm/jvm/profiles", optionName);
        cOpt->setValue("path", jvmPath);
        cOpt->setValue("args", jvmOptions);
        cOpt->setValue("server_args", serverOptions);
        this->option.reset(cOpt);
        return;
    }
    mcsm::GlobalOption* opt = new mcsm::GlobalOption("./jvm/profiles", optionName);
    opt->setValue("path", jvmPath);
    opt->setValue("args", jvmOptions);
    opt->setValue("server_args", serverOptions);
    this->option.reset(opt);
}

void mcsm::JvmOption::reset(){
    if(this->option->isGlobal()){
        mcsm::GlobalOption* opt = static_cast<mcsm::GlobalOption*>(this->option.get());
        opt->reset();
    }else{
        mcsm::Option* opt2 = static_cast<mcsm::Option*>(this->option.get());
        opt2->reset();
    }
}

bool mcsm::JvmOption::exists(){
    if(this->option.get() == nullptr) return false;
    return this->option->exists();
}

std::vector<std::string> mcsm::JvmOption::getJvmArguments(){
    std::vector<std::string> empty;
    if(!exists()){
        mcsm::error("This jvm option does not exist.");
        mcsm::error("Please edit the launch profile after creating.");
        std::exit(1);
    }
    if(this->option->isGlobal()){
        mcsm::GlobalOption* opt = static_cast<mcsm::GlobalOption*>(this->option.get());
        if(opt->getValue("args") == nullptr){
            mcsm::error("No \"args\" value specified in file " + opt->getName());
            mcsm::error("Manually editing the launch profile might have caused this issue.");
            mcsm::error("If you know what you're doing, I believe you that you know how to handle this issue.");
            mcsm::error("If you believe that this is a software issue, please report it to GitHub (https://github.com/dodoman8067/mcsm).");
            std::exit(1);
        }
        return opt->getValue("args");
    }else{
        mcsm::Option* opt2 = static_cast<mcsm::Option*>(this->option.get());
        if(opt2->getValue("args") == nullptr){
            mcsm::error("No \"args\" value specified in file " + opt2->getName());
            mcsm::error("Manually editing the launch profile might have caused this issue.");
            mcsm::error("If you know what you're doing, I believe you that you know how to handle this issue.");
            mcsm::error("If you believe that this is a software issue, please report it to GitHub (https://github.com/dodoman8067/mcsm).");
            std::exit(1);
        }
        return opt2->getValue("args");
    }
}

void mcsm::JvmOption::setJvmArguments(const std::vector<std::string>& jvmArgs){
    if(!exists()){
        mcsm::error("This jvm option does not exist.");
        mcsm::error("Please edit the launch profile after creating.");
        std::exit(1);
    }
    if(this->option->isGlobal()){
        mcsm::GlobalOption* opt = static_cast<mcsm::GlobalOption*>(this->option.get());
        opt->setValue("args", jvmArgs);
    }else{
        mcsm::Option* opt2 = static_cast<mcsm::Option*>(this->option.get());
        opt2->setValue("args", jvmArgs);
    }
}

std::string mcsm::JvmOption::getJvmPath(){
    if(!exists()){
        mcsm::error("This jvm option does not exist.");
        mcsm::error("Please edit the launch profile after creating.");
        std::exit(1);
    }
    if(this->option->isGlobal()){
        mcsm::GlobalOption* opt = static_cast<mcsm::GlobalOption*>(this->option.get());
        if(opt->getValue("path") == nullptr){
            mcsm::error("No \"path\" value specified in file " + opt->getName());
            mcsm::error("Manually editing the launch profile might have caused this issue.");
            mcsm::error("If you know what you're doing, I believe you that you know how to handle this issue.");
            mcsm::error("If you believe that this is a software issue, please report it to GitHub (https://github.com/dodoman8067/mcsm).");
            std::exit(1);
        }
        return opt->getValue("path");
    }else{
        mcsm::Option* opt2 = static_cast<mcsm::Option*>(this->option.get());
        if(opt2->getValue("path") == nullptr){
            mcsm::error("No \"path\" value specified in file " + opt2->getName());
            mcsm::error("Manually editing the launch profile might have caused this issue.");
            mcsm::error("If you know what you're doing, I believe you that you know how to handle this issue.");
            mcsm::error("If you believe that this is a software issue, please report it to GitHub (https://github.com/dodoman8067/mcsm).");
            std::exit(1);
        }
        return opt2->getValue("path");
    }
}

void mcsm::JvmOption::setJvmPath(const std::string& jvmPath){
    if(!exists()){
        mcsm::error("This jvm option does not exist.");
        mcsm::error("Please edit the launch profile after creating.");
        std::exit(1);
    }
    if(this->option->isGlobal()){
        mcsm::GlobalOption* opt = static_cast<mcsm::GlobalOption*>(this->option.get());
        opt->setValue("path", jvmPath);
    }else{
        mcsm::Option* opt2 = static_cast<mcsm::Option*>(this->option.get());
        opt2->setValue("path", jvmPath);
    }
}

std::vector<std::string> mcsm::JvmOption::getServerArguments(){
    std::vector<std::string> empty;
    if(!exists()){
        mcsm::error("This jvm option does not exist.");
        mcsm::error("Please edit the launch profile after creating.");
        std::exit(1);
    }
    if(this->option->isGlobal()){
        mcsm::GlobalOption* opt = static_cast<mcsm::GlobalOption*>(this->option.get());
        if(opt->getValue("server_args") == nullptr){
            mcsm::error("No \"server_args\" value specified in file " + opt->getName());
            mcsm::error("Manually editing the launch profile might have caused this issue.");
            mcsm::error("If you know what you're doing, I believe you that you know how to handle this issue.");
            mcsm::error("If you believe that this is a software issue, please report it to GitHub (https://github.com/dodoman8067/mcsm).");
            std::exit(1);
        }
        return opt->getValue("server_args");
    }else{
        mcsm::Option* opt2 = static_cast<mcsm::Option*>(this->option.get());
        if(opt2->getValue("server_args") == nullptr){
            mcsm::error("No \"server_args\" value specified in file " + opt2->getName());
            mcsm::error("Manually editing the launch profile might have caused this issue.");
            mcsm::error("If you know what you're doing, I believe you that you know how to handle this issue.");
            mcsm::error("If you believe that this is a software issue, please report it to GitHub (https://github.com/dodoman8067/mcsm).");
            std::exit(1);
        }
        return opt2->getValue("server_args");
    }
}

void mcsm::JvmOption::setServerArguments(const std::vector<std::string>& serverArgs){
    if(!exists()){
        mcsm::error("This jvm option does not exist.");
        mcsm::error("Please edit the launch profile after creating.");
        std::exit(1);
    }
    if(this->option->isGlobal()){
        mcsm::GlobalOption* opt = static_cast<mcsm::GlobalOption*>(this->option.get());
        opt->setValue("server_args", serverArgs);
    }else{
        mcsm::Option* opt2 = static_cast<mcsm::Option*>(this->option.get());
        opt2->setValue("server_args", serverArgs);
    }
}

std::string mcsm::JvmOption::getProfileName() const {
    return this->name;
}

std::string mcsm::JvmOption::getProfilePath() const {
    if(this->option->isGlobal()){
        mcsm::GlobalOption* opt = static_cast<mcsm::GlobalOption*>(this->option.get());
        return opt->getPath();
    }else{
        mcsm::Option* opt2 = static_cast<mcsm::Option*>(this->option.get());
        return opt2->getPath();
    }
}

mcsm::SearchTarget mcsm::JvmOption::getSearchTarget() const {
    if(this->option->isGlobal()){
        return mcsm::SearchTarget::GLOBAL;
    }else{
        return mcsm::SearchTarget::CURRENT;
    }
}

mcsm::JvmOption::~JvmOption(){

}
