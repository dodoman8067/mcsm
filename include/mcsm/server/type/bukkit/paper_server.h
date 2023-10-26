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

#ifndef __MCSM_PAPER_SERVER_H__
#define __MCSM_PAPER_SERVER_H__

#include <mcsm/server/type/base/bukkit_server.h>
#include <mcsm/server/type/bukkit/downloadable.h>
#include <mcsm/http/get.h>
#include <mcsm/util/mc/mc_utils.h>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

namespace mcsm {
    class PaperServer : public mcsm::BukkitServer, public mcsm::Downloadable {
    private:
    public:
        int getVersion(const std::string& ver) const;
        std::vector<std::string> getAvailableVersions() override;
        void download(const std::string& version) override;
        void download(const std::string& version, const std::string& path) override;
        void download(const std::string& version, const std::string& path, const std::string& name) override;
        bool hasVersion(const std::string& version) override;
    };
}


#endif // __MCSM_PAPER_SERVER_H__