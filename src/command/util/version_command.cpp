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

#include <mcsm/command/util/version_command.h>

mcsm::VersionCommand::VersionCommand(const std::string& name, const std::string& description, const std::string& version) : mcsm::Command(name, description) {
    this->version = version;
}

mcsm::VersionCommand::~VersionCommand(){

}

void mcsm::VersionCommand::execute(const std::vector<std::string>& /* args */){
    #ifdef MCSM_DEBUG
        std::cout << "MCSM version : " << version << " (DEBUG)\n";
    #else
        std::cout << "MCSM version : " << version << "\n";
    #endif
    std::cout << "cURL version : " << curl_version() << "\n";
    #ifdef MCSM_NLOHMANN_VER
        std::cout << "nlohmann::json version : " << MCSM_NLOHMANN_VER << "\n";
    #endif
    #ifdef LIBGIT2_VERSION
        std::cout << "libgit2 version : " << LIBGIT2_VERSION << "\n";
    #endif
}