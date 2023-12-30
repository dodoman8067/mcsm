#ifndef __MCSM_FABRIC_SERVER_OPTION_H__
#define __MCSM_FABRIC_SERVER_OPTION_H__

#include <mcsm/data/options/server_option.h>

namespace mcsm {
    class FabricServerOption : public mcsm::ServerOption {
    public:
        FabricServerOption();
        FabricServerOption(const std::string& version);
        FabricServerOption(const std::string& version, std::shared_ptr<mcsm::Server> server);
        ~FabricServerOption();
        void create(const std::string& name, mcsm::JvmOption& defaultOption) override;
        void start() override;
        void start(std::unique_ptr<mcsm::JvmOption> option) override;
        std::string getServerJarBuild() const override;
        void setServerJarBuild(const std::string& build) override;
        std::string getLoaderVersion() const;
        void setLoaderVersion(const std::string& version);
        std::string getInstallerVersion() const;
        void setInstallerVersion(const std::string& version);
    };
}

#endif // __MCSM_FABRIC_SERVER_OPTION_H__