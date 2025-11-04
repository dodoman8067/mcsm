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
    auto exePath = mcsm::getExecutablePath(); // to execute mcsm start later
    if(!exePath) return tl::unexpected(exePath.error());

    auto mode = this->group->getMode();
    if(!mode) return tl::unexpected(mode.error());

    if(mode.value() == "screen"){
        auto groupName = this->group->getName();
        if(!groupName) return tl::unexpected(groupName.error());

        std::vector<const mcsm::ServerConfigLoader*> servers = this->group->getServers();

        std::string groupPath = this->group->getHandle()->getPath();

        for(const mcsm::ServerConfigLoader* server : servers){
            if(server == nullptr){
                auto customTemp = mcsm::errors::INTERNAL_FUNC_EXECUTION_FAILED;
                customTemp.message = "Null server config loader found in ServerGroupLoader's servers.";
                mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, customTemp, {});
                return tl::unexpected(err);
            }

            auto groupServerPath = server->getServerName();
            if(!groupServerPath) return tl::unexpected(groupServerPath.error());

            std::string modified = mcsm::safeString(groupServerPath.value()); // session names are groupname.(server path with / replaced by .))

            std::error_code ec;
            std::filesystem::current_path(server->getHandle()->getPath(), ec);
            if(mcsm::isDebug()) mcsm::info("Work path changed to: " + server->getHandle()->getPath());
            if(ec){
                mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::SERVER_GROUP_CANNOT_START_SERVER, {ec.message()});
                return tl::unexpected(err);
            }

            mcsm::ScreenSession session(groupName.value() + "." + modified, exePath.value() + " start -__mcsm__Internal_Group_Start \"" + groupPath + "\"");
            // StartServerCommand will handle the rest
            if(mcsm::isDebug()) mcsm::info("Start command: " + session.getCommand());
            return session.start();
        }
    }else{
        // TODO
    }
    return {};
}

mcsm::VoidResult mcsm::ServerGroupManager::start(const std::string& serverPath){
    auto exePath = mcsm::getExecutablePath(); // to execute mcsm start later
    if(!exePath) return tl::unexpected(exePath.error());

    auto mode = this->group->getMode();
    if(!mode) return tl::unexpected(mode.error());

    if(mode.value() == "screen"){
        auto groupName = this->group->getName();
        if(!groupName) return tl::unexpected(groupName.error());

        std::vector<const mcsm::ServerConfigLoader*> servers = this->group->getServers();

        std::string groupPath = this->group->getHandle()->getPath();

        // iterates through servers and starts the requested server
        for(const mcsm::ServerConfigLoader* server : servers){
            if(server == nullptr){
                auto customTemp = mcsm::errors::INTERNAL_FUNC_EXECUTION_FAILED;
                customTemp.message = "Null server config loader found in ServerGroupLoader's servers.";
                mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, customTemp, {});
                return tl::unexpected(err);
            }

            std::string groupServerPath = server->getHandle()->getPath();
            
            std::string modified = mcsm::safeString(groupServerPath); // session names are groupname.(server path with / replaced by .))

            if(serverPath == groupServerPath){ // true means this is the requested server
                std::error_code ec;
                std::filesystem::current_path(server->getHandle()->getPath(), ec);
                if(mcsm::isDebug()) mcsm::info("[DEBUG] Work path changed to: " + server->getHandle()->getPath());
                if(ec){
                    mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::SERVER_GROUP_CANNOT_START_SERVER, {ec.message()});
                    return tl::unexpected(err);
                }

                mcsm::ScreenSession session(groupName.value() + "." + modified, exePath.value() + " start -__mcsm__Internal_Group_Start \"" + groupPath + "\"");
                mcsm::info("[DEBUG] Checking if session is running for: " + serverPath + " with session name: " + session.getFullSessionName());
                if(session.isRunning()){
                    mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::SERVER_GROUP_CANNOT_START_SERVER, {"Server \"" + serverPath + "\" is already running."});
                    return tl::unexpected(err);
                }
                // StartServerCommand will handle the rest
                if(mcsm::isDebug()) mcsm::info("[DEBUG] Start command: " + session.getCommand());
                return session.start();
            }
        }

        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::SERVER_NOT_LISTED_ON_CURRENT_GROUP, {serverPath});
        return tl::unexpected(err);
    }else{
        // TODO
    }
    return {};
}

