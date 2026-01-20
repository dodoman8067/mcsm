#ifndef __MCSM_TOML_OPTION_H__
#define __MCSM_TOML_OPTION_H__

#include <toml++/toml.hpp>
#include <filesystem>
#include <fstream>
#include <mcsm/util/cli/logging.h>
#include <mcsm/util/string_utils.h>
#include <mcsm/util/cli/cli_utils.h>

namespace mcsm {
    class TomlOption {
    private:
        std::string path;
        std::string name;
        mutable toml::table data; 
        bool loaded = false;

        mcsm::BoolResult createDirectories(std::string const &dirName, std::error_code &err) const;
    public:
        /**
         * @brief No mcsm::Result needed
        */
        TomlOption(const std::string& path, const std::string& name);
        /**
         * @brief No mcsm::Result needed
        */
        ~TomlOption();

        mcsm::VoidResult create();

        mcsm::VoidResult load();

        mcsm::VoidResult load(const bool& advancedParse);

        toml::table& getData() const;

        /**
         * @brief No mcsm::Result needed
        */
        std::string getPath() const;

        /**
         * @brief No mcsm::Result needed
        */
        std::string getName() const;

        mcsm::Result<toml::node*> getValue(const std::string& key) const;
        mcsm::VoidResult setValue(const std::string& key, toml::node& value);
        mcsm::BoolResult hasValue(const std::string& key) const;

        mcsm::BoolResult exists() const; // todo edit configurable or remove it too

        /**
         * @brief No mcsm::Result needed
        */
        bool isGlobal() const;

        mcsm::VoidResult save();

        mcsm::VoidResult save(const toml::table& table);
        
        mcsm::VoidResult reset();
    };
}

#endif // __MCSM_TOML_OPTION_H__