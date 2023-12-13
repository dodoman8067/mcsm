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

#ifndef __MCSM_SERVER_GENERATOR_H__
#define __MCSM_SERVER_GENERATOR_H__

#include <mcsm/data/options/server_option.h>
#include <mcsm/server/type/bukkit/paper_server.h>
#include <mcsm/server/type/bukkit/purpur_server.h>
#include <mcsm/server/type/base/vanilla_server.h>
#include <mcsm/server/server_type.h>

namespace mcsm {
    /**
     * Namespace for server generation-related methods.
    */
    namespace server {
        /**
         * Generates a Bukkit-API based Minecraft server.
         * @param name server's name
         * @param option server's default JVM launch profile
         * @param version Minecraft version for the server
         * @param type Bukkit-API server implementation (e.g., CRAFTBUKKIT, SPIGOT, PAPER, PURPUR, PUFFERFISH, FOLIA)
        */
        void generateBukkit(const std::string& name, mcsm::JvmOption& option, const std::string& version, const mcsm::BukkitServerType& type);

        /**
         * Generates a Forge server.
         * @param name server's name
         * @param option server's default JVM launch profile
         * @param version Minecraft version for the server
        */
        void generateForge(const std::string& name, mcsm::JvmOption& option, const std::string& version);

        /**
         * Generates a Fabric server.
         * @param name server's name
         * @param option server's default JVM launch profile
         * @param version Minecraft version for the server
        */
        void generateFabric(const std::string& name, mcsm::JvmOption& option, const std::string& version);

        /**
         * Generates a SpongeVanilla server.
         * @param name server's name
         * @param option server's default JVM launch profile
         * @param version Minecraft version for the server
        */
        void generateSponge(const std::string& name, mcsm::JvmOption& option, const std::string& version);

        /**
         * Generates a custom server.
         * @param name server's name
         * @param option server's default JVM launch profile
         * @param filePath jarfile's location (path)
         * @param version Minecraft version for the server
        */
        void generateCustom(const std::string& name, mcsm::JvmOption& option, const std::string& filePath, const std::string& version);

        /**
         * Generates a Vanilla server.
         * @param name server's name
         * @param option server's default JVM launch profile
         * @param version Minecraft version for the server
        */
        void generateVanilla(const std::string& name, mcsm::JvmOption& option, const std::string& version);

        /**
         * Configures a server.
         * @param serverOption mcsm::ServerOption instance
         * @param name server's name
         * @param option server's default JVM launch profile
        */
        void configure(mcsm::ServerOption& serverOption, const std::string& name, mcsm::JvmOption& option);

        /**
         * Returns a pointer of server instance.
         * @param server server's type
         * @return pointer of server instance based on `server`
        */
        std::shared_ptr<mcsm::Server> detectServerType(const std::string& server);
    };
}

#endif // __MCSM_SERVER_GENERATOR_H__