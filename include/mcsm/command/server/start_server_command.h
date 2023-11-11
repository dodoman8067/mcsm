#ifndef __MCSM_START_SERVER_COMMAND_H__
#define __MCSM_START_SERVER_COMMAND_H__

#include <mcsm/command/base/command.h>
#include <mcsm/util/cli/ask_input.h>
#include <mcsm/data/options/server_option.h>
#include <mcsm/server/type/bukkit/paper_server.h>

namespace mcsm {
    class StartServerCommand : public mcsm::Command {
    private:
        std::string getProfileName(const std::vector<std::string>& args) const;
        std::string getServerName(const std::vector<std::string>& args) const;
        std::string getServerVersion(const std::vector<std::string>& args) const;
        std::string getServerType(const std::vector<std::string>& args) const;
        mcsm::SearchTarget getSearchTarget(const std::vector<std::string>& args);
        std::unique_ptr<mcsm::JvmOption> searchOption(const std::vector<std::string>& args);
        std::unique_ptr<mcsm::JvmOption> searchOption(const mcsm::SearchTarget& target, const std::string& name);
        void detectServer(const std::vector<std::string>& args);
        inline bool isConfigured();
    public:
        StartServerCommand(const std::string& name, const std::string& description);
        ~StartServerCommand();
        void execute(const std::vector<std::string>& args) override;
        void askServer(std::string& value);
    };
}


#endif // __MCSM_START_SERVER_COMMAND_H__