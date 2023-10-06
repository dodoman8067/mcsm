#ifndef __MCSM_SERVER_H__
#define __MCSM_SERVER_H__

#include "server_type.h"

namespace mcsm {
    /**
     * @brief Represents base of server.
     */
    class Server {
    public:
        /**
         * @brief Returns the base type of server.
         * @return mcsm::ServerType enum
         */
        virtual mcsm::ServerType getType() const = 0;
    };
}

#endif // __MCSM_SERVER_H__