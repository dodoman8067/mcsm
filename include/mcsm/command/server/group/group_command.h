#ifndef __MCSM_GROUP_COMMAND_H__
#define __MCSM_GROUP_COMMAND_H__

#include <iostream>
#include <mcsm/command/base/command.h>

namespace mcsm {
    class GroupCommand : public mcsm::Command {
    private:
        std::string version;
    public:
        GroupCommand(const std::string& name, const std::string& description);
        ~GroupCommand();
        void execute(const std::vector<std::string>& args) override;
    };
}

#endif // __MCSM_GROUP_COMMAND_H__