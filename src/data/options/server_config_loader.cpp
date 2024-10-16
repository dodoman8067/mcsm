#include <mcsm/data/options/server_config_loader.h>
#include <mcsm/data/options/general_option.h>
#include <mcsm/server/server_registry.h>

mcsm::ServerConfigLoader::ServerConfigLoader(const std::string& path){
    this->configPath = path;
    this->optionHandle = nullptr;
    this->isLoaded = false;
}

mcsm::ServerConfigLoader::~ServerConfigLoader(){
    this->isLoaded = false;
}

mcsm::Result mcsm::ServerConfigLoader::loadConfig(){
    this->optionHandle = std::make_unique<mcsm::Option>(this->configPath, "server");
    const bool& optExists = this->optionHandle->exists();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    if(!optExists){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverNotConfigured(this->configPath)});
        return res;
    }

    bool advp = mcsm::GeneralOption::getGeneralOption().advancedParseEnabled();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    mcsm::Result lRes = this->optionHandle->load(advp);
    if(!lRes.isSuccess()) return lRes;

    this->isLoaded = true;
    return mcsm::Result({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
}

template <>
nlohmann::json::value_t mcsm::ServerConfigLoader::getJsonType<int>(){
    return nlohmann::json::value_t::number_integer;
}   

template <>
nlohmann::json::value_t mcsm::ServerConfigLoader::getJsonType<unsigned int>(){
    return nlohmann::json::value_t::number_unsigned;
}   

template <>
nlohmann::json::value_t mcsm::ServerConfigLoader::getJsonType<float>(){
    return nlohmann::json::value_t::number_float;
}   

template <>
nlohmann::json::value_t mcsm::ServerConfigLoader::getJsonType<double>(){
    return nlohmann::json::value_t::number_float;
}

template <>
nlohmann::json::value_t mcsm::ServerConfigLoader::getJsonType<bool>(){
    return nlohmann::json::value_t::boolean;
}

template <>
nlohmann::json::value_t mcsm::ServerConfigLoader::getJsonType<std::string>(){
    return nlohmann::json::value_t::string;
}

// use json.is_array to get arrays

template <>
nlohmann::json::value_t mcsm::ServerConfigLoader::getJsonType<nlohmann::json>(){
    return nlohmann::json::value_t::object;
}

template <>
nlohmann::json::value_t mcsm::ServerConfigLoader::getJsonType<std::vector<nlohmann::json>>(){
    return nlohmann::json::value_t::object;
}

// isLoaded won't be "true" if the option does not exist which is why I don't check the existence of the option file.

std::string mcsm::ServerConfigLoader::getServerName() const {
    if(!this->isLoaded){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
            "ServerConfigLoader function called without loadConfig.",
            "High chance to be an internal issue. Please open an issue in Github."
        }});
        return "";
    }

    nlohmann::json value = this->optionHandle->getValue("name");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";

    if(value == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFound("\"name\"", this->optionHandle->getName())});
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

mcsm::Result mcsm::ServerConfigLoader::setServerName(const std::string& name){
    if(!this->isLoaded){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
            "ServerConfigLoader function called without loadConfig.",
            "High chance to be an internal issue. Please open an issue in Github."
        }});
        return res;
    }
    if(!mcsm::isSafeString(name)){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::unsafeString(name)});
        return res;
    }
    mcsm::Result setRes = this->optionHandle->setValue("name", name);

    if(!setRes.isSuccess()) return setRes;
    return this->optionHandle->save();
}

std::string mcsm::ServerConfigLoader::getServerVersion() const {
    if(!this->isLoaded){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
            "ServerConfigLoader function called without loadConfig.",
            "High chance to be an internal issue. Please open an issue in Github."
        }});
        return "";
    }

    const nlohmann::json& value = this->optionHandle->getValue("version");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";

    if(value == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFound("\"version\"", this->optionHandle->getName())});
        return "";
    }
    if(!value.is_string()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"version\"", "string")});
        return "";
    }

    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
    if(!mcsm::isSafeString(value)){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::unsafeString(value)});
        return "";
    }
    return value;
}

mcsm::Result mcsm::ServerConfigLoader::setServerVersion(const std::string& version){
    if(!this->isLoaded){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
            "ServerConfigLoader function called without loadConfig.",
            "High chance to be an internal issue. Please open an issue in Github."
        }});
        return res;
    }
    if(!mcsm::isSafeString(version)){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::unsafeString(version)});
        return res;
    }
    mcsm::Result setRes = this->optionHandle->setValue("version", version);

    if(!setRes.isSuccess()) return setRes;
    return this->optionHandle->save();
}

