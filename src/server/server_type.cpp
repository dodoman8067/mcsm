/*
Copyright (c) 2023 dodoman8067

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

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