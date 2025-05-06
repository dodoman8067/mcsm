#ifndef __MCSM_STOP_H__
#define __MCSM_STOP_H__

#include <mcsm/server/server_group_manager.h>

namespace mcsm {
    class GroupStopSubCommand {
    private:
        mcsm::ServerGroupManager* manager;
        mcsm::ServerGroupLoader* loader;
        std::unordered_map<std::string, std::vector<std::string>> getServerNameToPathMap(mcsm::ServerGroupLoader* loader) const;
        void stopWithPaths(const bool& strict, const std::vector<std::string>& serverArgs);
        void stopAll(const bool& strict);
    public:
        GroupStopSubCommand(mcsm::ServerGroupManager* manager);
        ~GroupStopSubCommand() = default;
        void execute(const std::vector<std::string>& args);
    };
}

#endif // __MCSM_STOP_H__