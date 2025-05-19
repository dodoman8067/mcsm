#include <mcsm/server/server_group_manager.h>
#ifdef __linux__
    #include <unistd.h>
#endif
#ifdef _WIN32
    #include <windows.h>
#endif

mcsm::ServerGroupManager::ServerGroupManager(std::unique_ptr<mcsm::ServerGroupLoader> group){
    this->group = std::move(group);
}

mcsm::ServerGroupManager::~ServerGroupManager(){

}

mcsm::VoidResult mcsm::ServerGroupManager::start(){
    std::string exePath = mcsm::getExecutablePath();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

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

            std::string groupServerPath = server->getServerName();
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
                std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
                mcsm::Result res(resp.first, resp.second);
                return res;
            }

            std::string modified = mcsm::safeString(groupServerPath); // session names are groupname.(server path with / replaced by .))

            std::error_code ec;
            std::filesystem::current_path(server->getHandle()->getPath(), ec);
            if(mcsm::isDebug()) mcsm::info("Work path changed to: " + server->getHandle()->getPath());
            if(ec){
                return {mcsm::ResultType::MCSM_FAIL, {"Server starting failed with reason: " + ec.message()}}; 
            }

            mcsm::ScreenSession session(groupName + "." + modified, exePath + " start -__mcsm__Internal_Group_Start \"" + groupPath + "\"");
            // StartServerCommand will handle the rest
            if(mcsm::isDebug()) mcsm::info("Start command: " + session.getCommand());
            return session.start();
        }
    }else{
        // TODO
    }
    return {mcsm::ResultType::MCSM_SUCCESS, {"Success"}};
}

mcsm::VoidResult mcsm::ServerGroupManager::start(const std::string& serverPath){
    std::string exePath = mcsm::getExecutablePath(); // to execute mcsm start later
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

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

        // iterates through servers and starts the requested server
        for(const mcsm::ServerConfigLoader* server : servers){
            if(server == nullptr){
                return {mcsm::ResultType::MCSM_FAIL, {"Null server config loader found in ServerGroupLoader's servers.", "Please report this to Github and explain how did you get this message."}};
            }

            std::string groupServerPath = server->getHandle()->getPath();
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
                std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
                mcsm::Result res(resp.first, resp.second);
                return res;
            }
            
            std::string modified = mcsm::safeString(groupServerPath); // session names are groupname.(server path with / replaced by .))

            if(serverPath == groupServerPath){ // true means this is the requested server
                std::error_code ec;
                std::filesystem::current_path(server->getHandle()->getPath(), ec);
                if(mcsm::isDebug()) mcsm::info("[DEBUG] Work path changed to: " + server->getHandle()->getPath());
                if(ec){
                    return {mcsm::ResultType::MCSM_FAIL, {"Server starting failed with reason: " + ec.message()}}; 
                }

                mcsm::ScreenSession session(groupName + "." + modified, exePath + " start -__mcsm__Internal_Group_Start \"" + groupPath + "\"");
                mcsm::info("[DEBUG] Checking if session is running for: " + serverPath + " with session name: " + session.getFullSessionName());
                if(session.isRunning()){
                    return {mcsm::ResultType::MCSM_WARN, {"Server \"" + serverPath + "\" is already running."}};
                }
                // StartServerCommand will handle the rest
                if(mcsm::isDebug()) mcsm::info("[DEBUG] Start command: " + session.getCommand());
                return session.start();
            }
        }

        return {mcsm::ResultType::MCSM_WARN, {"Server on \"" + serverPath + "\" not found on current group configuration."}};
    }else{
        // TODO
    }
    return {mcsm::ResultType::MCSM_SUCCESS, {"Success"}};
}

