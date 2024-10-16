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

#ifndef __SERVER_DATA_OPTION_H__
#define __SERVER_DATA_OPTION_H__

#include <mcsm/data/option.h>
#include <mcsm/util/cli/cli_utils.h>
#include <chrono>

namespace mcsm {
    class ServerDataOption {
    protected:
        std::string path;
        std::unique_ptr<mcsm::Option> option;
    public:
        ServerDataOption();
        ServerDataOption(const std::string& path);
        ~ServerDataOption();

        mcsm::Result load();

        mcsm::Result create(const std::string& lastTimeLaunched);

        mcsm::Result reset();

        std::string getLastTimeLaunched() const;
        mcsm::Result updateLastTimeLaunched();

        std::string getServerTimeCreated() const;
        mcsm::Result updateServerTimeCreated();

        std::string getLastDownloadedBuild() const;
        mcsm::Result updateLastDownloadedBuild(const std::string& build);
        
        bool exists() const;
    };
}

#endif // __SERVER_DATA_OPTION_H__