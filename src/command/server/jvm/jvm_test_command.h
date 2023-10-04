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

#ifndef __MCSM_JVM_TEST_COMMAND_H__
#define __MCSM_JVM_TEST_COMMAND_H__

#include "../../base/command.h"
#include "../../../data/options/jvm_option.h"
#include "../../../util/cli_utils.h"

namespace mcsm {
    class JvmTestCommand : public mcsm::Command {
    private:
        mcsm::SearchTarget getSearchTarget(const std::vector<std::string>& args);
        std::unique_ptr<mcsm::JvmOption> searchOption(const mcsm::SearchTarget& target, const std::string& name);
        std::string getProfileName(const std::vector<std::string>& args) const;
        inline void performTest(std::unique_ptr<mcsm::JvmOption> option);
    public:
        JvmTestCommand(const std::string& name, const std::string& description);
        ~JvmTestCommand();
        void execute(const std::vector<std::string>& args) override;
    };
}

#endif // __MCSM_JVM_TEST_COMMAND_H__