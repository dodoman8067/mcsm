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

#ifndef __MCSM_OPTION_H__
#define __MCSM_OPTION_H__

#include <nlohmann/json.hpp>
#include <filesystem>
#include <fstream>
#include <mcsm/util/cli/logging.h>
#include <mcsm/data/configurable.h>
#include <mcsm/util/string_utils.h>
#include <mcsm/util/cli/cli_utils.h>

namespace mcsm {
    class Option {
    private:
        std::string path;
        std::string name;
        mutable nlohmann::json data; 

        mcsm::BoolResult createDirectories(std::string const &dirName, std::error_code &err) const;
    public:
        /**
         * @brief No mcsm::Result needed
        */
        Option(const std::string& path, const std::string& name);
        /**
         * @brief No mcsm::Result needed
        */
        ~Option();

        mcsm::VoidResult create();

        mcsm::VoidResult load();

        mcsm::VoidResult load(const bool& advancedParse);

        nlohmann::json& getData() const;

        /**
         * @brief No mcsm::Result needed
        */
        std::string getPath() const;

        /**
         * @brief No mcsm::Result needed
        */
        std::string getName() const;

        tl::expected<nlohmann::json, mcsm::Error> getValue(const std::string& key) const;
        mcsm::VoidResult setValue(const std::string& key, const nlohmann::json& value);
        mcsm::BoolResult hasValue(const std::string& key) const;

        mcsm::BoolResult exists() const; // todo edit configurable or remove it too

        /**
         * @brief No mcsm::Result needed
        */
        bool isGlobal() const;

        mcsm::VoidResult save();

        mcsm::VoidResult save(const nlohmann::json& json);
        
        mcsm::VoidResult reset();
    };
}

#endif