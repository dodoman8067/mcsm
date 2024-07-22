#include <mcsm/data/options/modded/fabric_server_option.h>

// You might be wondering why server option crating class would fail due to file non existent; It's because the first three constructors are for loading the existing config.
mcsm::FabricServerOption::FabricServerOption() : FabricServerOption(mcsm::getCurrentPath()){}

mcsm::FabricServerOption::FabricServerOption(const std::string& version, const std::string& path){
    // Result handle
    bool fileExists = mcsm::fileExists(path);
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return;

    if(!fileExists){
        if(!mcsm::mkdir(path)){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
                "Path mkdir failed : " + path,
                "High chance to be a software issue, please report this to GitHub (https://github.com/dodoman8067/mcsm)."
            }});
            return;
        }
    }
    mcsm::Option option(path, "server");

    bool exists = option.exists();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return;

    if(!exists){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverNotConfigured(path)});
        return;
    }

    // Gets the server type from json file.
    nlohmann::json type = option.getValue("type");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return;

    if(type == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFound("\"type\"", "server.json")});
        return; 
    }
    if(!type.is_string()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"type\"", "string")});
        return; 
    }

    // Creates a server instance with the string obtained from from option.getValue("type"). It has to be a Fabric server.
    std::shared_ptr<mcsm::Server> sPtr = mcsm::ServerRegistry::getServerRegistry().getServer(type);
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return;

    // Checks if the server json file is for fabric servers
    std::string sType = sPtr->getTypeAsString();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return;
    if(sType != "fabric"){
        // Returns an error result and call "return" so without error handling using this class would lead to undefined behaviour.
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
            "Class mcsm::FabricServerOption was constructed while non Fabric server pointer was passed as a parameter.",
            "This has a very high chance to be a software issue, please report this to GitHub (https://github.com/dodoman8067/mcsm)."
        }});
        return;
    }

    this->path = path;
    this->version = version;
    this->server = sPtr;
    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
}

mcsm::FabricServerOption::FabricServerOption(const std::string& path){
    // Checks if the path exists(not file)
    bool fileExists = mcsm::fileExists(path);
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return;

    if(!fileExists){
        if(!mcsm::mkdir(path)){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
                "Path mkdir failed : " + path,
                "High chance to be a software issue, please report this to GitHub (https://github.com/dodoman8067/mcsm)."
            }});
            return;
        }
    }
    mcsm::Option option(path, "server");

    bool exists = option.exists();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return;

    if(!exists){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverNotConfigured(path)});
        return;
    }

    nlohmann::json type = option.getValue("type");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return;

    if(type == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFound("\"type\"", "server.json")});
        return; 
    }
    if(!type.is_string()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"type\"", "string")});
        return; 
    }

    nlohmann::json version = option.getValue("version");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return;

    if(version == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFound("\"version\"", "server.json")});
        return;   
    }
    if(!version.is_string()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"version\"", "string")});
        return; 
    }

    std::shared_ptr<mcsm::Server> sPtr = mcsm::ServerRegistry::getServerRegistry().getServer(type);
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return;
    std::string sType = sPtr->getTypeAsString();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return;
    if(sType != "fabric"){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
            "Class mcsm::FabricServerOption was constructed while non Fabric server pointer was passed as a parameter.",
            "This has a very high chance to be a software issue, please report this to GitHub (https://github.com/dodoman8067/mcsm)."
        }});
        return;
    }
    this->path = path;
    this->server = sPtr;
    this->version = version;
    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
}

mcsm::FabricServerOption::FabricServerOption(const std::string& version, std::shared_ptr<mcsm::Server> server){
    this->version = version;
    this->server = server;
    std::string sType = server->getTypeAsString();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return;
    if(sType != "fabric"){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
            "Class mcsm::FabricServerOption was constructed while non Fabric server pointer was passed as a parameter.",
            "This has a very high chance to be a software issue, please report this to GitHub (https://github.com/dodoman8067/mcsm)."
        }});
        return;
    }
    std::string path1 = mcsm::getCurrentPath();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return;
    this->path = path1;
    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
}

mcsm::FabricServerOption::FabricServerOption(const std::string& version, std::shared_ptr<mcsm::Server> server, const std::string& path){
    bool fileExists = mcsm::fileExists(path);
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return;

    if(!fileExists){
        if(!mcsm::mkdir(path)){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
                "Path mkdir failed : " + path,
                "High chance to be a software issue, please report this to GitHub (https://github.com/dodoman8067/mcsm)."
            }});
            return;
        }
    }

    this->version = version;
    this->server = server;
    this->path = path;
    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
}

mcsm::FabricServerOption::~FabricServerOption(){

}

mcsm::Result mcsm::FabricServerOption::create(const std::string& name, mcsm::JvmOption& defaultOption){
    return create(name, defaultOption, true);
}