mcsm::VoidResult mcsm::ServerGroupManager::stop(){
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

        for(const mcsm::ServerConfigLoader* server : servers){
            if(server == nullptr){
                return {mcsm::ResultType::MCSM_FAIL, {"Null server config loader found in ServerGroupLoader's servers.", "Please report this to Github and explain how did you get this message."}};
            }

            std::string groupServerPath = server->getServerName();
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
                std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
                mcsm::Result res(resp.first, resp.second);
                return res;
            }

            std::string modified = mcsm::safeString(groupServerPath); // session names are groupname.(server path with / replaced by .))

            mcsm::ScreenSession session(groupName + "." + modified);
            if(!session.isRunning()){ // replace with runningsessionsoption#isrunning
                return {mcsm::ResultType::MCSM_FAIL, {"Cannot stop a session not running. ID: " + session.getFullSessionName()}};
            }
            return session.sendCommand("stop\n");
        }

    }else{
        // TODO
    }

    return {mcsm::ResultType::MCSM_SUCCESS, {"Success"}};
}

mcsm::VoidResult mcsm::ServerGroupManager::stop(const std::string& serverPath){
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

        for(const mcsm::ServerConfigLoader* server : servers){
            if(server == nullptr){
                return {mcsm::ResultType::MCSM_FAIL, {"Null server config loader found in ServerGroupLoader's servers.", "Please report this to Github and explain how did you get this message."}};
            }

            std::string groupServerPath = server->getHandle()->getPath();
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
                std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
                mcsm::Result res(resp.first, resp.second);
                return res;
            }
            std::string modified = mcsm::safeString(groupServerPath); // session names are groupname.(server path with / replaced by .))

            if(serverPath == groupServerPath){
                mcsm::ScreenSession session(groupName + "." + modified);
                if(!session.isRunning()){ // todo: replace with runningsessionsoption#isrunning
                    return {mcsm::ResultType::MCSM_FAIL, {"Cannot stop a session not running. ID: " + session.getFullSessionName()}};
                }
                return session.sendCommand("stop\n");
            }
        }

        return {mcsm::ResultType::MCSM_WARN, {"Server on \"" + serverPath + "\" not found on current group configuration."}};
    }else{
        // TODO
    }

    return {mcsm::ResultType::MCSM_SUCCESS, {"Success"}};
}

mcsm::IntResult mcsm::ServerGroupManager::getRunningSessions() const {
    std::string mode = this->group->getMode();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return -1;
    int count = 0;

    if(mode == "screen"){
        std::string groupName = this->group->getName();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return -1;

        std::vector<const mcsm::ServerConfigLoader*> servers = this->group->getServers();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return -1;
        
        // checks running sessions (session names are groupname.(server path with / replaced by .)) and increases count.
        for(const mcsm::ServerConfigLoader* server : servers){
            if(server == nullptr){
                mcsm::Result res(mcsm::ResultType::MCSM_FAIL, {"Null server config loader found in ServerGroupLoader's servers.", "Please report this to Github and explain how did you get this message."});
                return -1;
            }

            std::string groupServerPath = server->getServerName();
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return -1;
            
            mcsm::ScreenSession session(groupName + "." + groupServerPath);
            if(!session.isRunning()){
                continue;
            }

            mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
            count++;
        }

        return count;
    }else{
        // TODO
    }

    return -1;
}

tl::expected<std::vector<const mcsm::ServerConfigLoader*>, mcsm::Error> mcsm::ServerGroupManager::getRunningServers() const {
    std::string mode = this->group->getMode();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return {};
    std::vector<const mcsm::ServerConfigLoader*> vec;

    if(mode == "screen"){
        std::string groupName = this->group->getName();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return {};

        std::vector<const mcsm::ServerConfigLoader*> servers = this->group->getServers();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return {};

        // checks running sessions (session names are groupname.(server path with / replaced by .)) and adds them to vec.
        for(const mcsm::ServerConfigLoader* server : servers){
            if(server == nullptr){
                mcsm::Result res(mcsm::ResultType::MCSM_FAIL, {"Null server config loader found in ServerGroupLoader's servers.", "Please report this to Github and explain how did you get this message."});
                return {};
            }

            std::string groupServerPath = server->getHandle()->getPath();
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return {};
            
            mcsm::ScreenSession session(groupName + "." + mcsm::safeString(groupServerPath));
            if(!session.isRunning()){
                continue;
            }

            vec.push_back(server);
        }

        mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
        return vec;
    }else{
        // TODO
    }

    return {};
}