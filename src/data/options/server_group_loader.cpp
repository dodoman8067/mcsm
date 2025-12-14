#include <mcsm/data/options/server_group_loader.h>
#include <mcsm/data/options/general_option.h>
#include <unordered_set>
#include <regex>

mcsm::ServerGroupLoader::ServerGroupLoader(const std::string& path){
    this->path = path;
    this->handle = nullptr;
    this->loaded = false;
}

mcsm::ServerGroupLoader::~ServerGroupLoader(){
    this->loaded = false;
    this->loaders.clear();
}

mcsm::VoidResult mcsm::ServerGroupLoader::removeDuplicateServers(mcsm::Option* handle) {
    std::unordered_set<std::string> uniqueServers;
    nlohmann::json uniqueServerList = nlohmann::json::array();

    auto existSRes = this->handle->getValue("servers");
    if(!existSRes) return tl::unexpected(existSRes.error());
    const nlohmann::json& existingServers = existSRes.value();

    if(existingServers == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_NOT_FOUND, {"\"servers\"", this->handle->getName()});
        return tl::unexpected(err);
    }
    if(!existingServers.is_array()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE, {"\"servers\"", "array of string"});
        return tl::unexpected(err);
    }
    for(auto& j : existingServers){
        if(!j.is_string()){
            mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE, {"\"servers\"", "array of string"});
            return tl::unexpected(err);
        }
    }
    
    for(const auto& server : existingServers){
        std::string serverPath = mcsm::normalizePath(server);
        if(uniqueServers.insert(serverPath).second){
            uniqueServerList.push_back(serverPath);
        }
    }
    
    mcsm::VoidResult setRes = handle->setValue("servers", uniqueServerList);
    if(!setRes) return setRes;

    return handle->save();
}

mcsm::VoidResult mcsm::ServerGroupLoader::load() {
    this->handle = std::make_unique<mcsm::Option>(this->path, "mcsm_server_group");

    auto optExists = this->handle->exists();
    if(!optExists) return tl::unexpected(optExists.error());

    if(!optExists.value()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::SERVER_NOT_CONFIGURED, {this->path});
        return tl::unexpected(err);
    }

    bool advp = mcsm::GeneralOption::getGeneralOption().advancedParseEnabled();

    mcsm::VoidResult lRes = this->handle->load(advp);
    if(!lRes) return lRes;

    mcsm::VoidResult rmDupRes = removeDuplicateServers(this->handle.get());
    if(!rmDupRes) return rmDupRes;

    auto eSRes = this->handle->getValue("servers");
    if(!eSRes) return tl::unexpected(eSRes.error());

    const nlohmann::json& existingServers = eSRes.value();
    // existingServers vector contains an array of server paths
    // no need to check here. removeDuplicateServer does that

    std::vector<std::string> rawServersVec = existingServers;
    for(const std::string& serverStr : rawServersVec){
        std::unique_ptr<mcsm::ServerConfigLoader> loaderPtr = std::make_unique<mcsm::ServerConfigLoader>(serverStr);
        mcsm::VoidResult sprls = loaderPtr->loadConfig();
        if(!sprls) return sprls;
        this->loaders.push_back(std::move(loaderPtr));
    }

    this->loaded = true;
    return {};
}

// call this instead of handle->save() when the function modifies and saves this->loaders
mcsm::VoidResult mcsm::ServerGroupLoader::save(){
    std::vector<std::string> strVec;
    for(auto& v : this->loaders){
        if(v == nullptr){
            auto customTemp = mcsm::errors::INTERNAL_FUNC_EXECUTION_FAILED;
            customTemp.message = "Null server loader instance detected on server group loader instance.";
            mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, customTemp, {});
            return tl::unexpected(err);
        }
        strVec.push_back(v->getHandle()->getPath());
    }
    
    mcsm::VoidResult setRes = this->handle->setValue("servers", strVec);
    if(!setRes) return setRes;

    return this->handle->save();
}

const mcsm::Option* mcsm::ServerGroupLoader::getHandle() const {
    return this->handle.get();
}

bool mcsm::ServerGroupLoader::isLoaded() const {
    return this->loaded;
}

mcsm::StringResult mcsm::ServerGroupLoader::getName() const {
    if(!this->loaded){
        auto customTemp = mcsm::errors::INTERNAL_FUNC_EXECUTION_FAILED;
        customTemp.message = "ServerGroupLoader function called without load.";
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, customTemp, {});
        return tl::unexpected(err);
    }

    auto valueRes = this->handle->getValue("name");
    if(!valueRes) return tl::unexpected(valueRes.error());

    nlohmann::json value = valueRes.value();
    if(value == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_NOT_FOUND, {"\"name\"", this->handle->getName()});
        return tl::unexpected(err);
    }
    if(!value.is_string()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE, {"\"name\"", "string"});
        return tl::unexpected(err);
    }

    if(!mcsm::isSafeString(value)){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::UNSAFE_STRING, {value});
        return tl::unexpected(err);
    }
    return value.get<std::string>();
}

