#ifndef __MCSM_SERVER_STARTER_H__
#define __MCSM_SERVER_STARTER_H__

#include <mcsm/data/options/server_config_loader.h>
#include <mcsm/data/options/server_data_option.h>

// I will also handle the multi-server starting feature here in the near future

namespace mcsm {
    class ServerStarter {
    public:
        explicit ServerStarter(mcsm::ServerConfigLoader* loader);
        ~ServerStarter();

        mcsm::Result startServer(mcsm::JvmOption& option, const std::string& path, const std::string& optionPath);
        mcsm::Result startServer(mcsm::JvmOption& option, const std::string& path, const std::string& optionPath, const std::string& groupOptionPath);
    private:
        mcsm::ServerConfigLoader* loader;
    };
}
#endif // __MCSM_SERVER_STARTER_H__