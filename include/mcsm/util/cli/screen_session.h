#ifndef __MCSM_SCREEN_SESSION_H__
#define __MCSM_SCREEN_SESSION_H__

#include <mcsm/util/cli/cli_utils.h>
#include <cstdio>

namespace mcsm {
    class ScreenSession {
    private:
        std::string name; // will create a session as {name}.mcsm
        std::string command; // will sh -c {command}
        std::string screenPath;
    public:
        ScreenSession(const std::string& name);
        ScreenSession(const std::string& name, const std::string& command);
        ~ScreenSession() = default;

        mcsm::VoidResult start();
        mcsm::VoidResult stop();
        mcsm::VoidResult attach();
        mcsm::VoidResult sendCommand(const std::string& str);

        bool isRunning() const; // only one screen session that ends with current name.mcsm can exist
        bool validateScreen() const;
        bool tryReconnect();

        inline std::string getCommand() const {
            return this->command;
        }

        inline std::string getName() const {
            return this->name;
        }
        
        inline std::string getFullSessionName() const {
            return this->name + ".mcsm";
        }
    };
}

#endif // __MCSM_SCREEN_SESSION_H__