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

mcsm::VoidResult mcsm::ServerConfigLoader::loadConfig(){
    this->optionHandle = std::make_unique<mcsm::Option>(this->configPath, "server");
    auto optExists = this->optionHandle->exists();
    if(!optExists) return tl::unexpected(optExists.error());

    if(!optExists.value()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::SERVER_NOT_CONFIGURED, {this->configPath});
        return tl::unexpected(err);
    }

    bool advp = mcsm::GeneralOption::getGeneralOption().advancedParseEnabled();

    auto lRes = this->optionHandle->load(advp);
    if(!lRes) return lRes;

    this->isLoaded = true;
    return {};
}

template <>
nlohmann::json::value_t mcsm::ServerConfigLoader::getJsonType<int>() const {
    return nlohmann::json::value_t::number_integer;
}   

template <>
nlohmann::json::value_t mcsm::ServerConfigLoader::getJsonType<unsigned int>() const {
    return nlohmann::json::value_t::number_unsigned;
}   

template <>
nlohmann::json::value_t mcsm::ServerConfigLoader::getJsonType<float>() const {
    return nlohmann::json::value_t::number_float;
}   

template <>
nlohmann::json::value_t mcsm::ServerConfigLoader::getJsonType<double>() const {
    return nlohmann::json::value_t::number_float;
}

template <>
nlohmann::json::value_t mcsm::ServerConfigLoader::getJsonType<bool>() const {
    return nlohmann::json::value_t::boolean;
}

template <>
nlohmann::json::value_t mcsm::ServerConfigLoader::getJsonType<std::string>() const {
    return nlohmann::json::value_t::string;
}

// use json.is_array to get arrays

template <>
nlohmann::json::value_t mcsm::ServerConfigLoader::getJsonType<nlohmann::json>() const {
    return nlohmann::json::value_t::object;
}

template <>
nlohmann::json::value_t mcsm::ServerConfigLoader::getJsonType<std::vector<nlohmann::json>>() const {
    return nlohmann::json::value_t::array;
}

// isLoaded won't be "true" if the option does not exist which is why I don't check the existence of the option file.

mcsm::StringResult mcsm::ServerConfigLoader::getServerName() const {
    if(!this->isLoaded){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::SERVER_DATA_ACCESSED_WITHOUT_LOAD, {});
        return tl::unexpected(err);
    }

    auto valueRes = this->optionHandle->getValue("name");
    if(!valueRes) return tl::unexpected(valueRes.error());

    nlohmann::json value = valueRes.value();

    if(value == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_NOT_FOUND, {"\"name\"", this->optionHandle->getName()});
        return tl::unexpected(err);
    }
    if(!value.is_string()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_WRONG_TYPE, {"\"name\"", "string"});
        return tl::unexpected(err);
    }

    if(!mcsm::isSafeString(value)){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::UNSAFE_STRING, {value});
        return tl::unexpected(err);
    }
    return value;
}

mcsm::VoidResult mcsm::ServerConfigLoader::setServerName(const std::string& name){
    if(!this->isLoaded){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::SERVER_DATA_ACCESSED_WITHOUT_LOAD, {});
        return tl::unexpected(err);
    }
    if(!mcsm::isSafeString(name)){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::UNSAFE_STRING, {name});
        return tl::unexpected(err);
    }
    auto setRes = this->optionHandle->setValue("name", name);

    if(!setRes) return setRes;
    return this->optionHandle->save();
}

mcsm::StringResult mcsm::ServerConfigLoader::getServerVersion() const {
    if(!this->isLoaded){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::SERVER_DATA_ACCESSED_WITHOUT_LOAD, {});
        return tl::unexpected(err);
    }

    auto valueRes = this->optionHandle->getValue("version");
    if(!valueRes) return tl::unexpected(valueRes.error());

    const nlohmann::json& value = valueRes.value();

    if(value == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_NOT_FOUND, {"\"version\"", this->optionHandle->getName()});
        return tl::unexpected(err);
    }
    if(!value.is_string()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_WRONG_TYPE, {"\"version\"", "string"});
        return tl::unexpected(err);
    }

    if(!mcsm::isSafeString(value)){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::UNSAFE_STRING, {value});
        return tl::unexpected(err);
    }
    return value;
}

mcsm::VoidResult mcsm::ServerConfigLoader::setServerVersion(const std::string& version){
    if(!this->isLoaded){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::SERVER_DATA_ACCESSED_WITHOUT_LOAD, {});
        return tl::unexpected(err);
    }
    if(!mcsm::isSafeString(version)){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::UNSAFE_STRING, {version});
        return tl::unexpected(err);
    }
    auto setRes = this->optionHandle->setValue("version", version);

    if(!setRes) return setRes;
    return this->optionHandle->save();
}

