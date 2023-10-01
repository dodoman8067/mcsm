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

#ifndef __MCSM_JVM_OPTION_H__
#define __MCSM_JVM_OPTION_H__

#include "../global_option.h"
#include "../option.h"
#include "../../jvm/java_detection.h"
#include "../../util/string_utils.h"
#include <memory>

namespace mcsm {
    enum SearchTarget {
        GLOBAL,
        CURRENT,
        ALL
    };

    class JvmOption {
    private:
        std::unique_ptr<mcsm::Configurable> option;
        std::string name;
    public:
        JvmOption(const std::string& name);
        JvmOption(const std::string& name, const SearchTarget& target);
        ~JvmOption();
        void create();
        void create(const std::string& jvmPath, const SearchTarget& target);
        void create(const std::string& jvmPath, const std::vector<std::string>& jvmOptions, const SearchTarget& target);
        void create(const std::string& jvmPath, const std::vector<std::string>& jvmOptions, const std::vector<std::string>& serverOptions, const SearchTarget& target);
        void reset();
        bool exists();
        std::string getJvmPath();
        std::vector<std::string> getJvmArguments();
        std::vector<std::string> getServerArguments();
        std::string getProfileName() const;
        std::string getProfilePath() const;
    };
}

#endif