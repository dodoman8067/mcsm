#include <mcsm/server/server.h>

void mcsm::Server::start(mcsm::JvmOption& option){
    std::string jvmOpt = " ";
    for(auto& s : option.getJvmArguments()){
        jvmOpt = jvmOpt + s + " ";
    }

    std::string svrOpt = " ";
    for(auto& s : option.getServerArguments()){
        svrOpt = svrOpt + s + " ";
    }

    std::string command = option.getJvmPath() + jvmOpt + this->getJarFile() + svrOpt;
    mcsm::info("Running command : " + command);
    mcsm::runCommand(command.c_str());
}

std::string mcsm::Server::getJarFile() const {
    mcsm::Option opt(".", "server");
    if(opt.exists() && opt.getValue("server_jar") != nullptr){
        if(!opt.getValue("server_jar").is_string()){
            mcsm::error("Value \"server_jar\" has to be a string, but it's not.");
            mcsm::error("Manually editing the launch profile might have caused this issue.");
            mcsm::error("If you know what you're doing, I believe you that you know how to handle this issue.");
            mcsm::error("If you believe that this is a software issue, please report it to GitHub (https://github.com/dodoman8067/mcsm).");
            std::exit(1);
        }
        return opt.getValue("server_jar");
    }
    return getTypeAsString() + ".jar";
}

bool mcsm::Server::isBasedAs(const std::string& input) const {
    if(!mcsm::isWhitespaceOrEmpty(getBasedServer())) return false;
    return getBasedServer() == input;
}