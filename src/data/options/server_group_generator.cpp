#include <mcsm/data/options/server_group_generator.h>
#include <mcsm/data/options/general_option.h>

mcsm::Result mcsm::ServerGroupGenerator::generate(const std::string& mode){
    return generate(mode, {});
}

mcsm::Result mcsm::ServerGroupGenerator::generate(const std::string& mode, const std::vector<const mcsm::ServerConfigLoader*> servers){
    if(mode != "screen" && mode != "default"){
        return {mcsm::ResultType::MCSM_FAIL, {"Invalid value of server group mode \"" + mode + "\"."}};
    }

    this->handle = std::make_unique<mcsm::Option>(this->path, "mcsm_server_group");

    bool exists = this->handle->exists();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    if(exists){
        return {mcsm::ResultType::MCSM_FAIL, {"Server group file already exists at: " + this->handle->getPath()}};
    }

    bool singleConfigExists = mcsm::fileExists(this->path + "/server.json");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    if(singleConfigExists){
        return {mcsm::ResultType::MCSM_FAIL, {"Cannot create server group config file where a single server is already configured: " + this->handle->getPath()}};
    }

    mcsm::Result cRes = this->handle->create();
    if(!cRes.isSuccess()) return cRes;

    const bool advp = mcsm::GeneralOption::getGeneralOption().advancedParseEnabled();
    mcsm::Result lRes = this->handle->load(advp);
    if(!lRes.isSuccess()) return lRes;

    mcsm::Result nameSetRes = this->handle->setValue("name", this->name);
    if(!nameSetRes.isSuccess()) return nameSetRes;

    mcsm::Result modeSetRes = this->handle->setValue("mode", mode);
    if(!modeSetRes.isSuccess()) return modeSetRes;

    std::vector<std::string> serversStrVec;
    if(!servers.empty()){
        serversStrVec.reserve(servers.size());

        for(auto& server : servers){
            if(server == nullptr){
                if(mcsm::isDebug()) mcsm::warning("mcsm::ServerGroupGenerator::generate's servers vector contains null pointers.");
                continue;
            }
            if(!server->isFullyLoaded()){
                if(mcsm::isDebug()) mcsm::warning("mcsm::ServerGroupGenerator::generate's servers vector contains unloaded servers.");
                continue;
            }
            std::string sPath = server->getHandle()->getPath();
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
                std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
                mcsm::Result res(resp.first, resp.second);
                return res;
            }
    
            serversStrVec.push_back(sPath);
        }
    }

    mcsm::Result serversSetRes = this->handle->setValue("servers", serversStrVec);
    if(!serversSetRes.isSuccess()) return serversSetRes;

    return this->handle->save();
}

const mcsm::Option* mcsm::ServerGroupGenerator::getHandle() const {
    if(this->handle) return this->handle.get();
    return nullptr;
}