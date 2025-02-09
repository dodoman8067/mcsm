#include <mcsm/data/options/server_group_generator.h>
#include <mcsm/data/options/general_option.h>

mcsm::Result mcsm::ServerGroupGenerator::generate(const std::string& mode){
    return generate(mode, {});
}

mcsm::Result mcsm::ServerGroupGenerator::generate(const std::string& mode, const std::vector<std::unique_ptr<mcsm::ServerConfigLoader>>& servers){
    if(mode != "screen" && mode != "default"){
        return {mcsm::ResultType::MCSM_FAIL, {"Invalid value of server group mode \"" + mode + "\"."}};
    }

    this->handle = std::make_unique<mcsm::Option>(path, "mcsm_server_group");

    bool exists = this->handle->exists();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    if(exists){
        return {mcsm::ResultType::MCSM_FAIL, {"Server group file already exists at: " + this->handle->getPath()}};
    }

    mcsm::Result cRes = this->handle->create();
    if(!cRes.isSuccess()) return cRes;

    const bool advp = mcsm::GeneralOption::getGeneralOption().advancedParseEnabled();
    mcsm::Result lRes = this->handle->load(advp);
    if(!lRes.isSuccess()) return lRes;

    mcsm::Result nameSetRes = this->handle->setValue("name", this->name);
    if(!nameSetRes.isSuccess()) return nameSetRes;

    std::vector<std::string> serversStrVec;
    if(!servers.empty()){
        serversStrVec.reserve(servers.size());

        for(auto& server : servers){
            if(server == nullptr){
                if(mcsm::isDebug()) mcsm::warning("mcsm::ServerGroupGenerator::generate's servers vector contains null pointers.");
                continue;
            }
            std::string sName = server->getServerName();
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
                std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
                mcsm::Result res(resp.first, resp.second);
                return res;
            }
    
            serversStrVec.push_back(sName);
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