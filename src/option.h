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
        std::ifstream* file;
        nlohmann::json data;
        bool createDirectories(std::string const &dirName, std::error_code &err);
    public:
        Option(const std::string& path, const std::string& name);
        ~Option();
        nlohmann::json load();
    };
}


#endif