std::unique_ptr<mcsm::JvmOption> mcsm::ServerConfigLoader::getDefaultOption() const {
    if(!this->isLoaded){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
            "ServerConfigLoader function called without loadConfig.",
            "High chance to be an internal issue. Please open an issue in Github."
        }});
        return nullptr;
    }

    const nlohmann::json& profileObj = this->optionHandle->getValue("default_launch_profile");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return nullptr;

    if(profileObj == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverDefaultProfileNotFound(this->optionHandle->getName())});
        return nullptr;
    }
    if(profileObj["name"] == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
            "No default launch profile name specified in file " + this->optionHandle->getName(),
            "Manually editing the launch profile might have caused this issue.",
            "If you know what you're doing, I believe you that you know how to handle this issue.",
            "Please report this to GitHub (https://github.com/dodoman8067/mcsm) if you think this is a software issue."
        }});
        return nullptr;      
    }
    if(!profileObj["name"].is_string()){
        // Don't use jsonWrongType
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
            "Value \"name\" in \"default_launch_profile\" has to be a string, but it's not.",
            "Manually editing the launch profile might have caused this issue.",
            "If you know what you're doing, I believe you that you know how to handle this issue.",
            "Please report this to GitHub (https://github.com/dodoman8067/mcsm) if you think this is a software issue."
        }});
        return nullptr;
    }
    if(profileObj["location"] == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
            "No default launch profile location specified in file " + this->optionHandle->getName(),
            "Manually editing the launch profile might have caused this issue.",
            "If you know what you're doing, I believe you that you know how to handle this issue.",
            "Please report this to GitHub (https://github.com/dodoman8067/mcsm) if you think this is a software issue."
        }});
        return nullptr;    
    }
    if(!profileObj["location"].is_string()){
        // Don't use jsonWrongType
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
            "Value \"location\" in \"default_launch_profile\" has to be a string, but it's not.",
            "Manually editing the launch profile might have caused this issue.",
            "If you know what you're doing, I believe you that you know how to handle this issue.",
            "Please report this to GitHub (https://github.com/dodoman8067/mcsm) if you think this is a software issue."
        }});
        return nullptr;
    }
    mcsm::SearchTarget target;
    if(profileObj["location"] == "global"){
        target = mcsm::SearchTarget::GLOBAL;
    }else if(profileObj["location"] == "current"){
        target = mcsm::SearchTarget::CURRENT;
    }else{
        // Don't use jsonWrongType
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
            "Value \"location\" in \"default_launch_profile\" has to be \"global\" or \"current\", but it's not.",
            "Manually editing the launch profile might have caused this issue.",
            "If you know what you're doing, I believe you that you know how to handle this issue.",
            "Please report this to GitHub (https://github.com/dodoman8067/mcsm) if you think this is a software issue."
        }});
        return nullptr;
    }
    std::unique_ptr<mcsm::JvmOption> jvmOption = std::make_unique<mcsm::JvmOption>(profileObj["name"], target);
    if(!jvmOption->exists() || jvmOption == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
            "Invalid default launch profile.",
            "File server.json might be corrupted or the profile is removed.",
            "Please change the profile or create a new server.json file."
        }});
        return nullptr;
    }

    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
    return jvmOption;
}

mcsm::Result mcsm::ServerConfigLoader::setDefaultOption(mcsm::JvmOption& jvmOption){
    if(!this->isLoaded){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
            "ServerConfigLoader function called without loadConfig.",
            "High chance to be an internal issue. Please open an issue in Github."
        }});
        return res;
    }
    nlohmann::json profileObj;
    profileObj["name"] = jvmOption.getProfileName();
    if(jvmOption.getSearchTarget() == mcsm::SearchTarget::GLOBAL){
        profileObj["location"] = "global";
    }else{
        profileObj["location"] = "current";
    }
    mcsm::Result setRes = this->optionHandle->setValue("default_launch_profile", profileObj);

    if(!setRes.isSuccess()) return setRes;
    return this->optionHandle->save();
}

