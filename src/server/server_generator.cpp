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
#include <mcsm/server/server_generator.h>

void mcsm::server::generateBukkit(const std::string& name, mcsm::JvmOption& option, const std::string& version, const mcsm::BukkitServerType& type){
    switch(type){
        case mcsm::BukkitServerType::PAPER: {
            std::shared_ptr<mcsm::PaperServer> server = std::make_shared<mcsm::PaperServer>();
            mcsm::ServerOption serverOption(version, server);
            configure(serverOption, name, option);
            break;
        }
        default: {
            std::cerr << "[mcsm] This kind of Bukkit-API based server implementation is not supported right now.\n";
            std::exit(1);
            break;
        }
    }
}

void mcsm::server::generateForge(const std::string& /* name */, mcsm::JvmOption& /* option */, const std::string& /* version */){

}

void mcsm::server::generateFabric(const std::string& /* name */, mcsm::JvmOption& /* option */, const std::string& /* version */){

}

void mcsm::server::generateSponge(const std::string& /* name */, mcsm::JvmOption& /* option */, const std::string& /* version */){

}

void mcsm::server::generateCustom(const std::string& /* name */, mcsm::JvmOption& /* option */, const std::string& /* filePath */, const std::string& /* version */){

}

void mcsm::server::configure(mcsm::ServerOption& serverOption, const std::string& name, mcsm::JvmOption& option){
    if(serverOption.exists()){
        std::cerr << "[mcsm] Error: Server is already configured in this directory.\n";
        std::cerr << "[mcsm] Try in another directory.\n";\
        std::exit(1);
    }
    serverOption.create(name, option);
    std::cout << "[mcsm] Configured server information :\n";
    std::cout << "[mcsm] Server name : " << serverOption.getServerName() << "\n";
    std::cout << "[mcsm] Server type : " << serverOption.getServerType() << "\n";
    std::cout << "[mcsm] Server version : " << serverOption.getServerVersion() << "\n";
    std::cout << "[mcsm] Server JVM launch profile : " << serverOption.getDefaultOption()->getProfileName() << "\n";
}