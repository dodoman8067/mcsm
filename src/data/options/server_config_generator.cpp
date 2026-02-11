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
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JVM_PROFILE_NOT_FOUND, {defaultOption.getProfileName()});
        return tl::unexpected(err);
    }
    
    auto canGenerate = validatePath();
    if(!canGenerate) return tl::unexpected(canGenerate.error());

    if(canGenerate.value()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::SERVER_ALREADY_CONFIGURED, {this->configPath});
        return tl::unexpected(err);
    }

    if(!mcsm::isSafeString(version)){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::UNSAFE_STRING, {version});
        return tl::unexpected(err);
    }

    this->optionHandle = std::make_unique<mcsm::TomlOption>(this->configPath, "server");
    bool advp = mcsm::GeneralOption::getGeneralOption().advancedParseEnabled();
        
    mcsm::VoidResult jLoadRes = this->optionHandle->load(advp);
    if(!jLoadRes) return jLoadRes;

    mcsm::VoidResult res1 = sDataOpt->create("none");
    if(!res1) return res1;

    toml::table header;
    toml::value<int64_t> vint(mcsm::SINGLE_CONFIG_VERSION);
    header.insert_or_assign("config_version", vint);

    toml::table meta;
    meta.insert_or_assign("name", valstr(mcsm::safeString(name)));
    meta.insert_or_assign("type", valstr(server->getTypeAsString()));
    meta.insert_or_assign("version", valstr(version));

    auto jvmpPath = defaultOption.getProfilePath();
    if(!jvmpPath) return tl::unexpected(jvmpPath.error());
    std::string jvmpLocation = mcsm::joinPath(jvmpPath.value(), defaultOption.getProfileName() + ".json");

    toml::table jvm;
    jvm.insert_or_assign("profile", valstr(jvmpLocation));

    toml::value<bool> val10(update);
    toml::table jar;
    jar.insert_or_assign("path", valstr(jarPath));
    jar.insert_or_assign("build", valstr(build));
    jar.insert_or_assign("automatic_updates", val10);

    toml::table launch;
    launch.insert_or_assign("run_before", "");
    launch.insert_or_assign("run_command_wrapper", "");
    launch.insert_or_assign("run_after", "");

    toml::table servertoml;
    servertoml.insert_or_assign("meta", meta);
    servertoml.insert_or_assign("jar", jar);
    servertoml.insert_or_assign("launch", launch);
    servertoml.insert_or_assign("jvm", jvm);
    
    mcsm::VoidResult res2 = this->optionHandle->setValue("server", servertoml);
    if(!res2) return res2;
    
    mcsm::VoidResult res5 = this->optionHandle->setValue("header", header);
    if(!res5) return res5;

    mcsm::VoidResult res8 = sDataOpt->updateServerTimeCreated();
    if(!res8) return res8;

    return this->optionHandle->save();
}

mcsm::TomlOption* mcsm::ServerConfigGenerator::getHandle(){
    return this->optionHandle.get();
}

std::string mcsm::ServerConfigGenerator::getPath() const {
    return this->configPath;
}

mcsm::BoolResult mcsm::ServerConfigGenerator::validatePath(){
    mcsm::TomlOption opt(this->configPath, "server");
    return !opt.exists();
}