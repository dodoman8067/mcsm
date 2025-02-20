#ifndef __MCSM_LIST_H__
#define __MCSM_LIST_H__

#include <mcsm/server/server_group_manager.h>

namespace mcsm {
    class GroupListSubCommand {
    private:
        mcsm::ServerGroupManager* manager;
    public:
        GroupListSubCommand(mcsm::ServerGroupManager* manager);
        ~GroupListSubCommand() = default;
        void execute(const std::vector<std::string>& args);
    };
}

#endif // __MCSM_LIST_RUNNING_H__