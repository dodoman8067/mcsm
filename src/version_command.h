#ifndef __VERSION_COMMAND_H__
#define __VERSION_COMMAND_H__

#include <iostream>
#include "command.h"
#include "curl_stuff.h"

namespace mcsm {
    class VersionCommand : public mcsm::Command {
    private:
        std::string version;
    public:
        VersionCommand(const std::string& name, const std::string& description, const std::string& version);
        ~VersionCommand();
        void execute(const std::vector<std::string>& args) override;
    };
}


#endif