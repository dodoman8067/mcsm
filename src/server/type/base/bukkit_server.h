#ifndef __MCSM_BUKKIT_SERVER_H__
#define __MCSM_BUKKIT_SERVER_H__

#include "../../server.h"

namespace mcsm {
    /**
     * @brief Represents a server that uses Bukkit API.
     */
    class BukkitServer : public mcsm::Server {
    public:
        BukkitServer();
        ~BukkitServer();
        /**
         * @brief Returns current server type.
         * 
         * @return mcsm::ServerType 
         */
        mcsm::ServerType getType() const override;
    };
}

#endif // __MCSM_BUKKIT_SERVER_H__