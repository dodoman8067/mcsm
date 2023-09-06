#ifndef __GET_SERVER_COMMAND_H__
#define __GET_SERVER_COMMAND_H__

#include "command.h"
#include <iostream>

namespace mcsm {
    class GetServerCommand : public mcsm::Command {
    private:
        
    public:
        GetServerCommand(const std::string& name, const std::string& description);
        ~GetServerCommand();
        void execute(const std::vector<std::string>& args) override;
    };
}


#endif