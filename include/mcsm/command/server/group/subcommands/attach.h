#ifndef __MCSM_ATTACH_H__
#define __MCSM_ATTACH_H__

#include <mcsm/command/base/command.h>
#include <mcsm/util/cli/screen_session.h>
#include <mcsm/server/server_group_manager.h>

namespace mcsm {
    class GroupAttachSubCommand {
    private:
        mcsm::ServerGroupManager* manager;
        mcsm::ServerGroupLoader* loader;
    public:
        GroupAttachSubCommand(mcsm::ServerGroupManager* manager);
        ~GroupAttachSubCommand() = default;
        void execute(const std::vector<std::string>& args);
    };
}


#endif // __MCSM_ATTACH_H__