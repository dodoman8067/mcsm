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

std::unordered_map<std::string, std::unique_ptr<mcsm::Server>> mcsm::ServerRegistry::serverFactories;
std::unordered_map<std::string, std::unique_ptr<mcsm::GeneralProperty>> mcsm::ServerRegistry::generalProperties;

mcsm::ServerRegistry& mcsm::ServerRegistry::getServerRegistry(){
    static mcsm::ServerRegistry instance;
    return instance;
}

void mcsm::ServerRegistry::registerServer(const std::string& name, std::unique_ptr<mcsm::Server> server){
    this->serverFactories[name] = std::move(server);
}

void mcsm::ServerRegistry::registerGeneralProperty(const std::string& name, std::unique_ptr<mcsm::GeneralProperty> property){
    this->generalProperties[name] = std::move(property);
}

tl::expected<mcsm::GeneralProperty*, mcsm::Error> mcsm::ServerRegistry::getGeneralProperty(const std::string& name){
    auto it = this->generalProperties.find(name);
    if(it != this->generalProperties.end()){
        mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, { "Success" }});
        return it->second.get();
    }
    mcsm::Result res({mcsm::ResultType::MCSM_FAIL, { "Not a registered property: " + name }});
    return nullptr;
}

std::vector<mcsm::GeneralProperty*> mcsm::ServerRegistry::getRegisteredProperties(){
    std::vector<mcsm::GeneralProperty*> properties;
    std::unordered_map<std::string, std::unique_ptr<mcsm::GeneralProperty>>::iterator it;
    for(it = this->generalProperties.begin(); it != this->generalProperties.end(); ++it){
        properties.push_back(it->second.get());
    }
    return properties;
}

tl::expected<mcsm::Server*, mcsm::Error> mcsm::ServerRegistry::getServer(const std::string& name) const {
    auto it = this->serverFactories.find(name);
    if(it != this->serverFactories.end()){
        mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, { "Success" }});
        return it->second.get();
    }
    mcsm::Result res({mcsm::ResultType::MCSM_FAIL, { "Server type not found: " + name }});
    return nullptr;
}

mcsm::StringResult mcsm::ServerRegistry::getServerTypeString(const mcsm::ServerType& type) const {
    for(const auto&[id, server]: serverFactories){
        if(server->getType() == type){
            mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, { "Success" }});
            return id;
        }
    }
    mcsm::Result res({mcsm::ResultType::MCSM_FAIL, { "Server type not found" }});
    return "";
}