#ifndef __MCSM_FABRIC_SERVER_OPTION_H__
#define __MCSM_FABRIC_SERVER_OPTION_H__

#include <mcsm/data/options/server_option.h>
#include <mcsm/server/type/modded/fabric_server.h>
#include <mcsm/data/options/modded/fabric_server_data_option.h>

namespace mcsm {
    class FabricServerOption {
    private:
        std::shared_ptr<mcsm::Server> server;
        std::string version;
    public:
        FabricServerOption();
        FabricServerOption(const std::string& version);
        FabricServerOption(const std::string& version, std::shared_ptr<mcsm::Server> server);
        ~FabricServerOption();

        bool exists();

        void create(const std::string& name, mcsm::JvmOption& defaultOption);

        void start();
        void start(std::unique_ptr<mcsm::JvmOption> option);

        std::string getServerJarBuild() const;
        void setServerJarBuild(const std::string& build);

        std::string getLoaderVersion() const;
        void setLoaderVersion(const std::string& version);

        std::string getInstallerVersion() const;
        void setInstallerVersion(const std::string& version);

        std::string getServerName() const;
        void setServerName(const std::string& name);

        std::string getServerVersion() const;
        void setServerVersion(const std::string& version);

        std::unique_ptr<mcsm::JvmOption> getDefaultOption() const;
        void setDefaultOption(std::unique_ptr<mcsm::JvmOption> jvmOption);

        std::string getServerType() const;

        std::string getServerJarFile() const;
        void setServerJarFile(const std::string& name);
        
        std::shared_ptr<mcsm::Server> getServer() const;
    };
}

#endif // __MCSM_FABRIC_SERVER_OPTION_H__