tl::expected<std::unique_ptr<mcsm::JvmOption>, mcsm::Error> mcsm::ServerConfigLoader::getDefaultOption() const {
    if(!this->isLoaded){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::SERVER_DATA_ACCESSED_WITHOUT_LOAD, {});
        return tl::unexpected(err);
    }

    auto pORes = this->optionHandle->getValue("default_launch_profile");
    const nlohmann::json& profileObj = pORes.value();

    if(profileObj == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::SERVER_DEFAULT_PROFILE_NOT_FOUND, {this->optionHandle->getName()});
        return tl::unexpected(err);
    }
    if(profileObj["name"] == nullptr){
        auto customTemp = mcsm::errors::JSON_NOT_FOUND;
        customTemp.message = "No default launch profile name specified in file " + this->optionHandle->getName();
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, customTemp, {});
        return tl::unexpected(err);
    }
    if(!profileObj["name"].is_string()){
        // Don't use jsonWrongType
        auto customTemp = mcsm::errors::JSON_WRONG_TYPE;
        customTemp.message = "Value \"name\" in \"default_launch_profile\" has to be a string, but it's not.\nManually editing the launch profile might have caused this issue.";
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, customTemp, {});
        return tl::unexpected(err);
    }
    if(profileObj["location"] == nullptr){
        auto customTemp = mcsm::errors::JSON_NOT_FOUND;
        customTemp.message = "No default launch location name specified in file " + this->optionHandle->getName();
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, customTemp, {});
        return tl::unexpected(err);
    }
    if(!profileObj["location"].is_string()){
        // Don't use jsonWrongType
        auto customTemp = mcsm::errors::JSON_WRONG_TYPE;
        customTemp.message = "Value \"location\" in \"default_launch_profile\" has to be a string, but it's not.\nManually editing the launch profile might have caused this issue.";
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, customTemp, {});
        return tl::unexpected(err);
    }
    mcsm::SearchTarget target;
    if(profileObj["location"] == "global"){
        target = mcsm::SearchTarget::GLOBAL;
    }else if(profileObj["location"] == "current"){
        target = mcsm::SearchTarget::CURRENT;
    }else{
        // Don't use jsonWrongType
        auto customTemp = mcsm::errors::JSON_WRONG_TYPE;
        customTemp.message = "Value \"location\" in \"default_launch_profile\" has to be \"global\" or \"current\", but it's not.\nManually editing the launch profile might have caused this issue.";
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, customTemp, {});
        return tl::unexpected(err);
    }
    std::unique_ptr<mcsm::JvmOption> jvmOption = std::make_unique<mcsm::JvmOption>(profileObj["name"], target, this->configPath);
    auto jvmExts = jvmOption->exists();
    if(!jvmExts) return tl::unexpected(jvmExts.error());

    if(!jvmExts.value() || jvmOption == nullptr){
        auto customTemp = mcsm::errors::SERVER_DEFAULT_PROFILE_NOT_FOUND;
        customTemp.message = "Invalid default launch profile.\nFile server.json may be corrupted or the profile is removed.";
        customTemp.solution = "Please change the profile or create a new server.json file.";
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, customTemp, {});
        return tl::unexpected(err);
    }

    return jvmOption;
}

mcsm::VoidResult mcsm::ServerConfigLoader::setDefaultOption(mcsm::JvmOption& jvmOption){
    if(!this->isLoaded){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::SERVER_DATA_ACCESSED_WITHOUT_LOAD, {});
        return tl::unexpected(err);
    }
    nlohmann::json profileObj;
    profileObj["name"] = jvmOption.getProfileName();
    if(jvmOption.getSearchTarget() == mcsm::SearchTarget::GLOBAL){
        profileObj["location"] = "global";
    }else{
        profileObj["location"] = "current";
    }
    auto setRes = this->optionHandle->setValue("default_launch_profile", profileObj);

    if(!setRes) return setRes;
    return this->optionHandle->save();
}

mcsm::StringResult mcsm::ServerConfigLoader::getServerType() const {
    if(!this->isLoaded){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::SERVER_DATA_ACCESSED_WITHOUT_LOAD, {});
        return tl::unexpected(err);
    }

    auto valueRes = this->optionHandle->getValue("type");
    if(!valueRes) return tl::unexpected(valueRes.error());
    const nlohmann::json& value = valueRes.value();

    if(value == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_NOT_FOUND, {"\"type\"", this->optionHandle->getName()});
        return tl::unexpected(err);
    }
    if(!value.is_string()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_WRONG_TYPE, {"\"type\"", "string"});
        return tl::unexpected(err);
    }

    if(!mcsm::isSafeString(value)){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::UNSAFE_STRING, {value});
        return tl::unexpected(err);
    }
    return value;
}

