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
    mcsm::ServerGroupLoader gLoader(groupOptionPath);
    mcsm::Result gLoadRes = gLoader.load();
    if(gLoadRes.getResult() != mcsm::ResultType::MCSM_OK && gLoadRes.getResult() != mcsm::ResultType::MCSM_SUCCESS){
        gLoadRes.printMessage();
        if(gLoadRes.getResult() != mcsm::ResultType::MCSM_WARN_NOEXIT) std::exit(1);
    }

    std::vector<const mcsm::ServerConfigLoader*> gServers = gLoader.getServers();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        mcsm::printResultMessage();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_WARN_NOEXIT) std::exit(1);
    }

    for(const mcsm::ServerConfigLoader* gServer : gServers){
        if(gServer == nullptr) continue;
        if(gServer->getHandle()->getPath() != mcsm::getCurrentPath()) continue;
        // add server session file
        return startServer(option, path, optionPath);
        // remove server session file
    }

    return {mcsm::ResultType::MCSM_FAIL, {"Failed to validate server group " + groupOptionPath}};
}