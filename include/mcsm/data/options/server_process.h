/*
MIT License

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

#ifndef __MCSM_SERVER_PROCESS_H__
#define __MCSM_SERVER_PROCESS_H__

#include <mcsm/data/option.h>
#include <mcsm/util/cli/cli_utils.h>
#include <chrono>
#include <thread>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
    #include <sys/wait.h>
    #include <signal.h>
    #include <fcntl.h>
#endif

namespace mcsm {
    class ServerProcess {
    private:
        std::string command;
        int inputFd = -1;
        int pid = -1;
        bool active = false;
    public:
        ServerProcess();
        ~ServerProcess();

        void startServer(const std::string& command, const std::string& workingPath);

        int getPID() const;
        int getInputFd() const;

        bool isActivate() const;

        void send(const std::string& input);
        void stop();
    };

    class ServerProcessManager {

    };
}

#endif // __MCSM_SERVER_PROCESS_H__