std::string mcsm::ServerConfigLoader::getServerType() const {
    if(!this->isLoaded){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
            "ServerConfigLoader function called without loadConfig.",
            "High chance to be an internal issue. Please open an issue in Github."
        }});
        return "";
    }

    const nlohmann::json& value = this->optionHandle->getValue("type");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";

    if(value == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFound("\"type\"", this->optionHandle->getName())});
        return "";
    }
    if(!value.is_string()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"type\"", "string")});
        return "";
    }

    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
    if(!mcsm::isSafeString(value)){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::unsafeString(value)});
        return "";
    }
    return value;
}

std::string mcsm::ServerConfigLoader::getServerJarFile() const {
    if(!this->isLoaded){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
            "ServerConfigLoader function called without loadConfig.",
            "High chance to be an internal issue. Please open an issue in Github."
        }});
        return "";
    }

    const nlohmann::json& value = this->optionHandle->getValue("server_jar_name");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";

    if(value == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFound("\"server_jar_name\"", this->optionHandle->getName())});
        return "";
    }
    if(!value.is_string()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"server_jar_name\"", "string")});
        return "";
    }

    if(!mcsm::isSafeString(value)){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::unsafeString(value)});
        return "";
    }

    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
    return value;
}

mcsm::Result mcsm::ServerConfigLoader::setServerJarFile(const std::string& name){
    if(!this->isLoaded){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
            "ServerConfigLoader function called without loadConfig.",
            "High chance to be an internal issue. Please open an issue in Github."
        }});
        return res;
    }
    if(!mcsm::isSafeString(name)){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::unsafeString(name)});
        return res;
    }
    mcsm::Result setRes = this->optionHandle->setValue("server_jar_name", name);

    if(!setRes.isSuccess()) return setRes;
    return this->optionHandle->save();
}

std::string mcsm::ServerConfigLoader::getServerJarBuild() const {
    if(!this->isLoaded){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
            "ServerConfigLoader function called without loadConfig.",
            "High chance to be an internal issue. Please open an issue in Github."
        }});
        return "";
    }

    const nlohmann::json& value = this->optionHandle->getValue("server_build");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";

    if(value == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFound("\"server_build\"", this->optionHandle->getName())});
        return "";
    }
    if(!value.is_string()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"server_build\"", "string")});
        return "";
    }

    if(!mcsm::isSafeString(value)){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::unsafeString(value)});
        return "";
    }

    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
    return value;
}

mcsm::Result mcsm::ServerConfigLoader::setServerJarBuild(const std::string& build){
    if(!this->isLoaded){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
            "ServerConfigLoader function called without loadConfig.",
            "High chance to be an internal issue. Please open an issue in Github."
        }});
        return res;
    }
    if(!mcsm::isSafeString(build)){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::unsafeString(build)});
        return res;
    }
    mcsm::Option option(this->configPath, "server");
    mcsm::Result setRes =  this->optionHandle->setValue("server_build", build);

    if(!setRes.isSuccess()) return setRes;
    return this->optionHandle->save();
}

bool mcsm::ServerConfigLoader::doesAutoUpdate() const {
    if(!this->isLoaded){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
            "ServerConfigLoader function called without loadConfig.",
            "High chance to be an internal issue. Please open an issue in Github."
        }});
        return false;
    }

    const nlohmann::json& value = this->optionHandle->getValue("auto_update");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";

    if(value == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFound("\"auto_update\"", this->optionHandle->getName())});
        return "";
    }
    if(!value.is_boolean()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"auto_update\"", "boolean")});
        return "";
    }

    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
    return value;
}

mcsm::Result mcsm::ServerConfigLoader::setAutoUpdate(const bool& update){
    if(!this->isLoaded){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
            "ServerConfigLoader function called without loadConfig.",
            "High chance to be an internal issue. Please open an issue in Github."
        }});
        return res;
    }
    
    mcsm::Result setRes = this->optionHandle->setValue("auto_update", update);
    if(!setRes.isSuccess()) return setRes;
    return this->optionHandle->save();
}

std::unique_ptr<mcsm::Option>& mcsm::ServerConfigLoader::getHandle(){
    return this->optionHandle;
}

bool mcsm::ServerConfigLoader::isFullyLoaded() const {
    return this->isLoaded;
}

std::shared_ptr<mcsm::Server> mcsm::ServerConfigLoader::getServerInstance(){
    if(!this->isLoaded){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
            "ServerConfigLoader function called without loadConfig.",
            "High chance to be an internal issue. Please open an issue in Github."
        }});
        return nullptr;
    }
    std::string sType = getServerType();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return nullptr;

    return mcsm::ServerRegistry::getServerRegistry().getServer(sType);
}