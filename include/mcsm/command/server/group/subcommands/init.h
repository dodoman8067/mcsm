#ifndef __MCSM_INIT_H__
#define __MCSM_INIT_H__

#include <mcsm/data/options/server_group_generator.h>

namespace mcsm {
    class GroupInitSubCommand {
    private:
        mcsm::ServerGroupGenerator* generator;
    public:
        GroupInitSubCommand(mcsm::ServerGroupGenerator* generator);
        ~GroupInitSubCommand();
        void execute(const std::vector<std::string>& args);
    };
}

#endif // __MCSM_INIT_H__