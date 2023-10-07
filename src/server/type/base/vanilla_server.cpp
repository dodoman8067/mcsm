#include "vanilla_server.h"

mcsm::VanillaServer::VanillaServer(){
    this->versions = std::make_unique<std::map<const std::string, const std::string>>();
    this->init();
}

mcsm::VanillaServer::~VanillaServer(){}

void mcsm::VanillaServer::init(){
    std::unique_ptr<std::map<const std::string, const std::string>>& map = this->versions;
    map->insert(std::make_pair("1.20.2", "https://piston-data.mojang.com/v1/objects/5b868151bd02b41319f54c8d4061b8cae84e665c/server.jar"));
    map->insert(std::make_pair("1.20.1", "https://piston-data.mojang.com/v1/objects/84194a2f286ef7c14ed7ce0090dba59902951553/server.jar"));
    map->insert(std::make_pair("1.20", "https://piston-data.mojang.com/v1/objects/15c777e2cfe0556eef19aab534b186c0c6f277e1/server.jar"));
    map->insert(std::make_pair("1.19.4", "https://piston-data.mojang.com/v1/objects/8f3112a1049751cc472ec13e397eade5336ca7ae/server.jar"));
    map->insert(std::make_pair("1.19.3", "https://piston-data.mojang.com/v1/objects/c9df48efed58511cdd0213c56b9013a7b5c9ac1f/server.jar"));
    map->insert(std::make_pair("1.19.2", "https://piston-data.mojang.com/v1/objects/f69c284232d7c7580bd89a5a4931c3581eae1378/server.jar"));
    mcsm::GlobalOption option(".server.url", "vanilla_server");
    option.setValue("versions", *map);
}

std::unique_ptr<std::vector<std::string>> mcsm::VanillaServer::getAvailableVersions(){
    std::unique_ptr<std::vector<std::string>> vector = std::make_unique<std::vector<std::string>>();
    vector->reserve(this->versions->size());
    return vector;
}

void mcsm::VanillaServer::download(const std::string& version){
    
}

void mcsm::VanillaServer::download(const std::string& version, const std::string& path){

}

void mcsm::VanillaServer::download(const std::string& version, const std::string& path, const std::string& name){

}

bool mcsm::VanillaServer::hasVersion(const std::string& version){
    return false;
}