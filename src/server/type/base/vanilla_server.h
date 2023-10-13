#ifndef __MCSM_VANILLA_SERVER_H__
#define __MCSM_VANILLA_SERVER_H__

#include "../../server.h"
#include "../downloadable.h"
#include "../../../data/global_option.h"
#include <map>

namespace mcsm {
    class VanillaServer : public mcsm::Server, public mcsm::Downloadable {
    private:
        std::unique_ptr<std::map<const std::string, const std::string>> versions;
        void init();
    public:
        VanillaServer();
        ~VanillaServer();
        std::unique_ptr<std::vector<std::string>> getAvailableVersions() override;
        void download(const std::string& version) override;
        void download(const std::string& version, const std::string& path) override;
        void download(const std::string& version, const std::string& path, const std::string& name) override;
        bool hasVersion(const std::string& version) override;
        mcsm::ServerType getType() const override;
    };
}


#endif // __MCSM_VANILLA_SERVER_H__