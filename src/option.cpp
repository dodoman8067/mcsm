#include "option.h"

mcsm::Option::Option(const std::string& path){
    std::string path1 = path;
    std::replace(path1.begin(), path1.end(), '.', '/');
    this->path = path1;
    std::cout << "path: " << this->path << std::endl;
}

mcsm::Option::~Option(){

}

nlohmann::json mcsm::Option::getData(){
    return nullptr;
}