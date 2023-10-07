#ifndef __MCSM_VANILLA_SERVER_H__
#define __MCSM_VANILLA_SERVER_H__

#include "../../server.h"
#include "../downloadable.h"
#include <map>

namespace mcsm {
    class VanillaServer : public mcsm::Server, public mcsm::Downloadable {
    private:
        std::unique_ptr<std::map<const std::string, const std::string>> verisons;
    public:
        VanillaServer();
        ~VanillaServer();
        std::unique_ptr<std::vector<std::string>> getAvailableVersions() override;
        void download(const std::string& version) override;
        void download(const std::string& version, const std::string& path) override;
        void download(const std::string& version, const std::string& path, const std::string& name) override;
        bool hasVersion(const std::string& version) override;
    };
}


#endif // __MCSM_VANILLA_SERVER_H__