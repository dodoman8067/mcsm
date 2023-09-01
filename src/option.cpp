#include "option.h"

mcsm::Option::Option(const std::string& path, const std::string& name){
    std::string path1 = path;
    std::replace(path1.begin(), path1.end(), '.', '/');
    this->path = path1;

    std::string name1 = name;
    name1 = name1.append(".json");
    this->name = name1;

    std::ifstream stream("./.mcsm" + this->path);
    std::cout << "path: " << "./.mcsm" << this->path << this->name << std::endl;
}

mcsm::Option::~Option(){

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