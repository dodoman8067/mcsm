#ifndef __GENERATE_SERVER_COMMAND_H__
#define __GENERATE_SERVER_COMMAND_H__

#include "command.h"
#include "../util/ask_input.h"
#include <iostream>
#include <limits>

namespace mcsm {
    class GenerateServerCommand : public Command {
    private:
    public:
        GenerateServerCommand(const std::string& name, const std::string& description);
        ~GenerateServerCommand();
        void execute(const std::vector<std::string>& args) override;
        void stuff(std::string& value);
    };
}

#endif