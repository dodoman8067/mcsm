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
            if(!server->hasVersion(version)){
                mcsm::error("Unsupported version.");
                mcsm::error("Please try again with a different version.");
                std::exit(1);
            }
            mcsm::ServerOption serverOption(version, server);
            configure(serverOption, name, option);
            break;
        }
        case mcsm::BukkitServerType::PURPUR: {
            std::shared_ptr<mcsm::PurpurServer> server = std::make_shared<mcsm::PurpurServer>();
            if(!server->hasVersion(version)){
                mcsm::error("Unsupported version.");
                mcsm::error("Please try again with a different version.");
                std::exit(1);
            }
            mcsm::ServerOption serverOption(version, server);
            configure(serverOption, name, option);
            break;
        }
        default: {
            mcsm::error("This kind of Bukkit-API based server implementation is not supported right now.");
            std::exit(1);
            break;
        }
    }
}

void mcsm::server::generateVanilla(const std::string& name, mcsm::JvmOption& option, const std::string& version){
    std::shared_ptr<mcsm::VanillaServer> server = std::make_shared<mcsm::VanillaServer>();
    if(!server->hasVersion(version)){
        mcsm::error("Unsupported version.");
        mcsm::error("Please try again with a different version.");
        std::exit(1);
    }
    mcsm::ServerOption serverOption(version, server);
    configure(serverOption, name, option);    
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
        mcsm::error("Server is already configured in this directory.");
        mcsm::error("Try in another directory.");
        std::exit(1);
    }
    serverOption.create(name, option);
    mcsm::success("Configured server information : ");
    mcsm::info("Server name : " + serverOption.getServerName());
    mcsm::info("Server type : " + serverOption.getServerType());
    mcsm::info("Server version : " + serverOption.getServerVersion());
    mcsm::info("Server JVM launch profile : " + serverOption.getDefaultOption()->getProfileName());
}

std::shared_ptr<mcsm::Server> mcsm::server::detectServerType(const std::string& server){
    std::shared_ptr<mcsm::Server> sPtr;

    if(server == "paper"){
        sPtr = std::make_shared<mcsm::PaperServer>();
    }
    if(server == "purpur"){
        sPtr = std::make_shared<mcsm::PurpurServer>();
    }
    if(server == "vanilla"){
        sPtr = std::make_shared<mcsm::VanillaServer>();
    }
    if(sPtr == nullptr){
        mcsm::warning("The following server type (" + server + ") is not supported.");
        mcsm::warning("Task aborted.");
        std::exit(1); 
    }

    return sPtr;
}