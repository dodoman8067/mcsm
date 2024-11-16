#ifndef __MCSM_RUNNING_SESSIONS_OPTION_H__
#define __MCSM_RUNNING_SESSIONS_OPTION_H__

#include <mcsm/data/options/server_group_loader.h>

namespace mcsm {
    class RunningSessionsOption {
    private:
        std::unique_ptr<mcsm::GlobalOption> handle;
        std::vector<std::unique_ptr<mcsm::ServerGroupLoader>> runningGroups;
    };
}

#endif // __MCSM_RUNNING_SESSIONS_OPTION_H__