mcsm::VoidResult mcsm::ServerGroupLoader::setName(const std::string& name){
    if(!this->loaded){
        auto customTemp = mcsm::errors::INTERNAL_FUNC_EXECUTION_FAILED;
        customTemp.message = "ServerGroupLoader function called without load.";
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, customTemp, {});
        return tl::unexpected(err);
    }
    if(!mcsm::isSafeString(name)){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::UNSAFE_STRING, {name});
        return tl::unexpected(err);
    }
    mcsm::VoidResult setRes = this->handle->setValue("name", name);

    if(!setRes) return setRes;
    return this->handle->save();
}

mcsm::StringResult mcsm::ServerGroupLoader::getMode() const {
    if(!this->loaded){
        auto customTemp = mcsm::errors::INTERNAL_FUNC_EXECUTION_FAILED;
        customTemp.message = "ServerGroupLoader function called without load.";
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, customTemp, {});
        return tl::unexpected(err);
    }

    auto valueRes = this->handle->getValue("mode");
    if(!valueRes) return tl::unexpected(valueRes.error());

    nlohmann::json value = valueRes.value();

    if(value == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_NOT_FOUND, {"\"mode\"", this->handle->getName()});
        return tl::unexpected(err);
    }
    if(!value.is_string()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE, {"\"mode\"", "string"});
        return tl::unexpected(err);
    }
    if(value != "screen" && value != "default"){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::SERVER_GROUP_MODE_INVALID, {std::string(value)});
        return tl::unexpected(err);
    }

    if(!mcsm::isSafeString(value)){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::UNSAFE_STRING, {value});
        return tl::unexpected(err);
    }
    return value;
}

mcsm::VoidResult mcsm::ServerGroupLoader::setMode(const std::string& mode){
    if(!this->loaded){
        auto customTemp = mcsm::errors::INTERNAL_FUNC_EXECUTION_FAILED;
        customTemp.message = "ServerGroupLoader function called without load.";
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, customTemp, {});
        return tl::unexpected(err);
    }
    if(!mcsm::isSafeString(mode)){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::UNSAFE_STRING, {mode});
        return tl::unexpected(err);
    }
    mcsm::VoidResult setRes = this->handle->setValue("mode", mode);

    if(!setRes) return setRes;
    return this->handle->save();
}

std::vector<const mcsm::ServerConfigLoader*> mcsm::ServerGroupLoader::getServers() const {
    std::vector<const mcsm::ServerConfigLoader*> servec;
    for(auto& v : this->loaders){
        servec.push_back(v.get());
    }
    return servec;
}

mcsm::VoidResult mcsm::ServerGroupLoader::setServers(const std::vector<mcsm::ServerConfigLoader*>& servers){
    if(!this->loaded){
        auto customTemp = mcsm::errors::INTERNAL_FUNC_EXECUTION_FAILED;
        customTemp.message = "ServerGroupLoader function called without load.";
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, customTemp, {});
        return tl::unexpected(err);
    }
    if(!mcsm::isSafeString(mode)){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::UNSAFE_STRING, {mode});
        return tl::unexpected(err);
    }
    this->loaders.clear();

    for(const auto* loader : servers){
        if(loader == nullptr){
            auto customTemp = mcsm::errors::INTERNAL_FUNC_EXECUTION_FAILED;
            customTemp.message = "ServerGroupLoader#setServers failed: Null server config loader instance in input vector.";
            mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, customTemp, {});
            return tl::unexpected(err);
        }
        this->loaders.push_back(std::make_unique<mcsm::ServerConfigLoader>(*loader));
    }

    return this->save();
}

mcsm::VoidResult mcsm::ServerGroupLoader::addServer(const std::string& path){
    std::string nPath = mcsm::normalizePath(path);
    std::unique_ptr<mcsm::ServerConfigLoader> serv = std::make_unique<mcsm::ServerConfigLoader>(nPath);
    mcsm::VoidResult loadRes = serv->loadConfig();
    if(!loadRes) return loadRes;

    for(auto& v : this->loaders){
        if(nPath == mcsm::normalizePath(v->getHandle()->getPath())){
            auto customTemp = mcsm::errors::SERVER_ALREADY_CONFIGURED;
            customTemp.message = "Server " + nPath + " already exists in the configuration.";
            customTemp.solution = "";
            mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, customTemp, {});
            return tl::unexpected(err);
        }
    }
    this->loaders.push_back(std::move(serv));
    return this->save();
}

mcsm::VoidResult mcsm::ServerGroupLoader::addServer(mcsm::ServerConfigLoader* server){
    if(server == nullptr){
        auto customTemp = mcsm::errors::ILLEGAL_PARAMETER;
        customTemp.message = "Null serverconfigloader instance detected on servergrouploader.";
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, customTemp, {});
        return tl::unexpected(err);
    }
    if(!server->isFullyLoaded()){
        auto customTemp = mcsm::errors::ILLEGAL_PARAMETER;
        customTemp.message = "ServerConfigLoader instance passed without being fully loaded on ServerGroupLoader.";
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, customTemp, {});
        return tl::unexpected(err);
    }

    std::string nPath = mcsm::normalizePath(server->getHandle()->getPath());
    for(auto& v : this->loaders){
        if(nPath == mcsm::normalizePath(v->getHandle()->getPath())){
            auto customTemp = mcsm::errors::SERVER_ALREADY_CONFIGURED;
            customTemp.message = "Server " + nPath + " already exists in the configuration.";
            customTemp.solution = "";
            mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, customTemp, {});
            return tl::unexpected(err);
        }
    }
    this->loaders.push_back(std::make_unique<mcsm::ServerConfigLoader>(*server));
    return this->save();
}

