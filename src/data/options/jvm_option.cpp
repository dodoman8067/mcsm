#include "jvm_option.h"

mcsm::JvmOption::JvmOption(const std::string& name){
    JvmOption(name, mcsm::SearchTarget::ALL);
}


mcsm::JvmOption::JvmOption(const std::string& name, const mcsm::SearchTarget& target){
    this->name = name;
    switch (target) {
        case mcsm::SearchTarget::ALL: {
            mcsm::GlobalOption globalOption(".jvm.profiles", name);
            if(globalOption.exists()){
                this->option.reset(new mcsm::GlobalOption(".jvm.profiles", name));
                break;
            }

            mcsm::Option option(".jvm.profiles", name);
            if(option.exists()){
                this->option.reset(new mcsm::Option(".jvm.profiles", name));   
                break;
            }
            break;
        }
        case mcsm::SearchTarget::GLOBAL: {
            mcsm::GlobalOption globalOption(".jvm.profiles", name);
            if(!globalOption.exists()) break;
            this->option.reset(new mcsm::GlobalOption(".jvm.profiles", name));
            break;
        }
        case mcsm::SearchTarget::CURRENT: {
            mcsm::Option option(".jvm.profiles", name);
            if(!option.exists()) break;
            this->option.reset(new mcsm::Option(".jvm.profiles", name));
            break;
        }
    }
}

void mcsm::JvmOption::create(){
    std::string jvm = mcsm::detectJava();
    if(mcsm::isWhitespaceOrEmpty(jvm)){
        std::cerr << "Error: JVM detection failed.\n";
        std::cerr << "This error can be fixed by setting your JAVA_HOME enviroment variable.\n";
        std::cerr << "Or if you believe this is a software issue, please report it to GitHub. (https://github.com/dodoman8067/mcsm)\n";
        std::cerr << "\n";
        std::cerr << "Error called from : mcsm::JvmOption::create()\n";
        std::exit(1);
    }
    create(jvm, "-Xms2G -Xmx2G", "nogui", mcsm::SearchTarget::GLOBAL);
}

void mcsm::JvmOption::create(const std::string& jvmPath, const mcsm::SearchTarget& target){
    create(jvmPath, "-Xms2G -Xmx2G", "nogui", target);
}

void mcsm::JvmOption::create(const std::string& jvmPath, const std::string& jvmOptions, const mcsm::SearchTarget& target){
    create(jvmPath, jvmOptions, "nogui", target);
}

void mcsm::JvmOption::create(const std::string& jvmPath, const std::string& jvmOptions, const std::string& serverOptions, const mcsm::SearchTarget& target){
    if(exists()) return;
    if(target == mcsm::SearchTarget::ALL) return;
    if(target == mcsm::SearchTarget::CURRENT){
        mcsm::Option* cOpt = new mcsm::Option(".jvm.profiles", this->name);
        cOpt->setValue("path", jvmPath);
        cOpt->setValue("args", jvmOptions);
        cOpt->setValue("server_args", serverOptions);
        this->option.reset(cOpt);
        return;
    }
    mcsm::GlobalOption* opt = new mcsm::GlobalOption(".jvm.profiles", this->name);
    opt->setValue("path", jvmPath);
    opt->setValue("args", jvmOptions);
    opt->setValue("server_args", serverOptions);
    this->option.reset(opt);
}

bool mcsm::JvmOption::exists(){
    if(this->option.get() == nullptr) return false;
    return this->option->exists();
}

std::string mcsm::JvmOption::getJvmArguments(){
    if(!exists()) return std::move("");
    mcsm::GlobalOption* opt = dynamic_cast<mcsm::GlobalOption*>(this->option.get());
    if(!opt){
        mcsm::Option* opt2 = dynamic_cast<mcsm::Option*>(this->option.get());
        if(!opt2){
            std::cerr << "Error: Both dynamic_cast calls from mcsm::Configurable to mcsm::GlobalOption and mcsm::Option have failed.\n";
            std::cerr << "Normally, one of these casts should be successful, as both classes extend mcsm::Configurable.\n";
            std::cerr << "If you see this error, this might be a operating system issue or software's issue.\n";
            std::cerr << "If you believe that this is a software issue, try downloading a new copy of program in GitHub. If that isn't solving the issue, please report to GitHub. (https://github.com/dodoman8067/mcsm)\n";
            std::cerr << "Or if you believe that this is operating system's issue, try updating the operating system. If it's not working, try reinstalling the operating system completely.\n";
            std::cerr << "\n";
            std::cerr << "Error called from : mcsm::JvmOption::getJvmArguments()\n";
            std::exit(1);
        }
        if(opt2->getValue("path") == nullptr) return std::move("");
        return opt2->getValue("path");
    }
    if(opt->getValue("args") == nullptr) return std::move("");
    return opt->getValue("args");
}

std::string mcsm::JvmOption::getJvmPath(){
    if(!exists()) return std::move("");
    mcsm::GlobalOption* opt = dynamic_cast<mcsm::GlobalOption*>(this->option.get());
    if(!opt){
        mcsm::Option* opt2 = dynamic_cast<mcsm::Option*>(this->option.get());
        if(!opt2){
            std::cerr << "Error: Both dynamic_cast calls from mcsm::Configurable to mcsm::GlobalOption and mcsm::Option have failed.\n";
            std::cerr << "Normally, one of these casts should be successful, as both classes extend mcsm::Configurable.\n";
            std::cerr << "If you see this error, this might be a operating system issue or software's issue.\n";
            std::cerr << "If you believe that this is a software issue, try downloading a new copy of program in GitHub. If that isn't solving the issue, please report to GitHub. (https://github.com/dodoman8067/mcsm)\n";
            std::cerr << "Or if you believe that this is operating system's issue, try updating the operating system. If it's not working, try reinstalling the operating system completely.\n";
            std::cerr << "\n";
            std::cerr << "Error called from : mcsm::JvmOption::getJvmPath()\n";
            std::exit(1);
        }
        if(opt2->getValue("path") == nullptr) return std::move("");
        return opt2->getValue("path");
    }
    if(opt->getValue("path") == nullptr) return std::move("");
    return opt->getValue("path");
}

std::string mcsm::JvmOption::getServerArguments(){
    if(!exists()) return std::move("");
    mcsm::GlobalOption* opt = dynamic_cast<mcsm::GlobalOption*>(this->option.get());
    if(!opt){
        mcsm::Option* opt2 = dynamic_cast<mcsm::Option*>(this->option.get());
        if(!opt2){
            std::cerr << "Error: Both dynamic_cast calls from mcsm::Configurable to mcsm::GlobalOption and mcsm::Option have failed.\n";
            std::cerr << "Normally, one of these casts should be successful, as both classes extend mcsm::Configurable.\n";
            std::cerr << "If you see this error, this might be a operating system issue or software's issue.\n";
            std::cerr << "If you believe that this is a software issue, try downloading a new copy of program in GitHub. If that isn't solving the issue, please report to GitHub. (https://github.com/dodoman8067/mcsm)\n";
            std::cerr << "Or if you believe that this is operating system's issue, try updating the operating system. If it's not working, try reinstalling the operating system completely.\n";
            std::cerr << "\n";
            std::cerr << "Error called from : mcsm::JvmOption::getServerArguments()\n";
            std::exit(1);
        }
        if(opt2->getValue("server_args") == nullptr) return std::move("");
        return opt2->getValue("server_args");
    }
    if(opt->getValue("server_args") == nullptr) return std::move("");
    return opt->getValue("server_args");  
}

mcsm::JvmOption::~JvmOption(){

}
