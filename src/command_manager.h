#ifndef __MCSM_COMMAND_MANAGER_H__
#define __MCSM_COMMAND_MANAGER_H__

#include <memory>
#include <vector>
#include "curl_stuff.h"
#include "command.h"

namespace mcsm {
    class CommandManager {
    private:
        static std::unique_ptr<std::vector<std::unique_ptr<mcsm::Command>>> commands;
        CommandManager();
    public:
        ~CommandManager();
        static void init();
        static std::vector<std::unique_ptr<mcsm::Command>>& getCommands();
        static void addCommand(std::unique_ptr<mcsm::Command> command);
    };
};

#endif