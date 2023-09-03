#ifndef __TEST__COMMAND_H__
#define __TEST__COMMAND_H__

#include "command.h"
#include <iostream>

namespace mcsm {
    class TestCommand : public mcsm::Command {
    public:
        TestCommand(const std::string& name, const std::string& description);
        ~TestCommand();
        void execute(const std::vector<std::string>& args) override;
    };
}

#endif