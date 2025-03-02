#ifndef __MCSM_SERVER_GROUP_MANAGER_H__
#define __MCSM_SERVER_GROUP_MANAGER_H__

#include <mcsm/data/options/running_sessions_option.h>
#include <mcsm/util/cli/screen_session.h>

namespace mcsm {
    // screen session name: (group).(servername)
    class ServerGroupManager {
    private:
        std::unique_ptr<mcsm::ServerGroupLoader> group;
    public:
        ServerGroupManager(std::unique_ptr<mcsm::ServerGroupLoader> group);
        ~ServerGroupManager();

        mcsm::Result start(); // starts all the servers in the group
        mcsm::Result start(const std::string& serverPath); // searches a registered server 'serverName' on the group and starts it

        mcsm::Result stop();
        mcsm::Result stop(const std::string& serverPath);

        int getRunningSessions() const;
        std::vector<const mcsm::ServerConfigLoader*> getRunningServers() const;

        inline mcsm::ServerGroupLoader* getGroupLoader() const { return this->group.get(); }
    };
}

#endif // __MCSM_SERVER_GROUP_MANAGER_H__