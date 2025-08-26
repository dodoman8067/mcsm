#include <mcsm/data/options/server_config_generator.h>
#include <mcsm/data/options/general_option.h>

mcsm::ServerConfigGenerator::ServerConfigGenerator(const std::string& path){
    this->configPath = path;
    this->optionHandle = nullptr;
}

mcsm::ServerConfigGenerator::~ServerConfigGenerator(){

}

mcsm::VoidResult mcsm::ServerConfigGenerator::generate(const std::string& version, mcsm::Server* server, mcsm::ServerDataOption* sDataOpt, const std::string& name, mcsm::JvmOption& defaultOption){
    return generate(version, server, sDataOpt, name, defaultOption, true, "latest");
}

mcsm::VoidResult mcsm::ServerConfigGenerator::generate(const std::string& version, mcsm::Server* server, mcsm::ServerDataOption* sDataOpt, const std::string& name, mcsm::JvmOption& defaultOption, const bool& update){
    return generate(version, server, sDataOpt, name, defaultOption, update, "latest");
}

mcsm::VoidResult mcsm::ServerConfigGenerator::generate(const std::string& version, mcsm::Server* server, mcsm::ServerDataOption* sDataOpt, const std::string& name, mcsm::JvmOption& defaultOption, const bool& update, const std::string& build){
    return generate(version, server, sDataOpt, name, defaultOption, update, build, server->getTypeAsString() + ".jar");
}

mcsm::VoidResult mcsm::ServerConfigGenerator::generate(const std::string& version, mcsm::Server* server, mcsm::ServerDataOption* sDataOpt, const std::string& name, mcsm::JvmOption& defaultOption, const bool& update, const std::string& build, const std::string& jarPath){
    auto jvmOptionExists = defaultOption.exists();
    if(!jvmOptionExists) return tl::unexpected(jvmOptionExists.error());

    if(!jvmOptionExists.value()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JVM_PROFILE_NOT_FOUND, {defaultOption.getProfileName()});
        return tl::unexpected(err);
    }
    
    auto canGenerate = validatePath();
    if(!canGenerate) return tl::unexpected(canGenerate.error());

    if(canGenerate.value()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::SERVER_ALREADY_CONFIGURED, {this->configPath});
        return tl::unexpected(err);
    }

    this->optionHandle = std::make_unique<mcsm::Option>(this->configPath, "server");
    bool advp = mcsm::GeneralOption::getGeneralOption().advancedParseEnabled();
        
    mcsm::VoidResult jLoadRes = this->optionHandle->load(advp);
    if(!jLoadRes) return jLoadRes;

    mcsm::VoidResult res1 = sDataOpt->create("none");
    if(!res1) return res1;

    nlohmann::json profileObj;
    profileObj["name"] = defaultOption.getProfileName();
    if(defaultOption.getSearchTarget() == mcsm::SearchTarget::GLOBAL){
        profileObj["location"] = "global";
    }else{
        profileObj["location"] = "current";
    }
    
    mcsm::VoidResult res2 = this->optionHandle->setValue("name", mcsm::safeString(name));
    if(!res2) return res2;
    
    if(!mcsm::isSafeString(version)){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::UNSAFE_STRING, {version});
        return tl::unexpected(err);
    }
    mcsm::VoidResult res3 = this->optionHandle->setValue("version", version);
    if(!res3) return res3;
    
    mcsm::VoidResult res4 = this->optionHandle->setValue("default_launch_profile", profileObj);
    if(!res4) return res4;

    mcsm::VoidResult res5 = this->optionHandle->setValue("server_jar", jarPath);
    if(!res5) return res5;

    mcsm::VoidResult res6 = this->optionHandle->setValue("server_build", build);
    if(!res6) return res6;

    mcsm::VoidResult res10 = this->optionHandle->setValue("auto_update", update);
    if(!res10) return res10;

    mcsm::VoidResult res7 = this->optionHandle->setValue("type", server->getTypeAsString());
    if(!res7) return res7;

    mcsm::VoidResult res8 = sDataOpt->updateServerTimeCreated();
    if(!res8) return res8;

    return this->optionHandle->save();
}

mcsm::Option* mcsm::ServerConfigGenerator::getHandle(){
    return this->optionHandle.get();
}

std::string mcsm::ServerConfigGenerator::getPath() const {
    return this->configPath;
}

mcsm::BoolResult mcsm::ServerConfigGenerator::validatePath(){
    mcsm::Option opt(this->configPath, "server");
    return !opt.exists();
}