#ifndef __MCSM_ADD_H__
#define __MCSM_ADD_H__

#include <mcsm/data/options/server_group_loader.h>

namespace mcsm {
    class GroupAddSubCommand {
    private:
        mcsm::ServerGroupLoader* loader;
    public:
        GroupAddSubCommand(mcsm::ServerGroupLoader* loader);
        ~GroupAddSubCommand() = default;
        void execute(const std::vector<std::string>& args);
    };
}

#endif // __MCSM_ADD_H__