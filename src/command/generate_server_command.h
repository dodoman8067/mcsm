#ifndef __GENERATE_SERVER_COMMAND_H__
#define __GENERATE_SERVER_COMMAND_H__

#include "command.h"

namespace mcsm {
    class GenerateServerCommand : public Command {
    private:
    public:
        GenerateServerCommand(const std::string& name, const std::string& description);
        ~GenerateServerCommand();
        void execute(const std::vector<std::string>& args) override;
    };
}

#endif