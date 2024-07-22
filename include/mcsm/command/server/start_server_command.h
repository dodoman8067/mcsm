/*
Copyright (c) 2023 dodoman8067

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef __MCSM_START_SERVER_COMMAND_H__
#define __MCSM_START_SERVER_COMMAND_H__

#include <mcsm/command/base/command.h>
#include <mcsm/util/cli/ask_input.h>
#include <mcsm/data/options/server_option.h>
#include <mcsm/data/options/modded/fabric_server_option.h>

namespace mcsm {
    class StartServerCommand : public mcsm::Command {
    private:
        std::string getProfileName(const std::vector<std::string>& args) const;
        std::string getServerName(const std::vector<std::string>& args) const;
        std::string getServerVersion(const std::vector<std::string>& args) const;
        std::string getServerType(const std::vector<std::string>& args) const;
        mcsm::SearchTarget getSearchTarget(const std::vector<std::string>& args);
        std::unique_ptr<mcsm::JvmOption> searchOption(const std::vector<std::string>& args);
        std::unique_ptr<mcsm::JvmOption> searchOption(const mcsm::SearchTarget& target, const std::string& name);
        void detectServer(const std::vector<std::string>& args);
        inline bool isConfigured();
    public:
        StartServerCommand(const std::string& name, const std::string& description);
        ~StartServerCommand();
        void execute(const std::vector<std::string>& args) override;
    };
}


#endif // __MCSM_START_SERVER_COMMAND_H__