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

#ifndef __MCSM_SERVER_REGISTRY_H__
#define __MCSM_SERVER_REGISTRY_H__

#include <unordered_map>
#include <functional>
#include <memory>
#include <string>
#include <mcsm/server/server.h>
#include <mcsm/server/server_type.h>
#include <mcsm/data/options/general_property.h>

namespace mcsm {
    using ServerFactory = std::function<std::shared_ptr<mcsm::Server>()>;

    class ServerRegistry {
    public:
        static mcsm::ServerRegistry& getServerRegistry();
        void registerServer(const std::string& name, mcsm::ServerFactory factory, mcsm::ServerType type);

        std::shared_ptr<mcsm::Server> getServer(const std::string& name) const;

        std::string getServerTypeString(const mcsm::ServerType type) const;

        void registerGeneralProperty(const std::string& name, std::unique_ptr<mcsm::GeneralProperty> property);

        mcsm::GeneralProperty* getGeneralProperty(const std::string& name);

        std::vector<mcsm::GeneralProperty*> getRegisteredProperties();
    private:
        struct ServerEntry {
            mcsm::ServerFactory factory;
            mcsm::ServerType type;
        };

        static std::unordered_map<std::string, ServerEntry> serverFactories;
        static std::unordered_map<std::string, std::unique_ptr<mcsm::GeneralProperty>> generalProperties;

        ServerRegistry() = default;
        ~ServerRegistry() = default;

        ServerRegistry(const ServerRegistry&) = delete;
        ServerRegistry& operator=(const ServerRegistry&) = delete;
    };
}

#endif // __MCSM_SERVER_REGISTRY_H__