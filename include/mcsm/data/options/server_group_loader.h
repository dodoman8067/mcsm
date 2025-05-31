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

        mcsm::VoidResult removeDuplicateServers(mcsm::Option* handle);
    public: 
        ServerGroupLoader(const std::string& path);
        ~ServerGroupLoader();

        mcsm::VoidResult load();
        mcsm::VoidResult save();

        const mcsm::Option* getHandle() const;

        bool isLoaded() const; 

        mcsm::StringResult getName() const;
        mcsm::VoidResult setName(const std::string& name);

        mcsm::StringResult getMode() const; // "default" or "screen"
        mcsm::VoidResult setMode(const std::string& mode); // fails if `mode` is not one of "default" or "screen"

        std::vector<const mcsm::ServerConfigLoader*> getServers() const;
        mcsm::VoidResult setServers(const std::vector<mcsm::ServerConfigLoader*>& servers);

        mcsm::VoidResult addServer(const std::string& path);
        mcsm::VoidResult addServer(mcsm::ServerConfigLoader* server);
        mcsm::VoidResult addServer(std::unique_ptr<mcsm::ServerConfigLoader> server);
        mcsm::VoidResult addServer(const std::vector<std::unique_ptr<mcsm::ServerConfigLoader>>& servers);

        mcsm::VoidResult removeServer(const std::string& path);
        mcsm::VoidResult removeServer(mcsm::ServerConfigLoader* server);
        mcsm::VoidResult removeServer(const std::vector<mcsm::ServerConfigLoader*>& servers);
    };
}

#endif // __MCSM_SERVER_GROUP_LOADER_H__