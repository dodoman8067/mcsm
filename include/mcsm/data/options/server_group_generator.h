#ifndef __MCSM_SERVER_GROUP_GENERATOR_H__
#define __MCSM_SERVER_GROUP_GENERATOR_H__

#include <mcsm/data/options/server_config_loader.h>

namespace mcsm {
    class ServerGroupGenerator {
    private:
        std::string name;
        std::string path;
        std::unique_ptr<mcsm::Option> handle;
    public:
        ServerGroupGenerator(const std::string& name, const std::string& path);
        ServerGroupGenerator(const mcsm::ServerGroupGenerator& other)
            : name(other.name),
              path(other.path),
              handle(other.handle ? std::make_unique<mcsm::Option>(*other.handle) : nullptr){}
        ~ServerGroupGenerator();

        // fails if group file already configured
        mcsm::Result generate();
        mcsm::Result generate(const std::vector<std::unique_ptr<mcsm::ServerConfigLoader>>& servers);

        const mcsm::Option* getHandle() const;
    };
}

#endif // __MCSM_SERVER_GROUP_GENERATOR_H__