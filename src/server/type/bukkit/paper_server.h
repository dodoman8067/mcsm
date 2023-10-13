#ifndef __MCSM_PAPER_SERVER_H__
#define __MCSM_PAPER_SERVER_H__

#include "../base/bukkit_server.h"
#include "../downloadable.h"
#include <curl/curl.h>

namespace mcsm {
    class PaperServer : public mcsm::BukkitServer, public mcsm::Downloadable {
    private:
        std::string getVersion(const std::string& ver) const;
    public:
        std::unique_ptr<std::vector<std::string>> getAvailableVersions() override;
        void download(const std::string& version) override;
        void download(const std::string& version, const std::string& path) override;
        void download(const std::string& version, const std::string& path, const std::string& name) override;
        bool hasVersion(const std::string& version) override;
    };
}


#endif // __MCSM_PAPER_SERVER_H__