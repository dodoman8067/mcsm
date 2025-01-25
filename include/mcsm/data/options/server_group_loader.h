#ifndef __MCSM_SERVER_GROUP_LOADER_H__
#define __MCSM_SERVER_GROUP_LOADER_H__

#include <mcsm/data/options/server_config_loader.h>

namespace mcsm {
    class ServerGroupLoader {
    private:
        std::string path, mode;
        std::unique_ptr<mcsm::Option> handle;
        std::vector<std::unique_ptr<mcsm::ServerConfigLoader>> loaders;
        bool loaded;
    public: 
        ServerGroupLoader(const std::string& path);
        ~ServerGroupLoader();

        mcsm::Result load();
        mcsm::Result save();

        const mcsm::Option* getHandle() const;

        bool isLoaded() const; 

        std::string getName() const;
        mcsm::Result setName(const std::string& name);

        std::string getMode() const; // "default" or "screen"
        mcsm::Result setMode(const std::string& mode); // fails if `mode` is not one of "default" or "screen"

        std::vector<const mcsm::ServerConfigLoader*> getServers() const;
        mcsm::Result setServers(const std::vector<mcsm::ServerConfigLoader*>& servers);

        mcsm::Result addServer(const std::string& path);
        mcsm::Result addServer(mcsm::ServerConfigLoader* server);
        mcsm::Result addServer(std::unique_ptr<mcsm::ServerConfigLoader> server);
        mcsm::Result addServer(const std::vector<std::unique_ptr<mcsm::ServerConfigLoader>>& servers);

        mcsm::Result removeServer(const std::string& path);
        mcsm::Result removeServer(mcsm::ServerConfigLoader* server);
        mcsm::Result removeServer(const std::vector<mcsm::ServerConfigLoader*>& servers);
    };
}

#endif // __MCSM_SERVER_GROUP_LOADER_H__