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

mcsm::Result mcsm::ServerGroupLoader::removeDuplicateServers(mcsm::Option* handle) {
    std::unordered_set<std::string> uniqueServers;
    nlohmann::json uniqueServerList = nlohmann::json::array();

    const nlohmann::json& existingServers = this->handle->getValue("servers");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }
    if(existingServers == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFound("\"servers\"", this->handle->getName())});
        return res;
    }
    if(!existingServers.is_array()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"servers\"", "array of string")});
        return res;
    }
    for(auto& j : existingServers){
        if(!j.is_string()){
            return {mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"servers\"", "array of string")};
        }
    }
    
    for(const auto& server : existingServers){
        std::string serverPath = mcsm::normalizePath(server);
        if(uniqueServers.insert(serverPath).second){
            uniqueServerList.push_back(serverPath);
        }
    }
    
    mcsm::Result setRes = handle->setValue("servers", uniqueServerList);
    if(!setRes.isSuccess()) return setRes;

    return handle->save();
}

mcsm::Result mcsm::ServerGroupLoader::load() {
    this->handle = std::make_unique<mcsm::Option>(this->path, "mcsm_server_group");
    const bool& optExists = this->handle->exists();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    if(!optExists){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverNotConfigured(this->path)});
        return res;
    }

    bool advp = mcsm::GeneralOption::getGeneralOption().advancedParseEnabled();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    mcsm::Result lRes = this->handle->load(advp);
    if(!lRes.isSuccess()) return lRes;

    mcsm::Result rmDupRes = removeDuplicateServers(this->handle.get());
    if(!rmDupRes.isSuccess()) return rmDupRes;

    const nlohmann::json& existingServers = this->handle->getValue("servers");
    // existingServers vector contains an array of server paths
    // no need to check here. removeDuplicateServer does that

    std::vector<std::string> rawServersVec = existingServers;
    for(const std::string& serverStr : rawServersVec){
        std::unique_ptr<mcsm::ServerConfigLoader> loaderPtr = std::make_unique<mcsm::ServerConfigLoader>(serverStr);
        mcsm::Result sprls = loaderPtr->loadConfig();
        if(!sprls.isSuccess()) return sprls;
        this->loaders.push_back(std::move(loaderPtr));
    }

    this->loaded = true;
    return mcsm::Result({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
}

// call this instead of handle->save() when the function modifies and saves this->loaders
mcsm::Result mcsm::ServerGroupLoader::save(){
    std::vector<std::string> strVec;
    for(auto& v : this->loaders){
        if(v == nullptr){
            return {mcsm::ResultType::MCSM_FAIL, {"Null server loader instance detected on server group loader instance."}};
        }
        strVec.push_back(v->getHandle()->getPath());
    }
    
    mcsm::Result setRes = this->handle->setValue("servers", strVec);
    if(!setRes.isSuccess()) return setRes;

    return this->handle->save();
}

const mcsm::Option* mcsm::ServerGroupLoader::getHandle() const {
    return this->handle.get();
}

bool mcsm::ServerGroupLoader::isLoaded() const {
    return this->loaded;
}

std::string mcsm::ServerGroupLoader::getName() const {
    if(!this->loaded){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
            "ServerGroupLoader function called without load.",
            "High chance to be an internal issue. Please open an issue in Github."
        }});
        return "";
    }

    nlohmann::json value = this->handle->getValue("name");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";

    if(value == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFound("\"name\"", this->handle->getName())});
        return "";
    }
    if(!value.is_string()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"name\"", "string")});
        return "";
    }

    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
    if(!mcsm::isSafeString(value)){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::unsafeString(value)});
        return "";
    }
    return value;
}

mcsm::Result mcsm::ServerGroupLoader::setName(const std::string& name){
    if(!this->loaded){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
            "ServerGroupLoader function called without load.",
            "High chance to be an internal issue. Please open an issue in Github."
        }});
        return res;
    }
    if(!mcsm::isSafeString(name)){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::unsafeString(name)});
        return res;
    }
    mcsm::Result setRes = this->handle->setValue("name", name);

    if(!setRes.isSuccess()) return setRes;
    return this->handle->save();
}

std::string mcsm::ServerGroupLoader::getMode() const {
    if(!this->loaded){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
            "ServerGroupLoader function called without load.",
            "High chance to be an internal issue. Please open an issue in Github."
        }});
        return "";
    }

    nlohmann::json value = this->handle->getValue("mode");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";

    if(value == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFound("\"mode\"", this->handle->getName())});
        return "";
    }
    if(!value.is_string()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"mode\"", "string")});
        return "";
    }
    if(value != "screen" && value != "default"){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {"Invalid value of server group mode \"" + std::string(value) + "\"."}});
        return "";
    }

    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
    if(!mcsm::isSafeString(value)){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::unsafeString(value)});
        return "";
    }
    return value;
}

mcsm::Result mcsm::ServerGroupLoader::setMode(const std::string& mode){
    if(!this->loaded){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
            "ServerGroupLoader function called without load.",
            "High chance to be an internal issue. Please open an issue in Github."
        }});
        return res;
    }
    if(!mcsm::isSafeString(mode)){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::unsafeString(mode)});
        return res;
    }
    mcsm::Result setRes = this->handle->setValue("mode", mode);

    if(!setRes.isSuccess()) return setRes;
    return this->handle->save();
}

std::vector<const mcsm::ServerConfigLoader*> mcsm::ServerGroupLoader::getServers() const {
    std::vector<const mcsm::ServerConfigLoader*> servec;
    for(auto& v : this->loaders){
        servec.push_back(v.get());
    }
    return servec;
}

