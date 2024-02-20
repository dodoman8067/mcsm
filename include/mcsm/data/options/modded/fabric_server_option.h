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
        std::string path;
    public:
        FabricServerOption();
        FabricServerOption(const std::string& path);
        FabricServerOption(const std::string& version, const std::string& path);
        FabricServerOption(const std::string& version, std::shared_ptr<mcsm::Server> server);
        FabricServerOption(const std::string& version, std::shared_ptr<mcsm::Server> server, const std::string& path);
        ~FabricServerOption();

        bool exists();

        mcsm::Result create(const std::string& name, mcsm::JvmOption& defaultOption);

        mcsm::Result start();
        mcsm::Result start(std::unique_ptr<mcsm::JvmOption> option);

        std::string getServerJarBuild() const;
        mcsm::Result setServerJarBuild(const std::string& build);

        std::string getLoaderVersion() const;
        mcsm::Result setLoaderVersion(const std::string& version);

        std::string getInstallerVersion() const;
        mcsm::Result setInstallerVersion(const std::string& version);

        std::string getServerName() const;
        mcsm::Result setServerName(const std::string& name);

        std::string getServerVersion() const;
        mcsm::Result setServerVersion(const std::string& version);

        std::unique_ptr<mcsm::JvmOption> getDefaultOption() const;
        mcsm::Result setDefaultOption(std::unique_ptr<mcsm::JvmOption> jvmOption);

        std::string getServerType() const;

        std::string getServerJarFile() const;
        mcsm::Result setServerJarFile(const std::string& name);
        
        std::shared_ptr<mcsm::Server> getServer() const;
    };
}

#endif // __MCSM_FABRIC_SERVER_OPTION_H__