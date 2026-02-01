#ifndef __MCSM_SERVER_CONFIG_GENERATOR_H__
#define __MCSM_SERVER_CONFIG_GENERATOR_H__

#include <mcsm/data/options/server_data_option.h>
#include <mcsm/data/options/server_config_loader.h>

namespace mcsm {
    const int MIN_SINGLE_CONFIG_VERSION = 1;
    const int SINGLE_CONFIG_VERSION = 1;
    class ServerConfigGenerator {
    public:
        explicit ServerConfigGenerator(const std::string& path);
        ServerConfigGenerator(const mcsm::ServerConfigGenerator& other)
            : configPath(other.configPath),
              optionHandle(other.optionHandle ? std::make_unique<mcsm::TomlOption>(*other.optionHandle) : nullptr){}
        ~ServerConfigGenerator();

        /**
         * Called by `Server#configure` which is called by `Server#generate`.
         */
        mcsm::VoidResult generate(const std::string& version, mcsm::Server* server, mcsm::ServerDataOption* sDataOpt, const std::string& name, mcsm::JvmOption& defaultOption);
        
        /**
         * Called by `Server#configure` which is called by `Server#generate`.
         */
        mcsm::VoidResult generate(const std::string& version, mcsm::Server* server, mcsm::ServerDataOption* sDataOpt, const std::string& name, mcsm::JvmOption& defaultOption, const bool& update);
        
        /**
         * Called by `Server#configure` which is called by `Server#generate`.
         */
        mcsm::VoidResult generate(const std::string& version, mcsm::Server* server, mcsm::ServerDataOption* sDataOpt, const std::string& name, mcsm::JvmOption& defaultOption, const bool& update, const std::string& build);

        mcsm::VoidResult generate(const std::string& version, mcsm::Server* server, mcsm::ServerDataOption* sDataOpt, const std::string& name, mcsm::JvmOption& defaultOption, const bool& update, const std::string& build, const std::string& jarPath);

        mcsm::TomlOption* getHandle();

        std::string getPath() const;

        mcsm::BoolResult validatePath();
    private:
        std::string configPath;
        std::unique_ptr<mcsm::TomlOption> optionHandle;
    };
}

#endif // __MCSM_SERVER_CONFIG_GENERATOR_H__