#ifndef __MCSM_START_H__
#define __MCSM_START_H__

#include <mcsm/server/server_group_manager.h>

namespace mcsm {
    class GroupStartSubCommand {
    private:
        mcsm::ServerGroupManager* manager;
        mcsm::ServerGroupLoader* loader;
        std::unordered_map<std::string, std::vector<std::string>> getServerNameToPathMap(mcsm::ServerGroupLoader* loader) const;
        void startWithPaths(const bool& strict, const std::vector<std::string>& serverArgs);
        void startAll(const bool& strict, const std::vector<std::string>& serverArgs);
    public:
        GroupStartSubCommand(mcsm::ServerGroupManager* manager);
        ~GroupStartSubCommand() = default;
        void execute(const std::vector<std::string>& args);
    };
}

#endif // __MCSM_START_H__