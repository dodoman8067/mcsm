#include <mcsm/data/options/modded/fabric_server_data_option.h>

mcsm::FabricServerDataOption::FabricServerDataOption() : ServerDataOption() {}

mcsm::FabricServerDataOption::FabricServerDataOption(const std::string& path) : ServerDataOption(path) {}

mcsm::FabricServerDataOption::~FabricServerDataOption(){}

mcsm::StringResult mcsm::FabricServerDataOption::getLoaderVersion() const {
    auto optExists = this->option->exists();
    if(!optExists) return tl::unexpected(optExists.error());
    if(!optExists.value()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::SERVER_DATA_NOT_CONFIGURED, {this->option->getPath()});
        return tl::unexpected(err);
    }

    auto valueRes = this->option->getValue("last_downloaded_loader_version");
    if(!valueRes) return tl::unexpected(valueRes.error());
    const nlohmann::json& value = valueRes.value();

    if(value == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_NOT_FOUND, {"\"last_downloaded_loader_version\"", this->option->getName()});
        return tl::unexpected(err);
    }
    if(!value.is_string()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_WRONG_TYPE, {"\"last_downloaded_loader_version\"", "string"});
        return tl::unexpected(err);
    }
    if(!mcsm::isSafeString(value)){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::UNSAFE_STRING, {value});
        return tl::unexpected(err);
    }

    return value;
}

mcsm::VoidResult mcsm::FabricServerDataOption::updateLoaderVersion(const std::string& version){
    if(!mcsm::isSafeString(version)){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::UNSAFE_STRING, {version});
        return tl::unexpected(err);
    }
    mcsm::VoidResult res1 = this->option->setValue("last_downloaded_loader_version", version);
    if(!res1) return res1;

    return this->option->save();
}

mcsm::StringResult mcsm::FabricServerDataOption::getInstallerVersion() const {
    auto optExists = this->option->exists();
    if(!optExists) return tl::unexpected(optExists.error());
    if(!optExists.value()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::SERVER_DATA_NOT_CONFIGURED, {this->option->getPath()});
        return tl::unexpected(err);
    }

    auto valueRes = this->option->getValue("last_downloaded_installer_version");
    if(!valueRes) return tl::unexpected(valueRes.error());
    const nlohmann::json& value = valueRes.value();

    if(value == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_NOT_FOUND, {"\"last_downloaded_installer_version\"", this->option->getName()});
        return tl::unexpected(err);
    }
    if(!value.is_string()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_WRONG_TYPE, {"\"last_downloaded_installer_version\"", "string"});
        return tl::unexpected(err);
    }
    if(!mcsm::isSafeString(value)){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::UNSAFE_STRING, {value});
        return tl::unexpected(err);
    }

    return value;
}

mcsm::VoidResult mcsm::FabricServerDataOption::updateInstallerVersion(const std::string& version){
    if(!mcsm::isSafeString(version)){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::UNSAFE_STRING, {version});
        return tl::unexpected(err);
    }
    mcsm::VoidResult res = this->option->setValue("last_downloaded_installer_version", version);
    if(!res) return res;

    return this->option->save();
}