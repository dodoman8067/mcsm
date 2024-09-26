#ifndef __MCSM_SERVER_STARTER_H__
#define __MCSM_SERVER_STARTER_H__

#include <mcsm/data/options/server_config_loader.h>
#include <mcsm/data/options/server_data_option.h>

namespace mcsm {
    class ServerStarter {
    public:
        explicit ServerStarter(ServerConfigLoader* loader);

        mcsm::Result startServer();
    private:
        ServerConfigLoader* configLoader;
    };
}
#endif // __MCSM_SERVER_STARTER_H__