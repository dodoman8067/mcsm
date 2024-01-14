#ifndef __MCSM_BUKKIT_PLUGIN_H__
#define __MCSM_BUKKIT_PLUGIN_H__

#include <mcsm/http/get.h>
#include <mcsm/data/options/server_option.h>

namespace mcsm {
    enum PluginRepo {
        BUKKITDEV,
        SPIGET,
        HANGAR
    };

    class BukkitPlugin {
    private:
        std::string name;

        bool found;
        int result;

        std::string nativeVersion;
        std::vector<std::string> supportedVersions;

        std::string downloads;
        std::string author;

        std::string createdAt;
        mcsm::PluginRepo repo;

        void init();
        std::string spigetGetCreatedAt();
        std::string spigetGetDownloads();
        std::string spigetGetNativeVersion();
        std::vector<std::string> spigetGetSupportedVersions();
        std::string spigetGetAuthor();
    public:
        BukkitPlugin(const std::string& name, const mcsm::PluginRepo& repo);
        BukkitPlugin(const std::string& name, const mcsm::PluginRepo& repo, const int& result);
        ~BukkitPlugin();

        bool isFound() const;
        int getResult() const;

        std::string getName() const;
        std::string getAuthor() const;

        std::string getNativeVersion() const;
        std::vector<std::string> getSupportedVersions() const;

        std::string getDownloads() const;
        
        std::string getCreateAt() const;
        mcsm::PluginRepo getRepo() const;
    };
}

#endif // __MCSM_BUKKIT_PLUGIN_H__