mcsm::VoidResult mcsm::ServerGroupLoader::addServer(std::unique_ptr<mcsm::ServerConfigLoader> server){
    if(server == nullptr){
        auto customTemp = mcsm::errors::ILLEGAL_PARAMETER;
        customTemp.message = "Null serverconfigloader instance detected on servergrouploader.";
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, customTemp, {});
        return tl::unexpected(err);
    }
    if(!server->isFullyLoaded()){
        auto customTemp = mcsm::errors::ILLEGAL_PARAMETER;
        customTemp.message = "ServerConfigLoader instance passed without being fully loaded on ServerGroupLoader.";
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, customTemp, {});
        return tl::unexpected(err);
    }

    std::string nPath = mcsm::normalizePath(server->getHandle()->getPath());
    for(auto& v : this->loaders){
        if(nPath == mcsm::normalizePath(v->getHandle()->getPath())){
            auto customTemp = mcsm::errors::SERVER_ALREADY_CONFIGURED;
            customTemp.message = "Server " + nPath + " already exists in the configuration.";
            customTemp.solution = "";
            mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, customTemp, {});
            return tl::unexpected(err);
        }
    }
    this->loaders.push_back(std::move(server));
    return this->save();
}

mcsm::VoidResult mcsm::ServerGroupLoader::addServer(const std::vector<std::unique_ptr<mcsm::ServerConfigLoader>>& servers){
    for(auto& serv : servers){
        if(serv == nullptr){
            auto customTemp = mcsm::errors::ILLEGAL_PARAMETER;
            customTemp.message = "Null serverconfigloader instance detected on servergrouploader.";
            mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, customTemp, {});
            return tl::unexpected(err);
        }
        if(!serv->isFullyLoaded()){
            auto customTemp = mcsm::errors::ILLEGAL_PARAMETER;
            customTemp.message = "ServerConfigLoader instance passed without being fully loaded on ServerGroupLoader.";
            mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, customTemp, {});
            return tl::unexpected(err);
        }
        std::string nPath = mcsm::normalizePath(serv->getHandle()->getPath());
        for(auto& v : this->loaders){
            if(nPath == mcsm::normalizePath(v->getHandle()->getPath())){
                auto customTemp = mcsm::errors::SERVER_ALREADY_CONFIGURED;
                customTemp.message = "Server " + nPath + " already exists in the configuration.";
                customTemp.solution = "";
                mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, customTemp, {});
                return tl::unexpected(err);
            }
        }
        this->loaders.push_back(std::make_unique<mcsm::ServerConfigLoader>(*serv));
    }
    return this->save();
}

mcsm::VoidResult mcsm::ServerGroupLoader::removeServer(const std::string& path){
    for(size_t i = 0; i<this->loaders.size(); i++){
        if(this->loaders[i]->getHandle()->getPath() == path){
            this->loaders.erase(this->loaders.begin() + i);
            return save();
        }
    }

    auto customTemp = mcsm::errors::ILLEGAL_PARAMETER;
    customTemp.message = "Cannot remove an element that doesn't exist.";
    mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, customTemp, {});
    return tl::unexpected(err);
}

mcsm::VoidResult mcsm::ServerGroupLoader::removeServer(mcsm::ServerConfigLoader* server){
    for(size_t i = 0; i<this->loaders.size(); i++){
        if(this->loaders[i]->getHandle()->getPath() == server->getHandle()->getPath()){
            this->loaders.erase(this->loaders.begin() + i);
            return save();
        }
    }
    auto customTemp = mcsm::errors::ILLEGAL_PARAMETER;
    customTemp.message = "Cannot remove an element that doesn't exist.";
    mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, customTemp, {});
    return tl::unexpected(err);
}

mcsm::VoidResult mcsm::ServerGroupLoader::removeServer(const std::vector<mcsm::ServerConfigLoader*>& servers){
    bool removedAny = false;

    for(const auto& server : servers){
        auto it = std::find_if(this->loaders.begin(), this->loaders.end(),
            [&](const std::unique_ptr<mcsm::ServerConfigLoader>& loader){
                return loader->getHandle()->getPath() == server->getHandle()->getPath();
            });

        if(it != this->loaders.end()){
            this->loaders.erase(it);
            removedAny = true;
        }
    }

    if(removedAny){
        return save();
    }

    auto customTemp = mcsm::errors::INTERNAL_FUNC_EXECUTION_FAILED;
    customTemp.message = "None of the specified servers could be removed.";
    mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, customTemp, {});
    return tl::unexpected(err);
}