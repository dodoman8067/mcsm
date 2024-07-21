/*
MIT License

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

#include <mcsm/server/server_registry.h>

std::unordered_map<std::string, mcsm::ServerRegistry::ServerEntry> mcsm::ServerRegistry::serverFactories;

mcsm::ServerRegistry& mcsm::ServerRegistry::getServerRegistry(){
    static mcsm::ServerRegistry instance;
    return instance;
}

void mcsm::ServerRegistry::registerServer(const std::string& name, ServerFactory factory, mcsm::ServerType type){
    this->serverFactories[name] = { factory, type };
}

std::shared_ptr<mcsm::Server> mcsm::ServerRegistry::getServer(const std::string& name) const {
    auto it = this->serverFactories.find(name);
    if(it != this->serverFactories.end()){
        mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, { "Success" }});
        return it->second.factory();
    }
    mcsm::Result res({mcsm::ResultType::MCSM_FAIL, { "Server type not found: " + name }});
    return nullptr;
}

std::string mcsm::ServerRegistry::getServerTypeString(const mcsm::ServerType type) const {
    for(const auto& pair : serverFactories){
        if(pair.second.type == type){
            mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, { "Success" }});
            return pair.first;
        }
    }
    mcsm::Result res({mcsm::ResultType::MCSM_FAIL, { "Server type not found" }});
    return "";
}