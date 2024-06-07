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

#ifndef __MCSM_COMPILED_H__
#define __MCSM_COMPILED_H__

#include <mcsm/data/options/server_option.h>

namespace mcsm {
    /**
     * Represents a compiled (binary not available) server interface.
     */
    class Compiled {
    public:
        /**
        * Returns vector of available versions.
        * @return vector of available versions
        */
        virtual std::vector<std::string> getAvailableVersions() = 0;

        /**
         * Clones the source code of server for specified version.
         * @param version version of the server
         */
        virtual mcsm::Result cloneSource(const std::string& version) = 0;

        /**
         * Clones the source code of server for specified version in the specified directory.
         * @param version version of the server
         * @param path file's path
         */
        virtual mcsm::Result cloneSource(const std::string& version, const std::string& path) = 0;

        /**
         * Compiles the source code of the server.
         * @param version version of the server
         */
        virtual mcsm::Result compile(const std::string version);

        /**
         * Compiles the source code of the server.
         * @param version version of the server
         * @param workingPath where to perform the compilation
         */
        virtual mcsm::Result compile(const std::string version, const std::string workingPath);

        /**
         * Compiles the source code of the server.
         * @param version version of the server
         * @param workingPath where to perform the compilation
         * @param outputPath where to copy the output jar
         */
        virtual mcsm::Result compile(const std::string version, const std::string workingPath, const std::string outputPath);

        /**
         * Returns if the following server has this version.
         * @param version version of the server
         * @return true when version exists, otherwise false
         */
        virtual bool hasVersion(const std::string& version) = 0;
    };
}

#endif // __MCSM_DOWNLOADABLE_H__