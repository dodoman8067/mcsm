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

#ifndef __MCSM_VANILLA_SERVER_H__
#define __MCSM_VANILLA_SERVER_H__

#include <mcsm/server/server.h>
#include <mcsm/server/type/downloadable.h>
#include <mcsm/data/global_option.h>
#include <mcsm/data/options/server_option.h>
#include <mcsm/http/download.h>
#include <map>

namespace mcsm {
    class VanillaServer : public mcsm::Server, public mcsm::Downloadable {
    private:
        std::unique_ptr<std::map<const std::string, const std::string>> versions;
        void init();
    public:
        VanillaServer();
        ~VanillaServer();
        std::string getJarFile() override;
        std::vector<std::string> getAvailableVersions() override;
        void download(const std::string& version) override;
        void download(const std::string& version, const std::string& path) override;
        void download(const std::string& version, const std::string& path, const std::string& name) override;
        void start(mcsm::JvmOption& option) override;
        bool hasVersion(const std::string& version) override;
        mcsm::ServerType getType() const override;
        std::string getTypeAsString() const override;
    };
}


#endif // __MCSM_VANILLA_SERVER_H__