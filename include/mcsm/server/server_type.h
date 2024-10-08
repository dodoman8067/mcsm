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

#ifndef __MCSM_SERVER_TYPE_H__
#define __MCSM_SERVER_TYPE_H__

#include <string>

namespace mcsm {
    // Represents an enum of Minecraft server type.
    enum class ServerType {
        // Represents vanilla server
        VANILLA,
        // Represents bukkit server
        BUKKIT,
        // Represents sponge vanilla server
        SPONGE_VANILLA,
        // Represents bungeecord server
        BUNGEECORD,
        // Represents velocity server
        VELOCITY,
        // Represents forge server
        FORGE,
        // Represents fabric server
        FABRIC,
        // Represents custom type of server
        CUSTOM,
        // Represents unknown type of server
        UNKNOWN
    };

    // Represents an enum of server type that implements bukkit api.
    enum class BukkitServerType {
        // Represents craftbukkit
        CRAFTBUKKIT,
        // Represents spigot
        SPIGOT,
        // Represents paper
        PAPER,
        // Represents purpur
        PURPUR,
        // Represents folia
        FOLIA
    };

    /**
     * Returns an enum representing server type in string.
     * @param name string to convert to an enum value
     * @return enum representing server type
    */
    mcsm::ServerType getServerType(const std::string& name);

    /**
     * Returns a string representing server type in enum.
     * @param type enum value to convert it to a string
     * @return string version of server type
    */
    std::string getServerType(const mcsm::ServerType& type);
}

#endif