mcsm::Result mcsm::FabricServerOption::create(const std::string& name, mcsm::JvmOption& defaultOption, const bool& update){
    mcsm::Option option(this->path, "server");
    
    bool optExists = option.exists();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    if(optExists){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverAlreadyConfigured(this->path)});
        return res;
    }

    mcsm::FabricServerDataOption serverDataOpt(this->path);
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    mcsm::Result res1 = serverDataOpt.create("none");
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
    
    mcsm::Result res2 = option.setValue("name", name);
    if(!res2.isSuccess()) return res2;
    
    mcsm::Result res3 = option.setValue("version", this->version);
    if(!res3.isSuccess()) return res3;
    
    mcsm::Result res4 = option.setValue("default_launch_profile", profileObj);
    if(!res4.isSuccess()) return res4;
    
    std::string jarFile = this->server->getJarFile(this->path);
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        jarFile = "fabric.jar";
    }

    mcsm::Result res5 = option.setValue("server_jar", jarFile);
    if(!res5.isSuccess()) return res5;
    
    mcsm::Result res6 = option.setValue("server_build", "latest");
    if(!res6.isSuccess()) return res6;

    mcsm::Result res12 = option.setValue("auto_update", update);
    if(!res12.isSuccess()) return res12;
    
    mcsm::Result res7 = option.setValue("type", this->server->getTypeAsString());
    if(!res7.isSuccess()) return res7;
    
    mcsm::Result res8 = serverDataOpt.updateServerTimeCreated();
    if(!res8.isSuccess()) return res8;

    mcsm::Result res9 = option.setValue("loader_version", "latest");
    if(!res9.isSuccess()) return res8;

    mcsm::Result res10 = option.setValue("installer_version", "latest");
    if(!res10.isSuccess()) return res8;

    mcsm::Result res11({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
    return res11;
}

mcsm::Result mcsm::FabricServerOption::start(){
    std::unique_ptr<mcsm::JvmOption> jvmOpt = getDefaultOption();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }
    return start(*jvmOpt, this->path, this->path);
}

mcsm::Result mcsm::FabricServerOption::start(mcsm::JvmOption& option){
    return start(option, this->path, this->path);
}

mcsm::Result mcsm::FabricServerOption::start(mcsm::JvmOption& option, const std::string& path, const std::string& optionPath){
    mcsm::FabricServerDataOption serverDataOpt(optionPath);
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    bool fileExists = mcsm::fileExists(optionPath + "/server.json");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    if(!fileExists){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverNotConfigured(optionPath)});
        return res;
    }

    std::string type = getServerType();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    if(type != "fabric"){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
            "Specified server path's server option wasn't for Fabric servers.",
            "Please try again in other directories."
        }});
        return res;
    }

    if(this->server == nullptr){
        this->server.reset();
        this->server = std::make_shared<mcsm::FabricServer>();
        /*
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }*/
    }

    std::string name = getServerName();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    std::string version = getServerVersion();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    std::string profileName = option.getProfileName();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    mcsm::success("Starting server..");
    mcsm::info("Server name : " + name);
    mcsm::info("Server MC version : " + version);
    mcsm::info("Server JVM launch profile : " + profileName);
    mcsm::Result res = serverDataOpt.updateLastTimeLaunched();
    if(!res.isSuccess()) return res;
    mcsm::Result res2 = this->server->start(option, path, optionPath);
    return res2;
}

std::string mcsm::FabricServerOption::getServerJarBuild() const {
    return getLoaderVersion();
}

mcsm::Result mcsm::FabricServerOption::setServerJarBuild(const std::string& build){
    return setLoaderVersion(build);
}

std::string mcsm::FabricServerOption::getLoaderVersion() const {
    mcsm::Option option(this->path, "server");
    bool optExists = option.exists();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";
    if(!optExists){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverNotConfigured(this->path)});
        return "";
    }

    nlohmann::json value = option.getValue("loader_version");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";

    if(value == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFound("\"loader_version\"", option.getName())});
        return "";
    }
    if(!value.is_string()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"loader_version\"", "string")});
        return "";
    }

    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
    return value;
}

mcsm::Result mcsm::FabricServerOption::setLoaderVersion(const std::string& version){
    mcsm::Option option(this->path, "server");
    return option.setValue("loader_version", version);
}

std::string mcsm::FabricServerOption::getInstallerVersion() const{
    mcsm::Option option(this->path, "server");
    bool optExists = option.exists();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";
    if(!optExists){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverNotConfigured(this->path)});
        return "";
    }

    nlohmann::json value = option.getValue("installer_version");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";

    if(value == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFound("\"installer_version\"", option.getName())});
        return "";
    }
    if(!value.is_string()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"installer_version\"", "string")});
        return "";
    }

    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
    return value;
}

