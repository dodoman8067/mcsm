#ifndef __MCSM_SERVER_OPTION_H__
#define __MCSM_SERVER_OPTION_H__

#include <mcsm/data/option.h>
#include <mcsm/server/server.h>
#include <memory>

namespace mcsm {
    class ServerOption {
    private:
        std::unique_ptr<mcsm::Server> server;
        std::string version;
    public:
        ServerOption(const std::string& version);
        ~ServerOption();
        void create(const std::string& name, mcsm::JvmOption& defaultOption, std::unique_ptr<mcsm::Server> server);
        void start();
        void start(std::unique_ptr<mcsm::JvmOption> option);
        std::string getFileName() const;
        std::string getServerName() const;
        std::string getServerVersion() const;
        std::unique_ptr<mcsm::Server> getServer() const;
        std::unique_ptr<mcsm::JvmOption> getDefaultOption() const;
    };
}

#endif // __MCSM_SERVER_OPTION_H__