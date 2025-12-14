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
        ServerGroupGenerator(const std::string& name, const std::string& path) : name(name), path(path), handle(nullptr){}
        ServerGroupGenerator(const mcsm::ServerGroupGenerator& other)
            : name(other.name),
              path(other.path),
              handle(other.handle ? std::make_unique<mcsm::Option>(*other.handle) : nullptr){}
        ~ServerGroupGenerator() = default;

        // fails if group file already configured
        mcsm::VoidResult generate(const std::string& mode);
        mcsm::VoidResult generate(const std::string& mode, const std::vector<const mcsm::ServerConfigLoader*> servers);

        inline std::string getName() const { return this->name; } // do not call getHandle().getName(); does not return server group's name

        const mcsm::Option* getHandle() const;
    };
}

#endif // __MCSM_SERVER_GROUP_GENERATOR_H__