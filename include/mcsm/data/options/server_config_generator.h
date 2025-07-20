#ifndef __MCSM_SERVER_CONFIG_GENERATOR_H__
#define __MCSM_SERVER_CONFIG_GENERATOR_H__

#include <mcsm/data/options/server_data_option.h>
#include <mcsm/server/server.h>

namespace mcsm {
    class ServerConfigGenerator {
    public:
        explicit ServerConfigGenerator(const std::string& path);
        ServerConfigGenerator(const mcsm::ServerConfigGenerator& other)
            : configPath(other.configPath),
              optionHandle(other.optionHandle ? std::make_unique<mcsm::Option>(*other.optionHandle) : nullptr){}
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

        mcsm::Option* getHandle();

        std::string getPath() const;

        mcsm::BoolResult validatePath();
    private:
        std::string configPath;
        std::unique_ptr<mcsm::Option> optionHandle;
    };
}

#endif // __MCSM_SERVER_CONFIG_GENERATOR_H__