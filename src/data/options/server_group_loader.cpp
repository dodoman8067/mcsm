#include "mcsm/data/toml_option.h"
#include <mcsm/data/options/server_group_loader.h>
#include <mcsm/data/options/general_option.h>
#include <toml++/impl/array.hpp>
#include <unordered_set>

mcsm::ServerGroupLoader::ServerGroupLoader(const std::string& path){
    this->path = path;
    this->handle = nullptr;
    this->loaded = false;
}

mcsm::ServerGroupLoader::~ServerGroupLoader(){
    this->loaded = false;
    this->loaders.clear();
}

mcsm::VoidResult mcsm::ServerGroupLoader::removeDuplicateServers(mcsm::TomlOption* handle, toml::array& servers) {
    std::unordered_set<std::string> uniqueServers;
    toml::array uniqueServerList = toml::array();

    for(auto& j : servers){
        if(!j.is_string()){
            mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::TOML_WRONG_TYPE, {"[group.servers]", "array of string"});
            return tl::unexpected(err);
        }
    }
    
    for(auto& server : servers){
        std::string serverPath = mcsm::normalizePath(server.as_string()->get());
        if(uniqueServers.insert(serverPath).second){
            uniqueServerList.push_back(serverPath);
        }
    }
    
    toml::table servsTable;
    servsTable.insert_or_assign("servers", uniqueServerList);

    toml::table groupTable = this->configRoot;
    groupTable.insert_or_assign("server", servsTable);
    groupTable.insert_or_assign("meta", this->rootMeta);

    mcsm::VoidResult setRes = handle->setValue("group", groupTable);
    if(!setRes) return setRes;

    return handle->save();
}

mcsm::VoidResult mcsm::ServerGroupLoader::load() {
    this->handle = std::make_unique<mcsm::TomlOption>(this->path, "mcsm_server_group");

    auto optExists = this->handle->exists();
    if(!optExists) return tl::unexpected(optExists.error());

    if(!optExists.value()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::SERVER_NOT_CONFIGURED, {this->path});
        return tl::unexpected(err);
    }

    bool advp = mcsm::GeneralOption::getGeneralOption().advancedParseEnabled();

    mcsm::VoidResult lRes = this->handle->load(advp);
    if(!lRes) return lRes;

    auto headerLoadRes = this->handle->getValue("header");
    if(!headerLoadRes){
        return tl::unexpected(headerLoadRes.error());
    }
    if(headerLoadRes.value() == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::SERVER_INVALID_CONFIG_HEADER, {mcsm::joinPath(this->path, "server")});
        return tl::unexpected(err);
    }
    if(!headerLoadRes.value()->is_table()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::TOML_WRONG_TYPE, {"\"[header]\"", "table"});
        return tl::unexpected(err);
    }
    toml::table headerTable = *headerLoadRes.value()->as_table();

    if(!headerTable.contains("config_version") || !headerTable["config_version"].is_integer()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::SERVER_INVALID_CONFIG_VERSION, {"file " + mcsm::joinPath(this->path, "server"), std::to_string(mcsm::MIN_GROUP_CONFIG_VERSION)});
        return tl::unexpected(err);
    }
    if(*headerTable.get_as<int64_t>("config_version") > (int64_t) mcsm::GROUP_CONFIG_VERSION || *headerTable.get_as<int64_t>("config_version") < (int64_t) mcsm::MIN_GROUP_CONFIG_VERSION){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::SERVER_INVALID_CONFIG_VERSION, {"file " + mcsm::joinPath(this->path, "server"), std::to_string(mcsm::MIN_GROUP_CONFIG_VERSION)});
        return tl::unexpected(err);
    }

    auto rootLoadRes = this->handle->getValue("group");
    if(!rootLoadRes){
        return tl::unexpected(rootLoadRes.error());
    }
    if(rootLoadRes == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::TOML_NOT_FOUND, {"\"[group]\"", mcsm::joinPath(this->path, "server")});
        return tl::unexpected(err);
    }
    if(!rootLoadRes.value()->is_table()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::TOML_WRONG_TYPE, {"\"[group]\"", "table"});
        return tl::unexpected(err);
    }

    toml::table groupTable = *rootLoadRes.value()->as_table();

    this->configRoot = groupTable;
    this->configHeader = headerTable;

    if(!groupTable.contains("meta")){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::TOML_NOT_FOUND, {"\"[group.meta]\"", mcsm::joinPath(this->path, "group")});
        return tl::unexpected(err);
    }
    if(!groupTable.contains("server")){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::TOML_NOT_FOUND, {"\"[group.server]\"", mcsm::joinPath(this->path, "group")});
        return tl::unexpected(err);
    }

    if(!groupTable["meta"].is_table()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::TOML_WRONG_TYPE, {"\"[group.meta]\"", "table"});
        return tl::unexpected(err);
    }
    if(!groupTable["server"].is_table()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::TOML_WRONG_TYPE, {"\"[group.server]\"", "table"});
        return tl::unexpected(err);
    }

    this->rootMeta = *groupTable["meta"].as_table();
    this->rootServers = *groupTable["server"].as_table();

    auto listRes = this->rootServers.get("servers");
    if(!this->rootServers.contains("servers")){

    }
    if(!listRes->is_array()){

    }

    toml::array serversCopy = *listRes->as_array();
    mcsm::VoidResult rmDupRes = removeDuplicateServers(this->handle.get(), serversCopy);
    if(!rmDupRes) return rmDupRes;

    listRes = &serversCopy;

    this->serversList = *listRes->as_array();

    for(auto& serverStr : serversCopy){
        std::unique_ptr<mcsm::ServerConfigLoader> loaderPtr = std::make_unique<mcsm::ServerConfigLoader>(serverStr.as_string()->get());
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

    toml::table servsTable;
    servsTable.insert_or_assign("servers", vectoarr(strVec));

    toml::table groupTable = this->configRoot;
    groupTable.insert_or_assign("server", servsTable);
    groupTable.insert_or_assign("meta", this->rootMeta);

    mcsm::VoidResult setRes = handle->setValue("group", groupTable);
    if(!setRes) return setRes;

    return this->handle->save();
}

