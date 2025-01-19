#ifndef __MCSM_RUNNING_SESSIONS_OPTION_H__
#define __MCSM_RUNNING_SESSIONS_OPTION_H__

#include <mcsm/data/options/server_group_loader.h>

namespace mcsm {
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

        mcsm::Result load();
        mcsm::Result save();

        std::vector<const mcsm::RunningGroup*> getRunningGroups() const;
        std::vector<const mcsm::ServerConfigLoader*> getRunningServersOfGroup(const std::string& groupName) const;

        mcsm::Result addRunningGroup(std::unique_ptr<mcsm::RunningGroup> group);
        mcsm::Result addRunningServer(const std::string& groupName, const mcsm::ServerConfigLoader* server);

        mcsm::Result removeRunningGroup(std::unique_ptr<mcsm::RunningGroup> group);
        mcsm::Result removeRunningServer(const std::string& groupName, const mcsm::ServerConfigLoader* server);
    };
}

#endif // __MCSM_RUNNING_SESSIONS_OPTION_H__