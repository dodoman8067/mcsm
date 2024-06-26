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
        mcsm::Result create(const std::string& name, mcsm::JvmOption& defaultOption, const bool& update);

        mcsm::Result start();
        mcsm::Result start(mcsm::JvmOption& option);
        mcsm::Result start(mcsm::JvmOption& option, const std::string& path, const std::string& optionPath);

        mcsm::Result update();

        std::string getOptionPath() const;

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
        mcsm::Result setDefaultOption(mcsm::JvmOption& jvmOption);

        std::string getServerType() const;

        std::string getServerJarFile() const;
        mcsm::Result setServerJarFile(const std::string& name);

        bool doesAutoUpdate() const;
        mcsm::Result setAutoUpdate(const bool& update);
        
        std::shared_ptr<mcsm::Server> getServer() const;
    };
}

#endif // __MCSM_FABRIC_SERVER_OPTION_H__