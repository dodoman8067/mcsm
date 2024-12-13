#include <mcsm/data/options/server_group_loader.h>
#include <mcsm/data/options/general_option.h>

mcsm::ServerGroupLoader::ServerGroupLoader(const std::string& path){
    this->path = path;
    this->handle = nullptr;
    this->loaded = false;
}

mcsm::ServerGroupLoader::~ServerGroupLoader(){
    this->loaded = false;
    this->loaders.clear();
}

mcsm::Result mcsm::ServerGroupLoader::load() {
    this->handle = std::make_unique<mcsm::Option>(this->path, "server_group");
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
    if(!existingServers.is_string()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"servers\"", "array of string")});
        return res;
    }

    // existingServers vector contains an array of server paths

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

const mcsm::Option* mcsm::ServerGroupLoader::getHandle() const {
    return this->handle.get();
}

const bool mcsm::ServerGroupLoader::isLoaded() const {
    return this->loaded;
}

std::string mcsm::ServerGroupLoader::getName() const {

}

mcsm::Result mcsm::ServerGroupLoader::setName(const std::string& name){

}

std::string mcsm::ServerGroupLoader::getMode() const {

}
mcsm::Result mcsm::ServerGroupLoader::setMode(const std::string& mode){

}

std::vector<const mcsm::ServerConfigLoader*> mcsm::ServerGroupLoader::getServers() const {
    // option value is "servers"
}

mcsm::Result mcsm::ServerGroupLoader::setServers(const std::vector<mcsm::ServerConfigLoader*>& servers){

}

mcsm::Result mcsm::ServerGroupLoader::addServer(const std::string& path){

}

mcsm::Result mcsm::ServerGroupLoader::addServer(mcsm::ServerConfigLoader* server){

}

mcsm::Result mcsm::ServerGroupLoader::addServer(std::unique_ptr<mcsm::ServerConfigLoader> server){

}

mcsm::Result mcsm::ServerGroupLoader::addServer(const std::vector<std::unique_ptr<mcsm::ServerConfigLoader>>& servers){

}

mcsm::Result mcsm::ServerGroupLoader::removeServer(const std::string& path){

}

mcsm::Result mcsm::ServerGroupLoader::removeServer(mcsm::ServerConfigLoader* server){

}

mcsm::Result mcsm::ServerGroupLoader::removeServer(const std::vector<mcsm::ServerConfigLoader*>& servers){

}