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

#ifndef __MCSM_DOWNLOADABLE_H__
#define __MCSM_DOWNLOADABLE_H__

#include <vector>
#include <memory>

namespace mcsm {
    /**
     * Represents a downloadable server interface.
     */
    class Downloadable {
    public:
        /**
         * Returns vector of downloadable versions.
         * @return vector of available versions
         */
        virtual std::vector<std::string> getAvailableVersions() = 0;

        /**
         * Downloads the server jarfile in the current directory.
         * @param version version of the server
         */
        virtual void download(const std::string& version) = 0;

        /**
         * Downloads the server jarfile in the specified directory.
         * @param version version of the server
         * @param path file's path
         */
        virtual void download(const std::string& version, const std::string& path) = 0;

        /**
         * Downloads the server jarfile in the specified directory with the following name.
         * @param version version of the server
         * @param path file's path
         * @param name file's name
         */
        virtual void download(const std::string& version, const std::string& path, const std::string& name) = 0;

        /**
         * Downloads the server jarfile in the specified directory with the following name.
         * @param version version of the server
         * @param path file's path
         * @param name file's name
         * @param optionPath server config to check in
         */
        //virtual void download(const std::string& version, const std::string& path, const std::string& name, const std::string& optionPath) = 0;

        /**
         * Returns if the following server has this version.
         * @param version version of the server
         * @return true when version exists, otherwise false
         */
        virtual bool hasVersion(const std::string& version) = 0;
    };
}

#endif // __MCSM_DOWNLOADABLE_H__