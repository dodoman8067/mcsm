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

#ifndef __MCSM_MULTI_SERVER_OPTION_H__
#define __MCSM_MULTI_SERVER_OPTION_H__

#include <mcsm/data/options/server_option.h>
#include <mcsm/server/server_process.h>
#include <mcsm/data/options/modded/fabric_server_option.h>
#include <variant>
#include <atomic>

namespace mcsm {
    class MultiServerOption {
    private:
        std::vector<std::unique_ptr<std::variant<mcsm::ServerOption, mcsm::FabricServerOption>>> servers;
        mutable std::vector<std::pair<std::string, std::shared_ptr<mcsm::ServerProcess>>> processes;
        std::unique_ptr<mcsm::Option> option;

        mutable std::mutex mtx;

        mcsm::Result load();
        mcsm::Result save();
        
        bool canBeTaken(const std::string& serverName) const;
        bool anyRunning() const;
        
        mcsm::Result addProcesses() const;
        
        std::string getServerStartCommand(std::variant<mcsm::ServerOption, mcsm::FabricServerOption>& server) const;

        mcsm::Result downloadPerServer();

        void inputHandler(std::atomic_bool& stopFlag) const;
        void processMonitor(std::atomic_bool& stopFlag) const;
    public:
        MultiServerOption(const std::string& path);
        ~MultiServerOption();
        
        bool exists() const;

        mcsm::Result create() const;

        mcsm::Result addServer(std::unique_ptr<std::variant<mcsm::ServerOption, mcsm::FabricServerOption>>& server);
        mcsm::Result removeServer(std::unique_ptr<std::variant<mcsm::ServerOption, mcsm::FabricServerOption>>& server);

        const std::vector<std::unique_ptr<std::variant<mcsm::ServerOption, mcsm::FabricServerOption>>>& getServers() const;
        
        const std::vector<mcsm::ServerOption*> getServerOptions() const;
        const std::vector<mcsm::FabricServerOption*> getFabricServerOptions() const;

        mcsm::Result start() const;
    };
}

#endif // __MCSM_MULTI_SERVER_OPTION_H__