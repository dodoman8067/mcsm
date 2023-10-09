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
    map->insert(std::make_pair("1.19.1", "https://piston-data.mojang.com/v1/objects/8399e1211e95faa421c1507b322dbeae86d604df/server.jar"));
    map->insert(std::make_pair("1.19", "https://piston-data.mojang.com/v1/objects/e00c4052dac1d59a1188b2aa9d5a87113aaf1122/server.jar"));
    map->insert(std::make_pair("1.18.2", "https://piston-data.mojang.com/v1/objects/c8f83c5655308435b3dcf03c06d9fe8740a77469/server.jar"));
    map->insert(std::make_pair("1.18.1", "https://piston-data.mojang.com/v1/objects/125e5adf40c659fd3bce3e66e67a16bb49ecc1b9/server.jar"));
    map->insert(std::make_pair("1.18", "https://piston-data.mojang.com/v1/objects/3cf24a8694aca6267883b17d934efacc5e44440d/server.jar"));
    map->insert(std::make_pair("1.17.1", "https://piston-data.mojang.com/v1/objects/a16d67e5807f57fc4e550299cf20226194497dc2/server.jar"));
    map->insert(std::make_pair("1.17", "https://piston-data.mojang.com/v1/objects/0a269b5f2c5b93b1712d0f5dc43b6182b9ab254e/server.jar"));
    map->insert(std::make_pair("1.16.5", "https://piston-data.mojang.com/v1/objects/1b557e7b033b583cd9f66746b7a9ab1ec1673ced/server.jar"));
    map->insert(std::make_pair("1.16.4", "https://piston-data.mojang.com/v1/objects/35139deedbd5182953cf1caa23835da59ca3d7cd/server.jar"));
    map->insert(std::make_pair("1.16.3", "https://piston-data.mojang.com/v1/objects/f02f4473dbf152c23d7d484952121db0b36698cb/server.jar"));
    map->insert(std::make_pair("1.16.2", "https://piston-data.mojang.com/v1/objects/c5f6fb23c3876461d46ec380421e42b289789530/server.jar"));
    map->insert(std::make_pair("1.16.1", "https://piston-data.mojang.com/v1/objects/a412fd69db1f81db3f511c1463fd304675244077/server.jar"));
    map->insert(std::make_pair("1.16", "https://piston-data.mojang.com/v1/objects/a0d03225615ba897619220e256a266cb33a44b6b/server.jar"));
    map->insert(std::make_pair("1.15.2", "https://piston-data.mojang.com/v1/objects/bb2b6b1aefcd70dfd1892149ac3a215f6c636b07/server.jar"));
    mcsm::GlobalOption option(".server.url", "vanilla_server");
    if(!option.hasValue("versions")){
        option.setValue("versions", *map);
    }
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