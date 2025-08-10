#ifndef __MCSM_SERVER_GROUP_MANAGER_H__
#define __MCSM_SERVER_GROUP_MANAGER_H__

#include <mcsm/data/options/server_group_loader.h>
#include <mcsm/util/cli/screen_session.h>

namespace mcsm {
    // screen session name: (group).(servername)
    class ServerGroupManager {
    private:
        std::unique_ptr<mcsm::ServerGroupLoader> group;
    public:
        ServerGroupManager(std::unique_ptr<mcsm::ServerGroupLoader> group);
        ~ServerGroupManager();

        mcsm::VoidResult start(); // starts all the servers in the group
        mcsm::VoidResult start(const std::string& serverPath); // searches a registered server 'serverName' on the group and starts it

        mcsm::VoidResult stop();
        mcsm::VoidResult stop(const std::string& serverPath);

        mcsm::IntResult getRunningSessions() const;
        tl::expected<std::vector<const mcsm::ServerConfigLoader*>, mcsm::Error> getRunningServers() const;

        inline mcsm::ServerGroupLoader* getGroupLoader() const { return this->group.get(); }
    };
}

#endif // __MCSM_SERVER_GROUP_MANAGER_H__