#include <mcsm/server/server_group_manager.h>

mcsm::ServerGroupManager::ServerGroupManager(std::unique_ptr<mcsm::ServerGroupLoader> group){
    this->group = std::move(group);
}

mcsm::ServerGroupManager::~ServerGroupManager(){

}

mcsm::Result mcsm::ServerGroupManager::start(){

}

mcsm::Result mcsm::ServerGroupManager::start(const std::string& serverName){
    std::string mode = this->group->getMode();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    if(mode == "screen"){
        std::string groupName = this->group->getName();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
            std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
            mcsm::Result res(resp.first, resp.second);
            return res;
        }

        std::vector<const mcsm::ServerConfigLoader*> servers = this->group->getServers();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
            std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
            mcsm::Result res(resp.first, resp.second);
            return res;
        }

        std::string groupPath = this->group->getHandle()->getPath();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
            std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
            mcsm::Result res(resp.first, resp.second);
            return res;
        }

        for(const mcsm::ServerConfigLoader* server : servers){
            if(server == nullptr){
                return {mcsm::ResultType::MCSM_FAIL, {"Null server config loader found in ServerGroupLoader's servers.", "Please report this to Github and explain how did you get this message."}};
            }

            std::string groupServerName = server->getServerName();
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
                std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
                mcsm::Result res(resp.first, resp.second);
                return res;
            }

            if(serverName == groupServerName){
                mcsm::ScreenSession session(groupName + "." + groupServerName, "{mcsm start(gonna replace this with argv[0] or something)} -__mcsm__Internal_Group_Start \"" + groupPath + "\"");
                // StartServerCommand will handle the rest
                return session.start();
            }
        }

        return {mcsm::ResultType::MCSM_WARN, {"Server named \"" + serverName + "\" on group not found."}};
    }else{
        // TODO
    }
}

mcsm::Result mcsm::ServerGroupManager::stop(){

}

mcsm::Result mcsm::ServerGroupManager::stop(const std::string& serverName){

}