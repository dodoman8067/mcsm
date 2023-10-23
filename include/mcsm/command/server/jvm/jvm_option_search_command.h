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

#ifndef __MCSM_JVM_OPTION_SEARCH_COMMAND_H__
#define __MCSM_JVM_OPTION_SEARCH_COMMAND_H__

#include <mcsm/command/base/command.h>
#include <mcsm/data/options/jvm_option.h>

namespace mcsm {
    class JvmOptionSearchCommand : public mcsm::Command {
    private:
        std::vector<std::unique_ptr<mcsm::JvmOption>> search(const mcsm::SearchTarget& target, const std::string& name);
        std::string getProfileName(const std::vector<std::string>& args) const;
        mcsm::SearchTarget getSearchTarget(const std::vector<std::string>& args);
    public:
        JvmOptionSearchCommand(const std::string& name, const std::string& description);
        ~JvmOptionSearchCommand();
        void execute(const std::vector<std::string>& args) override;
    };
}


#endif // __MCSM_JVM_OPTION_SEARCH_COMMAND_H__