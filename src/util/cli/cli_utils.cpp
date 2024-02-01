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
                return 1;
            }
        #else
            return 1;
        #endif
    }else{
        mcsm::error("This platform is not supported. Please use Windows or Linux.");
        std::exit(1);
    }
}

int mcsm::runCommand(const std::string& command){
    if(mcsm::getCurrentOS() == mcsm::OS::WINDOWS){
        return std::system(command.c_str());
    }else if(mcsm::getCurrentOS() == mcsm::OS::LINUX){
        #ifdef __linux__
            int code = std::system(command.c_str());
            if(WIFEXITED(code)){
                int finalCode = WEXITSTATUS(code);
                return finalCode;
            }else{
                return 1;
            }
        #else
            return 1;
        #endif
    }else{
        mcsm::error("This platform is not supported. Please use Windows or Linux.");
        std::exit(1);
    }
}

std::string mcsm::getCurrentPath(){
    std::error_code ec;
    std::string path = std::filesystem::current_path(ec);
    if(ec){
        mcsm::error("Getting current path operation failed : " + ec.message());
        mcsm::error("If you believe that this is a software issue, please report it to GitHub (https://github.com/dodoman8067/mcsm).");
    }
    return path;
}

bool mcsm::isDebug(){
    #ifdef MCSM_DEBUG
        return true;
    #else
        return false;
    #endif
}