mcsm::Result mcsm::ServerGroupLoader::setServers(const std::vector<mcsm::ServerConfigLoader*>& servers){
    if(!this->loaded){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
            "ServerGroupLoader function called without load.",
            "High chance to be an internal issue. Please open an issue in Github."
        }});
        return res;
    }
    if(!mcsm::isSafeString(mode)){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::unsafeString(mode)});
        return res;
    }
    this->loaders.clear();

    for(const auto* loader : servers){
        if(loader == nullptr){
            return {mcsm::ResultType::MCSM_FAIL, {"Null server config loader encountered in input vector."}};
        }
        this->loaders.push_back(std::make_unique<mcsm::ServerConfigLoader>(*loader));
    }

    return this->save();
}

mcsm::Result mcsm::ServerGroupLoader::addServer(const std::string& path){
    std::string nPath = mcsm::normalizePath(path);
    std::unique_ptr<mcsm::ServerConfigLoader> serv = std::make_unique<mcsm::ServerConfigLoader>(nPath);
    mcsm::Result loadRes = serv->loadConfig();
    if(!loadRes.isSuccess()) return loadRes;

    for(auto& v : this->loaders){
        if(nPath == mcsm::normalizePath(v->getHandle()->getPath())){
            return {mcsm::ResultType::MCSM_FAIL, {"Server " + nPath + " already exists in the configuration."}};
        }
    }
    this->loaders.push_back(std::move(serv));
    return this->save();
}

mcsm::Result mcsm::ServerGroupLoader::addServer(mcsm::ServerConfigLoader* server){
    if(server == nullptr){
        return {mcsm::ResultType::MCSM_FAIL, {"Null serverconfigloader instance detected on servergrouploader. Report this to github."}};
    }
    if(!server->isFullyLoaded()){
        return {mcsm::ResultType::MCSM_FAIL, {"ServerConfigLoader instance passed without being fully loaded on ServerGroupLoader. Report this to github."}};
    }

    std::string nPath = mcsm::normalizePath(server->getHandle()->getPath());
    for(auto& v : this->loaders){
        if(nPath == mcsm::normalizePath(v->getHandle()->getPath())){
            return {mcsm::ResultType::MCSM_FAIL, {"Server " + nPath + " already exists in the configuration."}};
        }
    }
    this->loaders.push_back(std::make_unique<mcsm::ServerConfigLoader>(*server));
    return this->save();
}

mcsm::Result mcsm::ServerGroupLoader::addServer(std::unique_ptr<mcsm::ServerConfigLoader> server){
    if(server == nullptr){
        return {mcsm::ResultType::MCSM_FAIL, {"Null serverconfigloader instance detected on servergrouploader. Report this to github."}};
    }
    if(!server->isFullyLoaded()){
        return {mcsm::ResultType::MCSM_FAIL, {"ServerConfigLoader instance passed without being fully loaded on ServerGroupLoader. Report this to github."}};
    }

    std::string nPath = mcsm::normalizePath(server->getHandle()->getPath());
    for(auto& v : this->loaders){
        if(nPath == mcsm::normalizePath(v->getHandle()->getPath())){
            return {mcsm::ResultType::MCSM_FAIL, {"Server " + nPath + " already exists in the configuration."}};
        }
    }
    this->loaders.push_back(std::move(server));
    return this->save();
}

mcsm::Result mcsm::ServerGroupLoader::addServer(const std::vector<std::unique_ptr<mcsm::ServerConfigLoader>>& servers){
    for(auto& serv : servers){
        if(serv == nullptr){
            return {mcsm::ResultType::MCSM_FAIL, {"Null serverconfigloader instance detected on servergrouploader. Report this to github."}};
        }
        if(!serv->isFullyLoaded()){
            return {mcsm::ResultType::MCSM_FAIL, {"ServerConfigLoader instance passed without being fully loaded on ServerGroupLoader. Report this to github."}};
        }
        std::string nPath = mcsm::normalizePath(serv->getHandle()->getPath());
        for(auto& v : this->loaders){
            if(nPath == mcsm::normalizePath(v->getHandle()->getPath())){
                return {mcsm::ResultType::MCSM_FAIL, {"Server " + nPath + " already exists in the configuration."}};
            }
        }
        this->loaders.push_back(std::make_unique<mcsm::ServerConfigLoader>(*serv));
    }
    return this->save();
}

mcsm::Result mcsm::ServerGroupLoader::removeServer(const std::string& path){
    for(size_t i = 0; i<this->loaders.size(); i++){
        if(this->loaders[i]->getHandle()->getPath() == path){
            this->loaders.erase(this->loaders.begin() + i);
            return save();
        }
    }
    return {mcsm::ResultType::MCSM_FAIL, {
        "Cannot remove an element that doesn't exist.",
        "Please report this to GitHub (https://github.com/dodoman8067/mcsm) if you believe that this is a software issue."
    }};
}

mcsm::Result mcsm::ServerGroupLoader::removeServer(mcsm::ServerConfigLoader* server){
    for(size_t i = 0; i<this->loaders.size(); i++){
        if(this->loaders[i]->getHandle()->getPath() == server->getHandle()->getPath()){
            this->loaders.erase(this->loaders.begin() + i);
            return save();
        }
    }
    return {mcsm::ResultType::MCSM_FAIL, {
        "Cannot remove an element that doesn't exist.",
        "Please report this to GitHub (https://github.com/dodoman8067/mcsm) if you believe that this is a software issue."
    }};
}

mcsm::Result mcsm::ServerGroupLoader::removeServer(const std::vector<mcsm::ServerConfigLoader*>& servers){
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

    return {mcsm::ResultType::MCSM_FAIL, {
        "None of the specified servers could be removed.",
        "Please report this to GitHub (https://github.com/dodoman8067/mcsm) if you believe that this is a software issue."
    }};
}