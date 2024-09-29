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


#ifndef __MCSM_DOWNLOAD_H__
#define __MCSM_DOWNLOAD_H__

#include <curl/curl.h>
#include <mcsm/util/cli/logging.h>
#include <mcsm/http/holder.h>
#include <mcsm/util/cli/cli_utils.h>
#include <cstdio>
#include <filesystem>

namespace mcsm {
    /**
     * Downloads a file in `url` named as `name`.
     * @param name file's name
     * @param url link to perform GET request
    */
    mcsm::Result download(const std::string& name, const std::string& url);

    /**
     * Downloads a file in `url` named as `name`.
     * @param name file's name
     * @param url link to perform GET request
     * @param path file's stored location
    */
    mcsm::Result download(const std::string& name, const std::string& url, const std::string& path);

    /**
     * Downloads a file in `url` named as `name` while printing percentages.
     * @param name file's name
     * @param url link to perform GET request
     * @param path file's stored location
     * @param percentages progress of downloading the file
    */
    mcsm::Result download(const std::string& name, const std::string& url, const std::string& path, const bool& percentages);

    /**
     * Checks if the following `url` returns a text.
     * @param url link to check with
     * @return true if the following `url` returns a text, otherwise false
     */
    bool isText(const std::string& url);
}

#endif // __MCSM_DOWNLOAD_H__