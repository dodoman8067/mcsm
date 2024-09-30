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

#ifndef __MCSM_BUKKIT_PLUGIN_H__
#define __MCSM_BUKKIT_PLUGIN_H__

#include <mcsm/http/get.h>
#include <nlohmann/json.hpp>

namespace mcsm {
    enum PluginRepo {
        BUKKITDEV,
        SPIGET,
        HANGAR
    };

    class BukkitPlugin {
    private:
        std::string name;

        bool found;
        int result;

        std::string nativeVersion;
        std::vector<std::string> supportedVersions;

        std::string downloads;
        std::string author;

        std::string createdAt;
        mcsm::PluginRepo repo;

        void init();
        std::string spigetGetCreatedAt();
        std::string spigetGetDownloads();
        std::string spigetGetNativeVersion();
        std::vector<std::string> spigetGetSupportedVersions();
        std::string spigetGetAuthor(const nlohmann::json& json);
    public:
        BukkitPlugin(const std::string& name, const mcsm::PluginRepo& repo);
        BukkitPlugin(const std::string& name, const mcsm::PluginRepo& repo, const int& result);
        ~BukkitPlugin();

        bool isFound() const;
        int getResult() const;

        std::string getName() const;
        std::string getAuthor() const;

        std::string getNativeVersion() const;
        std::vector<std::string> getSupportedVersions() const;

        std::string getDownloads() const;
        
        std::string getCreateAt() const;
        mcsm::PluginRepo getRepo() const;
    };
}

#endif // __MCSM_BUKKIT_PLUGIN_H__