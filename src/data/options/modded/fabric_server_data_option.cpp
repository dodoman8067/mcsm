#include <mcsm/data/options/modded/fabric_server_data_option.h>

mcsm::FabricServerDataOption::FabricServerDataOption() : ServerDataOption() {}

mcsm::FabricServerDataOption::FabricServerDataOption(const std::string& path) : ServerDataOption(path) {}

mcsm::FabricServerDataOption::~FabricServerDataOption(){}

std::string mcsm::FabricServerDataOption::getLoaderVersion() const {
    bool optExists = this->option->exists();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";
    if(!optExists){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::fileNotFound(this->option->getName())});
        return "";
    }

    const nlohmann::json& value = this->option->getValue("last_downloaded_loader_version");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";

    if(value == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFound("\"last_downloaded_loader_version\"", this->option->getName())});
        return "";
    }
    if(!value.is_string()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"last_downloaded_loader_version\"", "string")});
        return "";
    }

    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
    return mcsm::safeString(value);
}

mcsm::Result mcsm::FabricServerDataOption::updateLoaderVersion(const std::string& version){
    if(!mcsm::isSafeString(version)){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::unsafeString(version)});
        return res;
    }
    mcsm::Result res1 = this->option->setValue("last_downloaded_loader_version", mcsm::safeString(version));
    if(!res1.isSuccess()) return res1;

    return this->option->save();
}

std::string mcsm::FabricServerDataOption::getInstallerVersion() const {
    bool optExists = this->option->exists();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";
    if(!optExists){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::fileNotFound(this->option->getName())});
        return "";
    }

    const nlohmann::json& value = this->option->getValue("last_downloaded_installer_version");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";

    if(value == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFound("\"last_downloaded_installer_version\"", this->option->getName())});
        return "";
    }
    if(!value.is_string()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"last_downloaded_installer_version\"", "string")});
        return "";
    }

    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
    return mcsm::safeString(value);
}

mcsm::Result mcsm::FabricServerDataOption::updateInstallerVersion(const std::string& version){
    mcsm::Result res = this->option->setValue("last_downloaded_installer_version", mcsm::safeString(version));
    if(!res.isSuccess()) return res;

    return this->option->save();
}