#include <mcsm/util/cli/cli_utils.h>

int mcsm::runCommandQuietly(const std::string& command){
    if(mcsm::getCurrentOS() == mcsm::OS::WINDOWS){
        std::string cmd = command + " > NUL 2>&1";
        return std::system(cmd.c_str());
    }else if(mcsm::getCurrentOS() == mcsm::OS::LINUX){
        std::string cmd = command + " > /dev/null 2>&1";
        #ifdef __linux__
            int code = std::system(cmd.c_str());
            if(WIFEXITED(code)){
                int finalCode = WEXITSTATUS(code);
                return finalCode;
            }else{
                std::cerr << "[mcsm] Command " << cmd << " failed.\n";
                std::cerr << "If you believe this is a bug, please report it to GitHub (https://github.com/dodoman8067/mcsm).";
            }
        #endif
        return 1;
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