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

#ifndef __MCSM_FABRIC_SERVER_H__
#define __MCSM_FABRIC_SERVER_H__

#include <mcsm/server/type/downloadable.h>
#include <mcsm/http/get.h>
#include <mcsm/http/download.h>
#include <mcsm/data/options/server_option.h>
#include <mcsm/util/mc/mc_utils.h>

namespace mcsm {
    class FabricServer : public mcsm::Server, public mcsm::Downloadable {
    private:
    public:
        FabricServer();
        ~FabricServer();

        std::string getVersion(const std::string& ver) const;
        std::string getVersion() const;

        std::vector<std::string> getAvailableVersions() override;

        std::string getSupportedVersions() const override;

        std::string getBasedServer() const override;

        std::string getWebSite() const override;

        std::string getGitHub() const override;

        mcsm::Result start(mcsm::JvmOption& option) override;
        mcsm::Result start(mcsm::JvmOption& option, const std::string& path, const std::string& optionPath) override;

        mcsm::Result download(const std::string& version) override;
        mcsm::Result download(const std::string& version, const std::string& path) override;
        mcsm::Result download(const std::string& version, const std::string& path, const std::string& name) override;
        mcsm::Result download(const std::string& version, const std::string& path, const std::string& name, const std::string& optionPath) override;
        mcsm::Result download(const std::string& version, const std::string& loaderVersion, const std::string& installerVersion, const std::string& path, const std::string& name);
        mcsm::Result download(const std::string& version, const std::string& loaderVersion, const std::string& installerVersion, const std::string& path, const std::string& name, const std::string& optionPath);

        mcsm::Result obtainJarFile(const std::string& version, const std::string& path, const std::string& name, const std::string& optionPath) override;

        bool hasVersion(const std::string& version) override;

        std::string getTypeAsString() const override;

        mcsm::ServerType getType() const override;
        
        mcsm::Result update();
        mcsm::Result update(const std::string& optionPath);
        mcsm::Result update(const std::string& path, const std::string& optionPath);
    };
}

#endif // __MCSM_FABRIC_SERVER_H__