#ifndef __MCSM_COMMAND_MANAGER_H__
#define __MCSM_COMMAND_MANAGER_H__

#include <memory>
#include <vector>
#include "curl_stuff.h"
#include "command.h"

namespace mcsm {
    class CommandManager {
    private:
        std::unique_ptr<std::vector<std::unique_ptr<mcsm::Command>>> commands;
        CURL* curl;
    public:
        CommandManager(CURL* curl);
        ~CommandManager();
        std::vector<std::unique_ptr<mcsm::Command>>& getCommands();
        void addCommand(std::unique_ptr<mcsm::Command> command);
    };
};

#endif