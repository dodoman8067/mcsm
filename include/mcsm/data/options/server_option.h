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


#ifndef __MCSM_SERVER_OPTION_H__
#define __MCSM_SERVER_OPTION_H__

#include <mcsm/data/option.h>
#include <mcsm/server/server.h>
#include <mcsm/server/type/bukkit/paper_server.h>
#include <mcsm/server/type/downloadable.h>
#include <mcsm/data/options/server_data_option.h>
#include <memory>

namespace mcsm {
    class ServerOption {
    private:
        std::shared_ptr<mcsm::Server> server;
        std::string version;
    public:
        ServerOption();
        ServerOption(const std::string& version);
        ServerOption(const std::string& version, std::shared_ptr<mcsm::Server> server);
        ~ServerOption();
        void create(const std::string& name, mcsm::JvmOption& defaultOption);
        void start();
        void start(std::unique_ptr<mcsm::JvmOption> option);
        bool exists();
        std::string getServerName() const;
        void setServerName(const std::string& name);
        std::string getServerVersion() const;
        void setServerVersion(const std::string& version);
        std::unique_ptr<mcsm::JvmOption> getDefaultOption() const;
        void setDefaultOption(std::unique_ptr<mcsm::JvmOption> jvmOption);
        std::string getServerType() const;
        std::string getServerJarFile() const;
        void setServerJarFile(const std::string& name);
    };
}

#include <mcsm/server/server_generator.h>

#endif // __MCSM_SERVER_OPTION_H__