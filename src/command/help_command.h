#ifndef __HELP_COMMAND_H__
#define __HELP_COMMAND_H__

#include "command.h"
#include "command_manager.h"
#include <iostream>

namespace mcsm {
    class HelpCommand : public mcsm::Command {
    public:
        HelpCommand(const std::string& name, const std::string& description);
        ~HelpCommand();
        void execute(const std::vector<std::string>& args) override;
    private:

    };
}

#endif