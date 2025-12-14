#ifndef __MCSM_RUNNING_SESSIONS_OPTION_H__
#define __MCSM_RUNNING_SESSIONS_OPTION_H__

/*

#include <mcsm/data/options/server_group_loader.h>

namespace mcsm {
    // screen session name: (group).(servername)
    // not 100% sure if this class will be used.
    struct RunningGroup {
        RunningGroup& operator=(RunningGroup&& other) noexcept {
            if(this != &other){
                group = std::move(other.group);
                running = std::move(other.running);
            }
            return *this;
        }

        std::unique_ptr<mcsm::ServerGroupLoader> group;
        std::vector<std::unique_ptr<mcsm::ServerConfigLoader>> running;
    };

    class RunningSessionsOption {
    private:
        std::unique_ptr<mcsm::GlobalOption> handle;
        std::vector<std::unique_ptr<mcsm::RunningGroup>> runningGroups;
    public:
        RunningSessionsOption();
        ~RunningSessionsOption();

        mcsm::VoidResult load();
        mcsm::VoidResult save();

        std::vector<const mcsm::RunningGroup*> getRunningGroups() const;
        tl::expected<std::vector<const mcsm::ServerConfigLoader*>, mcsm::Error> getRunningServersOfGroup(const std::string& groupName) const;

        mcsm::VoidResult addRunningGroup(std::unique_ptr<mcsm::RunningGroup> group);
        mcsm::VoidResult addRunningServer(const std::string& groupName, const mcsm::ServerConfigLoader* server);

        mcsm::VoidResult removeRunningGroup(std::unique_ptr<mcsm::RunningGroup> group);
        mcsm::VoidResult removeRunningServer(const std::string& groupName, const mcsm::ServerConfigLoader* server);
    };
}
*/

#endif // __MCSM_RUNNING_SESSIONS_OPTION_H__