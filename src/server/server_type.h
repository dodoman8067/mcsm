#ifndef __MCSM_SERVER_TYPE_H__
#define __MCSM_SERVER_TYPE_H__

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
}

#endif