const mcsm::TomlOption* mcsm::ServerGroupLoader::getHandle() const {
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

    auto value = this->rootMeta.get("name");

    if(value == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::TOML_NOT_FOUND, {"[group.meta]->name", this->handle->getName()});
        return tl::unexpected(err);
    }
    if(!value->is_string()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::TOML_WRONG_TYPE, {"[group.meta]->name", "string"});
        return tl::unexpected(err);
    }

    if(!mcsm::isSafeString(gstr(value))){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::UNSAFE_STRING, {gstr(value)});
        return tl::unexpected(err);
    }
    return gstr(value);
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
    toml::table metaTable = this->rootMeta;
    metaTable.insert_or_assign("name", name);

    toml::table groupTable = this->configRoot;
    groupTable.insert_or_assign("meta", metaTable);
    mcsm::VoidResult setRes = handle->setValue("group", groupTable);
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

    auto value = this->rootMeta.get("mode");

    if(value == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::TOML_NOT_FOUND, {"[group.meta]->mode", this->handle->getName()});
        return tl::unexpected(err);
    }
    if(!value->is_string()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::TOML_WRONG_TYPE, {"[group.meta]->mode", "string"});
        return tl::unexpected(err);
    }
    if(gstr(value) != "screen" && gstr(value) != "default"){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::SERVER_GROUP_MODE_INVALID, {gstr(value)});
        return tl::unexpected(err);
    }

    if(!mcsm::isSafeString(gstr(value))){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::UNSAFE_STRING, {gstr(value)});
        return tl::unexpected(err);
    }
    return gstr(value);
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
    toml::table metaTable = this->rootMeta;
    metaTable.insert_or_assign("mode", mode);

    toml::table groupTable = this->configRoot;
    groupTable.insert_or_assign("meta", metaTable);
    mcsm::VoidResult setRes = handle->setValue("group", groupTable);
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
        const std::string p = loader->getHandle()->getPath();
        // serverconfigloader is not copiable
        auto newLoader = std::make_unique<mcsm::ServerConfigLoader>(p);
        auto loadRes = newLoader->loadConfig();
        if (!loadRes) return loadRes;

        this->loaders.push_back(std::move(newLoader));
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
    // serverconfigloader is not copiable
    auto newLoader = std::make_unique<mcsm::ServerConfigLoader>(nPath);
    auto loadRes = newLoader->loadConfig();
    if (!loadRes) return loadRes;

    this->loaders.push_back(std::move(newLoader));
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
        auto newLoader = std::make_unique<mcsm::ServerConfigLoader>(serv->getHandle()->getPath());
        auto loadRes = newLoader->loadConfig();
        if(!loadRes) return loadRes;
        this->loaders.push_back(std::move(newLoader));
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