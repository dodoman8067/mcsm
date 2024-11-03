#ifndef __MCSM_CLEAR_SERVER_COMMAND_H__
#define __MCSM_CLEAR_SERVER_COMMAND_H__

#include <mcsm/command/base/command.h>
#include <mcsm/data/options/modded/fabric_server_data_option.h>
#include <mcsm/data/options/server_config_loader.h>

namespace mcsm {
    class ClearServerCommand : public mcsm::Command {
    private:
        inline bool isConfigured();
    public:
        ClearServerCommand(const std::string& name, const std::string& description);
        ~ClearServerCommand() = default;
        void execute(const std::vector<std::string>& args) override;
    };
}

#endif // __MCSM_CLEAR_SERVER_COMMAND_H__