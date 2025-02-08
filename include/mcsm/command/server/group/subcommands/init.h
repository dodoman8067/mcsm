#ifndef __MCSM_INIT_H__
#define __MCSM_INIT_H__

#include <mcsm/command/base/command.h>
#include <mcsm/data/options/server_group_generator.h>

namespace mcsm {
    class GroupInitSubCommand {
    private:
        mcsm::ServerGroupGenerator* loader;
    public:
        GroupInitSubCommand(mcsm::ServerGroupGenerator* loader);
        ~GroupInitSubCommand();
        void execute(const std::vector<std::string>& args);
    };
}

#endif // __MCSM_INIT_H__