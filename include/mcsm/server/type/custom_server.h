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

#ifndef __MCSM_CUSTOM_SERVER_H__
#define __MCSM_CUSTOM_SERVER_H__

#include <mcsm/server/server.h>
#include <mcsm/http/download.h>
#include <regex>

namespace mcsm {
    class CustomServer : public mcsm::Server, public std::enable_shared_from_this<CustomServer> {
    private:
        bool isFile(const std::string& location) const;
        bool isURL(const std::string& location) const;
    public:
        CustomServer();
        ~CustomServer();

        mcsm::ServerType getType() const override;
        std::string getTypeAsString() const override;

        std::string getFileLocation(const std::string& optionPath) const;
        mcsm::Result setFileLocation(mcsm::Option* option, const std::string& location);
        mcsm::Result setupServerJarFile(const std::string& path, const std::string& optionPath);

        std::string getCustomStartCommand(const std::string& optionPath) const;
        mcsm::Result setCustomStartCommand(mcsm::Option* option, const std::string& command);

        std::string getSupportedVersions() const override;

        std::string getBasedServer() const override;
        bool isBasedAs(const std::string& input) const override;

        std::string getWebSite() const override;
        std::string getGitHub() const override;

        mcsm::Result obtainJarFile(const std::string& version, const std::string& path, const std::string& name, const std::string& optionPath) override;

        mcsm::Result start(mcsm::ServerConfigLoader* loader, mcsm::JvmOption& option, const std::string& path, const std::string& optionPath) override;

        const std::map<std::string, std::string> getRequiredValues() const override;

        mcsm::Result generate(const std::string& name, mcsm::JvmOption& option, const std::string& path, const std::string& version, const bool& autoUpdate, const std::map<std::string, std::string>& extraValues) override;
        mcsm::Result generate(const std::string& name, mcsm::JvmOption& option, const std::string& path, const std::string& version, const bool& autoUpdate, const std::string& fileLocation, const std::map<std::string, std::string>& extraValues);
    };
}

#endif // __MCSM_CUSTOM_SERVER_H__