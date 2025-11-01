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
#include <climits>
#ifdef __linux__
    #include <unistd.h>
    #include <sys/wait.h>
    #include <errno.h>
#endif
#ifdef __APPLE__
    #include <sys/wait.h>
    #include <mach-o/dyld.h>
#endif

namespace mcsm {
    /**
     * Runs a console command without printing the output.
     * @param command string to run as a command
    */
    mcsm::IntResult runCommandQuietly(const std::string& command);

    /**
     * Runs a console command.
     * @param command string to run as a command
    */
    mcsm::IntResult runCommand(const std::string& command);

    mcsm::StringResult getCurrentPath();
    mcsm::BoolResult fileExists(const std::string& path);
    mcsm::BoolResult removeFile(const std::string& path);
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

    static inline std::string getEnvStr(const char* k){
        if(!k) return {};
        if(const char* v = std::getenv(k)) return std::string(v);
        return {};
    }

    static inline std::string joinPath(const std::string& a, const std::string& b){
        if(a.empty()) return b;
        if(b.empty()) return a;
        return (std::filesystem::path(a) / std::filesystem::path(b)).string();
    }

    static inline std::string appSubdir(const std::string& base){
        return joinPath(base, "mcsm");
    }

    static inline std::string dataRoot(){
        switch (mcsm::getCurrentOS()){
            case mcsm::OS::WINDOWS: {
                std::string base = getEnvStr("LOCALAPPDATA");
                if(base.empty()){
                    const std::string up = getEnvStr("USERPROFILE");
                    if (!up.empty()) base = joinPath(up, "AppData/Local");
                }
                if(base.empty()) base = ".";
                return base;
            }
            case mcsm::OS::LINUX: {
                std::string xdg = getEnvStr("XDG_DATA_HOME");
                if(xdg.empty()){
                    const std::string home = getEnvStr("HOME");
                    xdg = home.empty() ? std::string(".") : joinPath(home, ".local/share");
                }
                return xdg;
            }
            case mcsm::OS::MAC_OS: {
                std::string val;
                const std::string home = getEnvStr("HOME");
                val = home.empty() ? std::string(".") : joinPath(home, "Library/Application Support");
                return val;
            }
        }
        return ".";
    }

    static inline std::string configRoot(){
        switch (mcsm::getCurrentOS()){
            case mcsm::OS::WINDOWS: {
                std::string base = getEnvStr("APPDATA");
                if(base.empty()){
                    const std::string up = getEnvStr("USERPROFILE");
                    if (!up.empty()) base = joinPath(up, "AppData/Roaming");
                }
                if(base.empty()) base = ".";
                return base;
            }
            case mcsm::OS::LINUX: {
                std::string xdg = getEnvStr("XDG_CONFIG_HOME");
                if(xdg.empty()){
                    const std::string home = getEnvStr("HOME");
                    xdg = home.empty() ? std::string(".") : joinPath(home, ".config");
                }
                return xdg;
            }
            case mcsm::OS::MAC_OS: {
                std::string val;
                const std::string home = getEnvStr("HOME");
                val = home.empty() ? std::string(".") : joinPath(home, "Library/Application Support");
                return val;
            }
        }
        return ".";
    }


    std::string getDataPathPerOS();
    std::string getConfigPathPerOS();

    std::string asGlobalDataPath(const std::string &value);
    std::string asGlobalConfigPath(const std::string &value);

    mcsm::BoolResult ensureDataDir();
    mcsm::BoolResult ensureConfigDir();

    mcsm::StringResult getExecutablePath();
}

#endif // __MCSM_CLI_UTILS_H__