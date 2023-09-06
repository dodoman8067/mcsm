#include "server_type.h"

mcsm::ServerType mcsm::getServerType(const std::string& name){
    if(name == "vanilla"){
        return mcsm::ServerType::VANILLA;
    }else if (name == "bukkit"){
        return mcsm::ServerType::BUKKIT;
    }else if (name == "sponge_vanilla" || name == "sponge"){
        return mcsm::ServerType::SPONGE_VANILLA;
    }else if(name == "bungeecord" || name == "bungee"){
        return mcsm::ServerType::BUNGEECORD;
    }else if(name == "velocity"){
        return mcsm::ServerType::VELOCITY;
    }else if(name == "forge"){
        return mcsm::ServerType::FORGE;
    }else if(name == "fabric"){
        return mcsm::ServerType::FABRIC;
    } else{
        return mcsm::ServerType::UNKNOWN;
    }
}

std::string mcsm::getServerType(const mcsm::ServerType& type){
    if(type == mcsm::ServerType::VANILLA){
        return "vanilla";
    }else if (type == mcsm::ServerType::BUKKIT){
        return "bukkit";
    }else if (type == mcsm::ServerType::SPONGE_VANILLA){
        return "sponge_vanilla";
    }else if(type == mcsm::ServerType::BUNGEECORD){
        return "bungeecord";
    }else if(type == mcsm::ServerType::VELOCITY){
        return "velocity";
    }else if(type == mcsm::ServerType::FORGE){
        return "forge";
    }else if(type == mcsm::ServerType::FABRIC){
        return "fabric";
    } else{
        return nullptr;
    }
}