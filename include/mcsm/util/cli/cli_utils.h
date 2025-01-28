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

#ifndef __MCSM_CLI_UTILS_H__
#define __MCSM_CLI_UTILS_H__

#include <string>
#include <filesystem>
#include <cstdlib>
#include <iostream>
#include <functional>
#include <mcsm/util/os/os_detection.h>
#include <mcsm/util/cli/result.h>
#include <mcsm/util/cli/logging.h>
#include <any>
#ifdef __linux__
    #include <sys/wait.h>
#endif

namespace mcsm {
    /**
     * Runs a console command without printing the output.
     * @param command string to run as a command
    */
    int runCommandQuietly(const std::string& command);

    /**
     * Runs a console command.
     * @param command string to run as a command
    */
    int runCommand(const std::string& command);

    std::string getCurrentPath();
    bool fileExists(const std::string& path);
    bool removeFile(const std::string& path);
    bool mkdir(const std::string& dirName);

    /**
     * @brief Get the Input object
     * 
     * @param input input string (will contain the final value)
     * @param beforeInputFunction called before accepting an input.
     * @param invalidInputFunction called if invalid input was passed
     * @param checkValid checks if the input is valid
     */
    void getInput(std::string& input, std::function<void(std::vector<std::string>)> beforeInputFunction, std::function<void(std::vector<std::string>)> invalidInputFunction, std::function<bool(std::vector<std::string>)> checkValid);

    bool isDebug();

    std::string getExecutablePath();
}

#endif // __MCSM_CLI_UTILS_H__