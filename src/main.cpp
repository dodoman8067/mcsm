/*
MIT License

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

#include <mcsm/init.h>
//#include <mcsm/data/options/multi_server_option.h>

const std::string version = "0.2";

int main(int argc, char *argv[]){
    /**
     * TODO
     * Implement mcsm update command.
     * Add multicurl task and make an internal method that returns CURL handle on Server class (obtainJarFile)
     */

    //libssh2 : cmake -B ./build -DBUILD_SHARED_LIBS=OFF -DOPENSSL_USE_STATIC_LIBS=ON -DZLIB_USE_STATIC_LIBS=ON -DENABLE_ZLIB_COMPRESSION=ON -DCRYPTO_BACKEND=OpenSSL
    //libgit2 : cmake -B ./build -DBUILD_SHARED_LIBS=OFF -DOPENSSL_USE_STATIC_LIBS=ON -DZLIB_USE_STATIC_LIBS=ON
    //libcurl-linux : cmake -B ./build -DBUILD_SHARED_LIBS=OFF -DOPENSSL_USE_STATIC_LIBS=ON -DZLIB_USE_STATIC_LIBS=ON
    //libcurl : cmake command : cmake -B ./build -G "MinGW Makefiles" -DBUILD_SHARED_LIBS=OFF -DZLIB_USE_STATIC_LIBS=ON -DCURL_USE_SCHANNEL=ON

    mcsm::init init;
    init.initMCSM(version);

    if(!init.isInitialized()){
        std::cerr << "Program initialization failed.\n";
        std::cerr << "High chance to be a software issue. Please report this to GitHub (https://github.com/dodoman8067/mcsm).\n";
        std::exit(1);
    }

    bool commandFound = false;

    //Prints default message when no arguments
    if(argc < 2){
        std::cout << "Welcome to MCSM (Minecraft Server Manager).\n";
        std::cout << "Type \"mcsm help\" for a list of commands.\n";

        /*
        mcsm::MultiServerOption a(mcsm::getCurrentPath());
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        mcsm::printResultMessage();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_WARN_NOEXIT) std::exit(1);
    }
        a.start().printMessage();
        */
        return 0;
    }

    //Iterates through CommandManager::getCommands() only if argc is more than 1
    for(auto& v : mcsm::CommandManager::getCommands()){
        if(argv[1] == v->getName() || v->hasAlias(argv[1])){
            std::vector<std::string> args;
            for(int i = 2; i < argc; i++){
                args.push_back(argv[i]);
            }
            v->execute(args);
            commandFound = true;
            break;
        }
    }

    //Exit if unknown command detected.
    if(!commandFound){
        std::cerr << "Unknown command \"" << argv[1] << "\". " << "Type \'mcsm help\' for a list of commands.\n";
        return 1;
    }
    
    return 0;
}