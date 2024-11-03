#include <mcsm/data/options/server_config_generator.h>
#include <mcsm/data/options/general_option.h>

mcsm::ServerConfigGenerator::ServerConfigGenerator(const std::string& path){
    this->configPath = path;
    this->optionHandle = nullptr;
}

mcsm::ServerConfigGenerator::~ServerConfigGenerator(){

}

mcsm::Result mcsm::ServerConfigGenerator::generate(const std::string& version, mcsm::Server* server, mcsm::ServerDataOption* sDataOpt, const std::string& name, mcsm::JvmOption& defaultOption){
    return generate(version, server, sDataOpt, name, defaultOption, true, "latest");
}

mcsm::Result mcsm::ServerConfigGenerator::generate(const std::string& version, mcsm::Server* server, mcsm::ServerDataOption* sDataOpt, const std::string& name, mcsm::JvmOption& defaultOption, const bool& update){
    return generate(version, server, sDataOpt, name, defaultOption, update, "latest");
}

mcsm::Result mcsm::ServerConfigGenerator::generate(const std::string& version, mcsm::Server* server, mcsm::ServerDataOption* sDataOpt, const std::string& name, mcsm::JvmOption& defaultOption, const bool& update, const std::string& build){
    bool jvmOptionExists = defaultOption.exists();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    if(!jvmOptionExists){
        return {mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jvmProfileNotFound()};
    }
    
    bool canGenerate = validatePath();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    if(!canGenerate){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverAlreadyConfigured(this->configPath)});
        return res;
    }

    this->optionHandle = std::make_unique<mcsm::Option>(this->configPath, "server");
    bool advp = mcsm::GeneralOption::getGeneralOption().advancedParseEnabled();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }
        
    mcsm::Result jLoadRes = this->optionHandle->load(advp);
    if(!jLoadRes.isSuccess()) return jLoadRes;

    mcsm::Result res1 = sDataOpt->create("none");
    if(!res1.isSuccess()) return res1;

    nlohmann::json profileObj;
    profileObj["name"] = defaultOption.getProfileName();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    if(defaultOption.getSearchTarget() == mcsm::SearchTarget::GLOBAL){
        profileObj["location"] = "global";
    }else{
        profileObj["location"] = "current";
    }
    
    mcsm::Result res2 = this->optionHandle->setValue("name", mcsm::safeString(name));
    if(!res2.isSuccess()) return res2;
    
    if(!mcsm::isSafeString(version)){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::unsafeString(version)});
        return res;
    }
    mcsm::Result res3 = this->optionHandle->setValue("version", version);
    if(!res3.isSuccess()) return res3;
    
    mcsm::Result res4 = this->optionHandle->setValue("default_launch_profile", profileObj);
    if(!res4.isSuccess()) return res4;

    std::string jarFile = server->getTypeAsString() + ".jar";

    if(!mcsm::isSafeString(jarFile)){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::unsafeString(jarFile)});
        return res;
    }
    mcsm::Result res5 = this->optionHandle->setValue("server_jar_name", jarFile);
    if(!res5.isSuccess()) return res5;

    mcsm::Result res6 = this->optionHandle->setValue("server_build", build);
    if(!res6.isSuccess()) return res6;

    mcsm::Result res10 = this->optionHandle->setValue("auto_update", update);
    if(!res10.isSuccess()) return res10;

    mcsm::Result res7 = this->optionHandle->setValue("type", server->getTypeAsString());
    if(!res7.isSuccess()) return res7;

    mcsm::Result res8 = sDataOpt->updateServerTimeCreated();
    if(!res8.isSuccess()) return res8;

    return this->optionHandle->save();
}

std::unique_ptr<mcsm::Option>& mcsm::ServerConfigGenerator::getHandle(){
    return this->optionHandle;
}

std::string mcsm::ServerConfigGenerator::getPath() const {
    return this->configPath;
}

bool mcsm::ServerConfigGenerator::validatePath(){
    mcsm::Option opt(this->configPath, "server");
    return !opt.exists();
}