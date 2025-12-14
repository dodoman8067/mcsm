#ifndef __MCSM_JVM_COMMAND_H__
#define __MCSM_JVM_COMMAND_H__

#include <iostream>
#include <mcsm/command/base/command.h>

namespace mcsm {
    class JvmCommand : public mcsm::Command {
    private:
        std::string version;
    public:
        JvmCommand(const std::string& name, const std::string& description);
        ~JvmCommand();
        void execute(const std::vector<std::string>& args) override;
    };
}

#endif // __MCSM_JVM_COMMAND_H__