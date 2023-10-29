#include <mcsm/util/cli/cli_utils.h>

int mcsm::runCommandQuietly(const std::string& command){
    if(mcsm::getCurrentOS() == mcsm::OS::WINDOWS){
        std::string cmd = command + " > NUL 2>&1";
        return std::system(cmd.c_str());
    }else if(mcsm::getCurrentOS() == mcsm::OS::LINUX){
        std::string cmd = command + " > /dev/null 2>&1";
        return std::system(cmd.c_str());
    }else{
        std::cerr << "[mcsm] This platform is not supported. Please use Windows or Linux.\n";
        std::exit(1);
    }
}

int mcsm::runCommand(const std::string& command){
    if(mcsm::getCurrentOS() == mcsm::OS::WINDOWS || mcsm::getCurrentOS() == mcsm::OS::LINUX){
        return std::system(command.c_str());
    }else{
        std::cerr << "[mcsm] This platform is not supported. Please use Windows or Linux.\n";
        std::exit(1);
    }
}