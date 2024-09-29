#ifndef __MCSM_SERVER_STARTER_H__
#define __MCSM_SERVER_STARTER_H__

#include <mcsm/data/options/server_config_loader.h>
#include <mcsm/data/options/server_data_option.h>

namespace mcsm {
    class ServerStarter {
    public:
        explicit ServerStarter(mcsm::ServerConfigLoader* loader);
        ~ServerStarter();

        mcsm::Result startServer(mcsm::JvmOption& option, const std::string& path, const std::string& optionPath);
    private:
        mcsm::ServerConfigLoader* loader;
    };
}
#endif // __MCSM_SERVER_STARTER_H__