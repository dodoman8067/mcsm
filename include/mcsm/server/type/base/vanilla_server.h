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
#include <mcsm/http/get.h>
#include <mcsm/http/download.h>
#include <map>

namespace mcsm {
    class VanillaServer : public mcsm::Server, public mcsm::Downloadable, public std::enable_shared_from_this<VanillaServer> {
    private:
        std::unique_ptr<std::map<const std::string, const std::string>> versions;
        std::string getVersionObject(const std::string& ver) const;
        std::string getServerJarURL(const std::string& ver) const;
    public:
        VanillaServer();
        ~VanillaServer();

        std::string getSupportedVersions() const override;

        std::string getBasedServer() const override;

        std::string getWebSite() const override;

        std::string getGitHub() const override;

        std::vector<std::string> getAvailableVersions() override;

        mcsm::Result download(const std::string& version) override;
        mcsm::Result download(const std::string& version, const std::string& path) override;
        mcsm::Result download(const std::string& version, const std::string& path, const std::string& name) override;
        mcsm::Result download(const std::string& version, const std::string& path, const std::string& name, const std::string& optionPath) override;

        mcsm::Result obtainJarFile(const std::string& version, const std::string& path, const std::string& name, const std::string& optionPath) override;

        mcsm::Result start(mcsm::ServerConfigLoader* loader, mcsm::JvmOption& option) override;
        mcsm::Result start(mcsm::ServerConfigLoader* loader, mcsm::JvmOption& option, const std::string& path, const std::string& optionPath) override;
        
        bool hasVersion(const std::string& version) const override;

        const std::map<std::string, std::string> getRequiredValues() const override;

        mcsm::ServerType getType() const override;
        std::string getTypeAsString() const override;

        mcsm::Result generate(const std::string& name, mcsm::JvmOption& option, const std::string& path, const std::string& version, const bool& autoUpdate, const std::map<std::string, std::string>& extraValues) override;
    };
}


#endif // __MCSM_VANILLA_SERVER_H__