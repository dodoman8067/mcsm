#ifndef __MCSM_OPTION_H__
#define __MCSM_OPTION_H__

#include <string>
#include <filesystem>
#include <iostream>
#include <nlohmann/json.hpp>

namespace mcsm {
    class Option {
    private:
        std::string path;
        nlohmann::json data;
    public:
        Option(const std::string& path);
        ~Option();
        nlohmann::json getData();
    };
}


#endif