mcsm::StringResult mcsm::ServerConfigLoader::getServerJarFile() const {
    if(!this->isLoaded){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::SERVER_DATA_ACCESSED_WITHOUT_LOAD, {});
        return tl::unexpected(err);
    }

    auto valueRes = this->optionHandle->getValue("server_jar_name");
    if(!valueRes) return tl::unexpected(valueRes.error());
    const nlohmann::json& value = valueRes.value();

    if(value == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_NOT_FOUND, {"\"server_jar_name\"", this->optionHandle->getName()});
        return tl::unexpected(err);
    }
    if(!value.is_string()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_WRONG_TYPE, {"\"server_jar_name\"", "string"});
        return tl::unexpected(err);
    }

    if(!mcsm::isSafeString(value)){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::UNSAFE_STRING, {value});
        return tl::unexpected(err);
    }
    return value;
}

mcsm::VoidResult mcsm::ServerConfigLoader::setServerJarFile(const std::string& name){
    if(!this->isLoaded){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::SERVER_DATA_ACCESSED_WITHOUT_LOAD, {});
        return tl::unexpected(err);
    }
    if(!mcsm::isSafeString(name)){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::UNSAFE_STRING, {name});
        return tl::unexpected(err);
    }
    auto setRes = this->optionHandle->setValue("server_jar_name", name);

    if(!setRes) return setRes;
    return this->optionHandle->save();
}

mcsm::StringResult mcsm::ServerConfigLoader::getServerJarBuild() const {
    if(!this->isLoaded){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::SERVER_DATA_ACCESSED_WITHOUT_LOAD, {});
        return tl::unexpected(err);
    }

    auto valueRes = this->optionHandle->getValue("server_build");
    if(!valueRes) return tl::unexpected(valueRes.error());
    const nlohmann::json& value = valueRes.value();

    if(value == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_NOT_FOUND, {"\"server_build\"", this->optionHandle->getName()});
        return tl::unexpected(err);
    }
    if(!value.is_string()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_WRONG_TYPE, {"\"server_build\"", "string"});
        return tl::unexpected(err);
    }

    if(!mcsm::isSafeString(value)){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::UNSAFE_STRING, {value});
        return tl::unexpected(err);
    }

    return value;
}

mcsm::VoidResult mcsm::ServerConfigLoader::setServerJarBuild(const std::string& build){
    if(!this->isLoaded){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::SERVER_DATA_ACCESSED_WITHOUT_LOAD, {});
        return tl::unexpected(err);
    }
    if(!mcsm::isSafeString(build)){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::UNSAFE_STRING, {build});
        return tl::unexpected(err);
    }
    auto setRes =  this->optionHandle->setValue("server_build", build);

    if(!setRes) return setRes;
    return this->optionHandle->save();
}

mcsm::BoolResult mcsm::ServerConfigLoader::doesAutoUpdate() const {
    if(!this->isLoaded){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::SERVER_DATA_ACCESSED_WITHOUT_LOAD, {});
        return tl::unexpected(err);
    }

    auto valueRes = this->optionHandle->getValue("auto_update");
    if(!valueRes) return tl::unexpected(valueRes.error());
    const nlohmann::json& value = valueRes.value();

    if(value == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_NOT_FOUND, {"\"auto_update\"", this->optionHandle->getName()});
        return tl::unexpected(err);
    }
    if(!value.is_boolean()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_WRONG_TYPE, {"\"auto_update\"", "boolean"});
        return tl::unexpected(err);
    }

    return value;
}

mcsm::VoidResult mcsm::ServerConfigLoader::setAutoUpdate(const bool& update){
    if(!this->isLoaded){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::SERVER_DATA_ACCESSED_WITHOUT_LOAD, {});
        return tl::unexpected(err);
    }
    
    auto setRes = this->optionHandle->setValue("auto_update", update);
    if(!setRes) return setRes;
    return this->optionHandle->save();
}

mcsm::Option* mcsm::ServerConfigLoader::getHandle() const {
    return this->optionHandle.get();
}

bool mcsm::ServerConfigLoader::isFullyLoaded() const {
    return this->isLoaded;
}

tl::expected<mcsm::Server*, mcsm::Error> mcsm::ServerConfigLoader::getServerInstance(){
    if(!this->isLoaded){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::SERVER_DATA_ACCESSED_WITHOUT_LOAD, {});
        return tl::unexpected(err);
    }
    auto sType = getServerType();
    if(!sType) return tl::unexpected(sType.error());

    return mcsm::ServerRegistry::getServerRegistry().getServer(sType.value());
}