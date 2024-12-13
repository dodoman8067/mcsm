#include <mcsm/data/options/server_starter.h>

mcsm::ServerStarter::ServerStarter(mcsm::ServerConfigLoader* loader){
    this->loader = loader;
}
        
mcsm::ServerStarter::~ServerStarter(){

}

mcsm::Result mcsm::ServerStarter::startServer(mcsm::JvmOption& option, const std::string& path, const std::string& optionPath){
    mcsm::ServerDataOption serverDataOpt(optionPath);
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    mcsm::Result sLoadRes = serverDataOpt.load();
    if(!sLoadRes.isSuccess()) return sLoadRes;

    bool fileExists = mcsm::fileExists(optionPath + "/server.json");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    if(!fileExists){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverNotConfigured(optionPath)});
        return res;
    }

    mcsm::Server* server = this->loader->getServerInstance();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    const std::string& name = this->loader->getServerName();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    const std::string& version = this->loader->getServerVersion();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    const std::string& profileName = option.getProfileName();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    mcsm::success("Starting server..");
    mcsm::info("Server name : " + name);
    mcsm::info("Server MC version : " + version);
    mcsm::info("Server JVM launch profile : " + profileName);
    mcsm::Result res = serverDataOpt.updateLastTimeLaunched();
    if(!res.isSuccess()) return res;
    mcsm::Result res2 = server->start(this->loader, option, path, optionPath);
    return res2;
}

mcsm::Result mcsm::ServerStarter::startServer(mcsm::JvmOption& option, const std::string& path, const std::string& optionPath, const std::string& groupOptionPath){
    // Doesn't do anything now.
    // Will verify if the group option is valid and if it has current server added on the list then update running sessions file

    startServer(option, path, optionPath);

    // Call remove server from running sessions file here
}