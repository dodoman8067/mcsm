#ifndef __MCSM_SERVER_CONFIG_GENERATOR_H__
#define __MCSM_SERVER_CONFIG_GENERATOR_H__

#include <mcsm/data/options/server_data_option.h>
#include <mcsm/server/server.h>

namespace mcsm {
    class ServerConfigGenerator {
    public:
        explicit ServerConfigGenerator(const std::string& path);
        ~ServerConfigGenerator();

        mcsm::Result generate(std::shared_ptr<mcsm::Server> server);

        std::unique_ptr<mcsm::Option>& getHandle();

        mcsm::Result validatePath();

    private:
        std::string configPath;
        std::unique_ptr<mcsm::Option> optionHandle;
    };
}

#endif // __MCSM_SERVER_CONFIG_GENERATOR_H__