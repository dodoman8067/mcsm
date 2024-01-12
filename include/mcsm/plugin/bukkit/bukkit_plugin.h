#ifndef __MCSM_BUKKIT_PLUGIN_H__
#define __MCSM_BUKKIT_PLUGIN_H__

#include <mcsm/http/get.h>
#include <vector>

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

        std::string nativeVersion;
        std::vector<std::string> supportedVersions;

        std::string downloads;
        std::string author;

        std::string createdAt;

        void init(const mcsm::PluginRepo& repo);
    public:
        BukkitPlugin(const std::string& name);
        BukkitPlugin(const std::string& name, const mcsm::PluginRepo& repo);
        ~BukkitPlugin();

        bool isFound() const;

        std::string getName() const;
        std::string getAuthor() const;

        std::string getNativeVersion() const;
        std::vector<std::string> getSupportedVersions() const;

        std::string getDownloads() const;
        
        std::string getCreateAt() const;
    };
}

#endif // __MCSM_BUKKIT_PLUGIN_H__