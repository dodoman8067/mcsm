#ifndef __MCSM_SERVER_TYPE_H__
#define __MCSM_SERVER_TYPE_H__

#include <string>

namespace mcsm {
    
    enum ServerType {
        VANILLA,
        BUKKIT,
        SPONGE_VANILLA,
        BUNGEECORD,
        VELOCITY,
        FORGE,
        FABRIC,
        UNKNOWN
    };
    
    enum BukkitServerType {
        CRAFTBUKKIT,
        SPIGOT,
        PAPER,
        PURPUR,
        PUFFERFISH,
        FOLIA,
        UNKNOWN
    };

    mcsm::ServerType getServerType(const std::string& name);
    std::string getServerType(const mcsm::ServerType& type);
}

#endif