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
#include <mcsm/data/options/server_process.h>
#include <random>

const std::string version = "0.2";

std::mutex coutMutex;

void runServerProcess(int id, const std::string& workingPath) {
std::random_device rd; 
    std::mt19937 mt(rd()); 
    std::uniform_int_distribution<int> dist(0, 99);
    int a = dist(mt);
    std::string command = "PowerShell -Command \"Start-Sleep -Seconds " + std::to_string(a);
    
    mcsm::ServerProcess myProcess(command, workingPath);
    {
        std::lock_guard<std::mutex> lock(coutMutex);
        std::cout << "Starting process " << id << " with sleep time " << a << " seconds.\n";
    }
    
    auto startResult = myProcess.start();
    if (startResult.getResult() != mcsm::ResultType::MCSM_SUCCESS) {
        std::lock_guard<std::mutex> lock(coutMutex);
        std::cerr << "Process " << id << " failed to start: " << std::endl;
        return;
    }

    auto waitResult = myProcess.waitForCompletion();
    {
        std::lock_guard<std::mutex> lock(coutMutex);
    if (waitResult.getResult() != mcsm::ResultType::MCSM_SUCCESS) {
        std::cerr << "Process " << id << " error waiting" << std::endl;
    } else {
        std::cout << "Process " << id << " completed " << std::endl;
    }
    }
}

int main(int argc, char *argv[]){
    /**
     * TODO
     * Implement mcsm update command.
     */

    //libssh2 : cmake -B ./build -DBUILD_SHARED_LIBS=OFF -DOPENSSL_USE_STATIC_LIBS=ON -DZLIB_USE_STATIC_LIBS=ON -DENABLE_ZLIB_COMPRESSION=ON -DCRYPTO_BACKEND=OpenSSL
    //libgit2 : cmake -B ./build -DBUILD_SHARED_LIBS=OFF -DOPENSSL_USE_STATIC_LIBS=ON -DZLIB_USE_STATIC_LIBS=ON
    //libcurl-linux : cmake -B ./build -DBUILD_SHARED_LIBS=OFF -DOPENSSL_USE_STATIC_LIBS=ON -DZLIB_USE_STATIC_LIBS=ON
    //libcurl : cmake command : cmake -B ./build -G "MinGW Makefiles" -DBUILD_SHARED_LIBS=OFF -DZLIB_USE_STATIC_LIBS=ON -DCURL_USE_SCHANNEL=ON

    mcsm::init init;
    init.initMCSM(version);

    if(!init.isInitialized()){
        std::cerr << "Program initialization failed.\n";
        std::cerr << "High chance to be a software issue. Please report it to GitHub (https://github.com/dodoman8067/mcsm).\n";
        std::exit(1);
    }

    bool commandFound = false;

    //Prints default message when no arguments
    if(argc < 2){
        std::cout << "Welcome to MCSM (Minecraft Server Manager).\n";
        std::cout << "Type \"mcsm help\" for a list of commands.\n";

            const int numberOfProcesses = 5; // Number of server processes to start
    std::vector<std::thread> threads; // Vector to hold threads
    std::string workingPath = "/"; // Set to appropriate directory

    // Start multiple server processes
    for (int i = 1; i <= numberOfProcesses; i++) {
        threads.emplace_back(runServerProcess, i, workingPath);
    }

    // Wait for all processes to complete
    for (auto& thread : threads) {
        thread.join();
    }

    std::cout << "All processes completed." << std::endl;
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