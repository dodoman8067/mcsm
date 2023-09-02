#include "option.h"

mcsm::Option::Option(const std::string& path, const std::string& name){
    std::string path1 = path;
    std::replace(path1.begin(), path1.end(), '.', '/');
    std::string finalPath = "./.mcsm" + path1;
    this->path = finalPath;

    std::string name1 = name;
    name1 = name1.append(".json");
    this->name = name1;

    this->file = new std::ifstream();
    this->file->open(this->path, std::ios::out | std::ios::app);
    this->file->close();
    this->file->open(this->path, std::ios::in | std::ios::out | std::ios::app);
    std::cout << "path: " << this->path << this->name << std::endl;
}

mcsm::Option::~Option(){
    this->file->close();
    delete this->file;
    this->file = nullptr;
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