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

#ifndef __MCSM_SERVER_H__
#define __MCSM_SERVER_H__

#include <mcsm/server/server_type.h>
#include <mcsm/data/options/jvm_option.h>
#include <mcsm/util/cli/cli_utils.h>

namespace mcsm {
    /**
     * Represents base of server.
     */
    class Server {
    public:
        /**
         * Returns the base type of server.
         * @return mcsm::ServerType enum
         */
        virtual mcsm::ServerType getType() const = 0;

        /**
         * Returns server type as a string
         * @return server type as a string
        */
        virtual std::string getTypeAsString() const = 0;

        /**
         * Returns server jarfile name.
         * @return jarfile name
        */
        virtual std::string getJarFile() const;

        /**
         * Returns the earliest Minecraft version supported by this server.
         * @return earliest Minecraft version supported by this server
        */
        virtual std::string getSupportedVersions() const = 0;

        /**
         * Returns a server name this server is based on.
         * @return based server (Paper -> Spigot for example)
        */
        virtual std::string getBasedServer() const = 0;

        /**
         * Returns the official website link for this server.
         * @return link for official website
        */
        virtual std::string getWebSite() const = 0;

        /**
         * Returns the official GitHub repostiory link.
         * @return GitHub repostiory link
        */
        virtual std::string getGitHub() const = 0;

        /**
         * Returns if this server is based as server `input`.
         * @param input string to check against
         * @return true if this server is based as `input`, otherwise false
        */
        virtual bool isBasedAs(const std::string& input) const;

        /**
         * Starts a Minecraft server with following launch profile `option`
         * @param option JVM launch profile
        */
        virtual void start(mcsm::JvmOption& option);
    };
}

#endif // __MCSM_SERVER_H__