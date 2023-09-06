#ifndef __MCSM_OPTION_H__
#define __MCSM_OPTION_H__

#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

namespace mcsm {
    class Option {
    private:
        std::string path;
        std::string name;
        bool createDirectories(std::string const &dirName, std::error_code &err);
    public:
        Option(const std::string& path, const std::string& name);
        ~Option();
        nlohmann::json load() const;
        std::string getPath();
        std::string getName();
        nlohmann::json getValue(const std::string& key) const;
        bool hasValue(const std::string& key) const;
        void setValue(const std::string& key, const nlohmann::json& value);
        void save(const nlohmann::json& jsonData) const;
    };
}

#endif