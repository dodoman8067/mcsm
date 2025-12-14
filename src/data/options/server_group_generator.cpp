#include <mcsm/data/options/server_group_generator.h>
#include <mcsm/data/options/general_option.h>

mcsm::VoidResult mcsm::ServerGroupGenerator::generate(const std::string& mode){
    return generate(mode, {});
}

mcsm::VoidResult mcsm::ServerGroupGenerator::generate(const std::string& mode, const std::vector<const mcsm::ServerConfigLoader*> servers){
    if(mode != "screen" && mode != "default"){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::SERVER_GROUP_MODE_INVALID, {mode});
        return tl::unexpected(err);
    }

    this->handle = std::make_unique<mcsm::Option>(this->path, "mcsm_server_group");

    auto exists = this->handle->exists();
    if(!exists) return tl::unexpected(exists.error());
    if(exists.value()){
        auto customTemp = mcsm::errors::SERVER_ALREADY_CONFIGURED;
        customTemp.message = "Server group file already exists at: " + this->handle->getPath();
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, customTemp, {mode});
        return tl::unexpected(err);
    }

    auto singleConfigExists = mcsm::fileExists(this->path + "/server.json");
    if(!singleConfigExists) return tl::unexpected(singleConfigExists.error());

    if(singleConfigExists.value()){
        auto customTemp = mcsm::errors::SERVER_ALREADY_CONFIGURED;
        customTemp.message = "Cannot create server group config file where a single server is already configured: " + this->handle->getPath();
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, customTemp, {mode});
        return tl::unexpected(err);
    }

    auto cRes = this->handle->create();
    if(!cRes) return cRes;

    const bool advp = mcsm::GeneralOption::getGeneralOption().advancedParseEnabled();
    auto lRes = this->handle->load(advp);
    if(!lRes) return lRes;

    auto nameSetRes = this->handle->setValue("name", this->name);
    if(!nameSetRes) return nameSetRes;

    auto modeSetRes = this->handle->setValue("mode", mode);
    if(!modeSetRes) return modeSetRes;

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
            serversStrVec.push_back(sPath);
        }
    }

    auto serversSetRes = this->handle->setValue("servers", serversStrVec);
    if(!serversSetRes) return serversSetRes;

    return this->handle->save();
}

const mcsm::Option* mcsm::ServerGroupGenerator::getHandle() const {
    if(this->handle) return this->handle.get();
    return nullptr;
}