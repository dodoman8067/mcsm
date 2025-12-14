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

#include <mcsm/data/option.h>
#include <mcsm/jvm/java_detection.h>
#include <mcsm/util/string_utils.h>
#include <memory>

namespace mcsm {
    enum class SearchTarget {
        GLOBAL,
        CURRENT,
        ALL
    };

    class JvmOption {
    private:
        std::unique_ptr<mcsm::Option> option;
        std::string name;
        bool initialized = false;
    public:
        JvmOption(const std::string& path, const std::string& name);
        JvmOption(std::unique_ptr<mcsm::Option> option); 
        ~JvmOption();

        mcsm::VoidResult init();

        mcsm::VoidResult create();
        mcsm::VoidResult create(const std::string& jvmPath);
        mcsm::VoidResult create(const std::string& jvmPath, const std::vector<std::string>& jvmOptions);
        mcsm::VoidResult create(const std::string& jvmPath, const std::vector<std::string>& jvmOptions, const std::vector<std::string>& serverOptions);

        mcsm::VoidResult reset();

        mcsm::BoolResult exists() const;

        mcsm::StringResult getJvmPath() const;
        mcsm::VoidResult setJvmPath(const std::string& jvmPath);

        tl::expected<std::vector<std::string>, mcsm::Error> getJvmArguments() const;
        mcsm::VoidResult setJvmArguments(const std::vector<std::string>& jvmArgs);

        tl::expected<std::vector<std::string>, mcsm::Error> getServerArguments() const;
        mcsm::VoidResult setServerArguments(const std::vector<std::string>& serverArgs);

        std::string getProfileName() const;

        mcsm::StringResult getProfilePath() const;
    };

    mcsm::Result<std::unique_ptr<mcsm::Option>> jvmProfileFromSearchTarget(const std::string& name, const mcsm::SearchTarget& target, const std::string& workingPath);
    
}

#endif