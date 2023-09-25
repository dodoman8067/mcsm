#include "jvm_option.h"

mcsm::JvmOption::JvmOption(const std::string& name) : JvmOption(name, mcsm::SearchTarget::ALL) {}

mcsm::JvmOption::JvmOption(const std::string& name, const mcsm::SearchTarget& target){
    this->name = name;
    switch (target) {
        case mcsm::SearchTarget::ALL: {
            mcsm::GlobalOption globalOption(".jvm.profiles", name);
            if(globalOption.exists()){
                this->option.reset(new mcsm::GlobalOption(".jvm.profiles", name));
            } else {
                this->option.reset(new mcsm::Option(".jvm.profiles", name));
            }
            break;
        }
        case mcsm::SearchTarget::GLOBAL: {
            mcsm::GlobalOption globalOption(".jvm.profiles", name);
            if(globalOption.exists()){
                this->option.reset(new mcsm::GlobalOption(".jvm.profiles", name));
            }
            break;
        }
        case mcsm::SearchTarget::CURRENT: {
            mcsm::Option option(".jvm.profiles", name);
            if(option.exists()){
                this->option.reset(new mcsm::Option(".jvm.profiles", name));
            }
            break;
        }
    }
}

void mcsm::JvmOption::create(){
    std::string jvm = mcsm::detectJava();
    std::vector<std::string> jvmArgs = {
        "-Xms2G",
        "-Xmx2G"
    };
    std::vector<std::string> serverArgs = {
        "nogui"
    };
    if(mcsm::isWhitespaceOrEmpty(jvm)){
        std::cerr << "Error: JVM detection failed.\n";
        std::cerr << "This error can be fixed by setting your JAVA_HOME enviroment variable.\n";
        std::cerr << "Or if you believe this is a software issue, please report it to GitHub. (https://github.com/dodoman8067/mcsm)\n";
        std::cerr << "\n";
        std::cerr << "Error called from : mcsm::JvmOption::create()\n";
        std::exit(1);
    }
    create(std::move(jvm), std::move(jvmArgs), std::move(serverArgs), mcsm::SearchTarget::GLOBAL);
}

void mcsm::JvmOption::create(const std::string& jvmPath, const mcsm::SearchTarget& target){
    std::vector<std::string> jvmArgs = {
        "-Xms2G",
        "-Xmx2G"
    };
    std::vector<std::string> serverArgs = {
        "nogui"
    };
    create(std::move(jvmPath), std::move(jvmArgs), std::move(serverArgs), std::move(target));
}

void mcsm::JvmOption::create(const std::string& jvmPath, const std::vector<std::string>& jvmOptions, const mcsm::SearchTarget& target){
    std::vector<std::string> serverArgs = {
        "nogui"
    };
    create(std::move(jvmPath), std::move(jvmOptions), std::move(serverArgs), std::move(target));
}

void mcsm::JvmOption::create(const std::string& jvmPath, const std::vector<std::string>& jvmOptions, const std::vector<std::string>& serverOptions, const mcsm::SearchTarget& target){
    std::string filePath;
    std::string optionName;

    if(target == mcsm::SearchTarget::CURRENT){
        mcsm::Option option(".jvm.profiles", this->name);
        filePath = option.getPath();
        optionName = option.getName();
    }else{
        mcsm::GlobalOption globalOption(".jvm.profiles", this->name);
        filePath = globalOption.getPath();
        optionName = globalOption.getName();
    }

    if(std::filesystem::exists(filePath + "/" + optionName)){
        std::filesystem::remove(filePath + "/" + optionName);
    }

    if(target == mcsm::SearchTarget::ALL) return;
    if(target == mcsm::SearchTarget::CURRENT){
        mcsm::Option* cOpt = new mcsm::Option(".jvm.profiles", optionName);
        cOpt->setValue("path", jvmPath);
        cOpt->setValue("args", jvmOptions);
        cOpt->setValue("server_args", serverOptions);
        this->option.reset(cOpt);
        return;
    }
    mcsm::GlobalOption* opt = new mcsm::GlobalOption(".jvm.profiles", optionName);
    opt->setValue("path", jvmPath);
    opt->setValue("args", jvmOptions);
    opt->setValue("server_args", serverOptions);
    this->option.reset(opt);
}

void mcsm::JvmOption::reset(){
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
            std::cerr << "Error called from : mcsm::JvmOption::reset()\n";
            std::exit(1);
        }
        opt2->reset();
    }
    opt->reset();
}

bool mcsm::JvmOption::exists(){
    if(this->option.get() == nullptr) return false;
    return this->option->exists();
}

std::vector<std::string> mcsm::JvmOption::getJvmArguments(){
    std::vector<std::string> empty;
    if(!exists()) return empty;
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
        // I debugged here for more than 2 hours because returning "path" instead of "args"
        if(opt2->getValue("args") == nullptr) return empty;
        return opt2->getValue("args");
    }
    if(opt->getValue("args") == nullptr) return empty;
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

std::vector<std::string> mcsm::JvmOption::getServerArguments(){
    std::vector<std::string> empty;
    if(!exists()) return empty;
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
        if(opt2->getValue("server_args") == nullptr) return empty;
        return opt2->getValue("server_args");
    }
    if(opt->getValue("server_args") == nullptr) return empty;
    return opt->getValue("server_args");  
}

std::string mcsm::JvmOption::getProfileName() const {
    return this->name;
}

mcsm::JvmOption::~JvmOption(){

}