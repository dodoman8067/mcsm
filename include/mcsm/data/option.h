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
    class Option : public mcsm::Configurable {
    private:
        std::string path;
        std::string name;

        bool createDirectories(std::string const &dirName, std::error_code &err) const;
    public:
        /**
         * @brief No mcsm::Result needed
        */
        Option(const std::string& path, const std::string& name);
        /**
         * @brief No mcsm::Result needed
        */
        ~Option();

        nlohmann::json load() const;

        nlohmann::json load(const bool& advancedParse) const;

        /**
         * @brief No mcsm::Result needed
        */
        std::string getPath();

        /**
         * @brief No mcsm::Result needed
        */
        std::string getName();

        nlohmann::json getValue(const std::string& key) const;
        mcsm::Result setValue(const std::string& key, const nlohmann::json& value) const;
        bool hasValue(const std::string& key) const;

        bool exists() const override;

        /**
         * @brief No mcsm::Result needed
        */
        bool isGlobal() const override;

        mcsm::Result save(const nlohmann::json& jsonData) const;
        
        mcsm::Result reset() const;
    };
}

#endif