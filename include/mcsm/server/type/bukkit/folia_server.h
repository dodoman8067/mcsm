#ifndef __MCSM_FOLIA_SERVER_H__
#define __MCSM_FOLIA_SERVER_H__

#include <mcsm/server/type/base/bukkit_server.h>
#include <mcsm/server/type/downloadable.h>
#include <mcsm/http/get.h>
#include <mcsm/http/download.h>
#include <mcsm/util/mc/mc_utils.h>

namespace mcsm {
    struct FoliaMetaData {
        std::vector<std::string> recommendedJavaFlags;
        std::string minJava;
        std::string channel; // alpha, stable etc..
        std::string supportStatus;
        std::string downloadUrl;
        std::string build;
    };
    class FoliaServer : public mcsm::BukkitServer, public mcsm::Downloadable, public std::enable_shared_from_this<FoliaServer> {
    private:
    public:
        FoliaServer();
        ~FoliaServer();

        mcsm::Result<mcsm::FoliaMetaData> getVersionData(const std::string& ver);
        mcsm::Result<mcsm::FoliaMetaData> getVersionData(const std::string& ver, const std::string& build);

        mcsm::IntResult getVersion(const std::string& ver) const;
        mcsm::IntResult getVersion(const std::string& ver, const std::string& build) const;

        std::vector<std::string> getAvailableVersions() override;

        std::string getSupportedVersions() const override;

        std::string getBasedServer() const override;

        std::string getWebSite() const override;
        
        std::string getGitHub() const override;

        mcsm::StringResult start(mcsm::ServerConfigLoader* loader, mcsm::JvmOption& option) override;
        mcsm::StringResult start(mcsm::ServerConfigLoader* loader, mcsm::JvmOption& option, const std::string& path, const std::string& optionPath) override;

        mcsm::VoidResult download(const std::string& version) override;
        mcsm::VoidResult download(const std::string& version, const std::string& path) override;
        mcsm::VoidResult download(const std::string& version, const std::string& path, const std::string& name) override;
        mcsm::VoidResult download(const std::string& version, const std::string& path, const std::string& name, const std::string& optionPath) override;

        mcsm::VoidResult obtainJarFile(const std::string& version, const std::string& path, const std::string& name, const std::string& optionPath) override;

        mcsm::BoolResult hasVersion(const std::string& version) const override;

        std::string getTypeAsString() const override;

        mcsm::VoidResult update();
        mcsm::VoidResult update(const std::string& optionPath);
        mcsm::VoidResult update(const std::string& path, const std::string& optionPath);

        mcsm::VoidResult generate(const std::string& name, mcsm::JvmOption& option, const std::string& path, const std::string& version, const bool& autoUpdate, const std::map<std::string, std::string>& extraValues) override;
    };
}


#endif // __MCSM_FOLIA_SERVER_H__