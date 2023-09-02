#include "option.h"

mcsm::Option::Option(const std::string& path, const std::string& name){
    std::string path1 = path;
    std::replace(path1.begin(), path1.end(), '.', '/');
    std::string finalPath = "./.mcsm" + path1;
    this->path = finalPath;

    std::string name1 = name;
    name1 = name1.append(".json");
    this->name = name1;

    this->file = new std::ifstream(finalPath);
    std::cout << "path: " << this->path << this->name << std::endl;
}

mcsm::Option::~Option(){
    delete this->file;
}

bool mcsm::Option::createDirectories(std::string const &dirName, std::error_code &err){
    err.clear();
    if(!std::filesystem::create_directories(dirName, err)){
        if(std::filesystem::exists(dirName)){
            err.clear();
            return true;    
        }
        return false;
    }
    return true;
}

nlohmann::json mcsm::Option::load(){
    return nullptr;
}