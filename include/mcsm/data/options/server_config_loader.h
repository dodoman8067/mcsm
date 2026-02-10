#ifndef __MCSM_SERVER_CONFIG_LOADER_H__
#define __MCSM_SERVER_CONFIG_LOADER_H__

#include "mcsm/util/cli/result.h"
#include <mcsm/data/toml_option.h>

namespace mcsm {
    class Server;
    class JvmOption;
    
    class ServerConfigLoader {
    private:
        toml::table configRoot;
        toml::table configHeader;

        toml::table rootMeta;
        toml::table rootJar;
        toml::table rootJvm;
        toml::table rootLaunch;
    public:
        explicit ServerConfigLoader(const std::string& path);
        ServerConfigLoader(const ServerConfigLoader& other)
            : configPath(other.configPath),
              optionHandle(other.optionHandle ? std::make_unique<mcsm::TomlOption>(*other.optionHandle) : nullptr),
              isLoaded(other.isLoaded){}
        ~ServerConfigLoader();

        mcsm::VoidResult loadConfig();

        template <typename T>
        inline tl::expected<T, mcsm::Error> get(const std::string& key) const{
            if(!this->isLoaded){
                mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::SERVER_DATA_ACCESSED_WITHOUT_LOAD, {});
                return tl::unexpected(err);
            }

            auto valueRes = this->optionHandle->getValue(key);
            if(!valueRes) return tl::unexpected(valueRes.error());
            auto value = valueRes.value();

            if(value == nullptr){
                mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_NOT_FOUND, {"\"" + key + "\"", this->optionHandle->getName()});
                return tl::unexpected(err);
            }

            return value->as<T>();
        }

        mcsm::StringResult getServerName() const;
        mcsm::VoidResult setServerName(const std::string& name);

        mcsm::StringResult getServerVersion() const;
        mcsm::VoidResult setServerVersion(const std::string& version);

        tl::expected<std::unique_ptr<mcsm::JvmOption>, mcsm::Error> getDefaultOption() const;
        mcsm::VoidResult setDefaultOption(mcsm::JvmOption& jvmOption);

        mcsm::StringResult getServerType() const;

        mcsm::StringResult getServerJar() const;
        mcsm::VoidResult setServerJar(const std::string& filePath);

        mcsm::StringResult getServerJarFile() const;
        //mcsm::VoidResult setServerJarFile(const std::string& name);

        mcsm::StringResult getServerJarPath() const;

        mcsm::StringResult getServerJarBuild() const;
        mcsm::VoidResult setServerJarBuild(const std::string& build);

        mcsm::BoolResult doesAutoUpdate() const;
        mcsm::VoidResult setAutoUpdate(const bool& update);

        mcsm::IntResult getConfigVersion() const;
        mcsm::VoidResult setConfigVersion(const int& ver);

        mcsm::TomlOption* getHandle() const;

        bool isFullyLoaded() const;

        tl::expected<mcsm::Server*, mcsm::Error> getServerInstance();
        
    private:
        std::string configPath;
        std::unique_ptr<mcsm::TomlOption> optionHandle;
        bool isLoaded;
    };
}

#endif // __MCSM_SERVER_CONFIG_LOADER_H__