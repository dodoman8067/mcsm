#ifndef __MCSM_ATTACH_H__
#define __MCSM_ATTACH_H__

#include <mcsm/command/base/command.h>
#include <mcsm/util/cli/screen_session.h>
#include <mcsm/data/options/server_group_loader.h>

namespace mcsm {
    class GroupAttachSubCommand {
    private:
        mcsm::ServerGroupLoader* loader;
    public:
        GroupAttachSubCommand(mcsm::ServerGroupLoader* loader);
        ~GroupAttachSubCommand();
        void execute(const std::vector<std::string>& args);
    };
}


#endif // __MCSM_ATTACH_H__