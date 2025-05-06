#ifndef __MCSM_REMOVE_H__
#define __MCSM_REMOVE_H__

#include <mcsm/data/options/server_group_loader.h>

namespace mcsm {
    class GroupRemoveSubCommand {
    private:
        mcsm::ServerGroupLoader* loader;
        std::unordered_map<std::string, std::vector<std::string>> getServerNameToPathMap(mcsm::ServerGroupLoader* loader) const;
    public:
        GroupRemoveSubCommand(mcsm::ServerGroupLoader* loader);
        ~GroupRemoveSubCommand() = default;
        void execute(const std::vector<std::string>& args);
    };
}

#endif // __MCSM_REMOVE_H__