mcsm::VoidResult mcsm::ServerGroupManager::stop(){
    auto mode = this->group->getMode();
    if(!mode) return tl::unexpected(mode.error());

    if(mode.value() == "screen"){
        auto groupName = this->group->getName();
        if(!groupName) return tl::unexpected(groupName.error());

        std::vector<const mcsm::ServerConfigLoader*> servers = this->group->getServers();

        for(const mcsm::ServerConfigLoader* server : servers){
            if(server == nullptr){
                auto customTemp = mcsm::errors::INTERNAL_FUNC_EXECUTION_FAILED;
                customTemp.message = "Null server config loader found in ServerGroupLoader's servers.";
                mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, customTemp, {});
                return tl::unexpected(err);
            }

            std::string groupServerPath = server->getHandle()->getPath();
            std::string modified = mcsm::safeString(groupServerPath); // session names are groupname.(server path with / replaced by .))

            mcsm::ScreenSession session(groupName.value() + "." + modified);
            if(!session.isRunning()){ // replace with runningsessionsoption#isrunning
                mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::SERVER_GROUP_CANNOT_STOP_SERVER, {"Cannot stop a session not running. ID: " + session.getFullSessionName()});
                return tl::unexpected(err);
            }
            return session.sendCommand("stop\n");
        }

    }else{
        // TODO
    }

    return {};
}

mcsm::VoidResult mcsm::ServerGroupManager::stop(const std::string& serverPath){
    auto mode = this->group->getMode();
    if(!mode) return tl::unexpected(mode.error());

    if(mode.value() == "screen"){
        auto groupName = this->group->getName();
        if(!groupName) return tl::unexpected(groupName.error());

        std::vector<const mcsm::ServerConfigLoader*> servers = this->group->getServers();

        for(const mcsm::ServerConfigLoader* server : servers){
            if(server == nullptr){
                auto customTemp = mcsm::errors::INTERNAL_FUNC_EXECUTION_FAILED;
                customTemp.message = "Null server config loader found in ServerGroupLoader's servers.";
                mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, customTemp, {});
                return tl::unexpected(err);
            }

            std::string groupServerPath = server->getHandle()->getPath();
            std::string modified = mcsm::safeString(groupServerPath); // session names are groupname.(server path with / replaced by .))

            if(serverPath == groupServerPath){
                mcsm::ScreenSession session(groupName.value() + "." + modified);
                if(!session.isRunning()){ // todo: replace with runningsessionsoption#isrunning
                    mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::SERVER_GROUP_CANNOT_STOP_SERVER, {"Cannot stop a session not running. ID: " + session.getFullSessionName()});
                    return tl::unexpected(err);
                }
                return session.sendCommand("stop\n");
            }
        }
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::SERVER_NOT_LISTED_ON_CURRENT_GROUP, {serverPath});
        return tl::unexpected(err);
    }else{
        // TODO
    }

    return {};
}

mcsm::IntResult mcsm::ServerGroupManager::getRunningSessions() const {
    auto mode = this->group->getMode();
    if(!mode) return tl::unexpected(mode.error());
    int count = 0;

    if(mode.value() == "screen"){
        auto groupName = this->group->getName();
        if(!groupName) return tl::unexpected(groupName.error());

        std::vector<const mcsm::ServerConfigLoader*> servers = this->group->getServers();
        
        // checks running sessions (session names are groupname.(server path with / replaced by .)) and increases count.
        for(const mcsm::ServerConfigLoader* server : servers){
            if(server == nullptr){
                auto customTemp = mcsm::errors::INTERNAL_FUNC_EXECUTION_FAILED;
                customTemp.message = "Null server config loader found in ServerGroupLoader's servers.";
                mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, customTemp, {});
                return tl::unexpected(err);
            }

            auto groupServerPath = server->getServerName();
            if(!groupServerPath) return tl::unexpected(groupServerPath.error());
            
            mcsm::ScreenSession session(groupName.value() + "." + groupServerPath.value());
            if(!session.isRunning()){
                continue;
            }

            count++;
        }

        return count;
    }else{
        // TODO
    }

    return -1;
}

tl::expected<std::vector<const mcsm::ServerConfigLoader*>, mcsm::Error> mcsm::ServerGroupManager::getRunningServers() const {
    auto mode = this->group->getMode();
    if(!mode) return tl::unexpected(mode.error());
    std::vector<const mcsm::ServerConfigLoader*> vec;

    if(mode.value() == "screen"){
        auto groupName = this->group->getName();
        if(!groupName) return tl::unexpected(groupName.error());

        std::vector<const mcsm::ServerConfigLoader*> servers = this->group->getServers();

        // checks running sessions (session names are groupname.(server path with / replaced by .)) and adds them to vec.
        for(const mcsm::ServerConfigLoader* server : servers){
            if(server == nullptr){
                auto customTemp = mcsm::errors::INTERNAL_FUNC_EXECUTION_FAILED;
                customTemp.message = "Null server config loader found in ServerGroupLoader's servers.";
                mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, customTemp, {});
                return tl::unexpected(err);
            }

            std::string groupServerPath = server->getHandle()->getPath();
            
            mcsm::ScreenSession session(groupName.value() + "." + mcsm::safeString(groupServerPath));
            if(!session.isRunning()){
                continue;
            }

            vec.push_back(server);
        }

        return vec;
    }else{
        // TODO
    }

    return {};
}