mcsm::Result mcsm::FabricServerOption::setInstallerVersion(const std::string& version){
    mcsm::Option option(this->path, "server");
    return option.setValue("installer_version", version);
}

bool mcsm::FabricServerOption::exists(){
    mcsm::Option option(this->path, "server");
    bool rt = option.exists();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return false;
    return rt;
}

std::string mcsm::FabricServerOption::getOptionPath() const {
    return this->path;
}

std::unique_ptr<mcsm::JvmOption> mcsm::FabricServerOption::getDefaultOption() const{
    mcsm::Option option(this->path, "server");
    bool optExists = option.exists();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return nullptr;
    if(!optExists){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverNotConfigured(this->path)});
        return nullptr;
    }
    nlohmann::json profileObj = option.getValue("default_launch_profile");
    if(profileObj == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverDefaultProfileNotFound(option.getName())});
        return nullptr;
    }
    if(profileObj["name"] == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
            "No default launch profile name specified in file " + option.getName(),
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
            "No default launch profile location specified in file " + option.getName(),
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

mcsm::Result mcsm::FabricServerOption::setDefaultOption(mcsm::JvmOption& jvmOption){
    mcsm::Option option(this->path, "server");
    nlohmann::json profileObj;
    profileObj["name"] = jvmOption.getProfileName();
    if(jvmOption.getSearchTarget() == mcsm::SearchTarget::GLOBAL){
        profileObj["location"] = "global";
    }else{
        profileObj["location"] = "current";
    }
    return option.setValue("default_launch_profile", profileObj);
}

std::string mcsm::FabricServerOption::getServerName() const {
    mcsm::Option option(this->path, "server");
    bool optExists = option.exists();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";
    if(!optExists){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverNotConfigured(this->path)});
        return "";
    }

    nlohmann::json value = option.getValue("name");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";

    if(value == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFound("\"name\"", option.getName())});
        return "";
    }
    if(!value.is_string()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"name\"", "string")});
        return "";
    }

    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
    return value;
}

mcsm::Result mcsm::FabricServerOption::setServerName(const std::string& name){
    mcsm::Option option(this->path, "server");
    return option.setValue("name", name);
}

std::string mcsm::FabricServerOption::getServerVersion() const {
    mcsm::Option option(this->path, "server");
    bool optExists = option.exists();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";
    if(!optExists){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverNotConfigured(this->path)});
        return "";
    }

    nlohmann::json value = option.getValue("version");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";

    if(value == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFound("\"version\"", option.getName())});
        return "";
    }
    if(!value.is_string()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"version\"", "string")});
        return "";
    }

    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
    return value;
}

mcsm::Result mcsm::FabricServerOption::setServerVersion(const std::string& version){
    mcsm::Option option(this->path, "server");
    return option.setValue("version", version);    
}

std::string mcsm::FabricServerOption::getServerType() const {
    mcsm::Option option(this->path, "server");
    bool optExists = option.exists();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";
    if(!optExists){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverNotConfigured(this->path)});
        return "";
    }

    nlohmann::json value = option.getValue("type");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";

    if(value == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFound("\"type\"", option.getName())});
        return "";
    }
    if(!value.is_string()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"type\"", "string")});
        return "";
    }

    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
    return value;
}

std::string mcsm::FabricServerOption::getServerJarFile() const{
    mcsm::Option option(this->path, "server");
    bool optExists = option.exists();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";
    if(!optExists){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverNotConfigured(this->path)});
        return "";
    }

    nlohmann::json value = option.getValue("server_jar");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";

    if(value == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFound("\"server_jar\"", option.getName())});
        return "";
    }
    if(!value.is_string()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"server_jar\"", "string")});
        return "";
    }

    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
    return value;
}

mcsm::Result mcsm::FabricServerOption::setServerJarFile(const std::string& name){
    mcsm::Option option(this->path, "server");
    return option.setValue("server_jar", name);
}

bool mcsm::FabricServerOption::doesAutoUpdate() const {
    mcsm::Option option(this->path, "server");
    bool optExists = option.exists();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";
    if(!optExists){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverNotConfigured(this->path)});
        return "";
    }

    nlohmann::json value = option.getValue("auto_update");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";

    if(value == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFound("\"auto_update\"", option.getName())});
        return "";
    }
    if(!value.is_boolean()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"auto_update\"", "boolean")});
        return "";
    }

    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
    return value;
}

mcsm::Result mcsm::FabricServerOption::setAutoUpdate(const bool& update){
    mcsm::Option option(this->path, "server");
    return option.setValue("auto_update", update);
}

std::shared_ptr<mcsm::Server> mcsm::FabricServerOption::getServer() const {
    if(this->server != nullptr){ mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}}); return this->server; }
    mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
        "Server instance null.",
        "There's a high chance to be a software issue. please report this to GitHub (https://github.com/dodoman8067/mcsm)."
    }});
    return nullptr;
}