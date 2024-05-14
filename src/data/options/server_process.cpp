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

#include <mcsm/data/options/server_process.h>
#include <fcntl.h> // For _O_WRONLY and other flags
#include <io.h>

mcsm::ServerProcess::ServerProcess(const std::string& command, const std::string& workingPath){
    this->command = command;
    this->workingPath = workingPath;
}

mcsm::ServerProcess::~ServerProcess(){}

#ifdef _WIN32
mcsm::Result mcsm::ServerProcess::start() {
    STARTUPINFOW si; // Notice the 'W' for wide characters
    PROCESS_INFORMATION pi;
    HANDLE hInputWrite, hInputReadTmp, hInputRead, hInputWriteTmp;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Create a pipe for the child process's input.
    if (!CreatePipe(&hInputReadTmp, &hInputWrite, NULL, 0)) {
        return mcsm::Result({mcsm::ResultType::MCSM_FAIL, {"Pipe failed."}});
    }
    if (!DuplicateHandle(GetCurrentProcess(), hInputReadTmp, GetCurrentProcess(),
                         &hInputRead, 0, FALSE, DUPLICATE_SAME_ACCESS)) {
        CloseHandle(hInputReadTmp);
        CloseHandle(hInputWrite);
        return mcsm::Result({mcsm::ResultType::MCSM_FAIL, {"DuplicateHandle failed."}});
    }
    CloseHandle(hInputReadTmp);

    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdInput = hInputRead;
    si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    si.hStdError = GetStdHandle(STD_ERROR_HANDLE);

    // Convert command to a modifiable array for CreateProcessW
    std::wstring wCommand = std::wstring(command.begin(), command.end());
    wchar_t *cmd = _wcsdup(wCommand.c_str());

    // Start the child process.
    if (!CreateProcessW(NULL, cmd, NULL, NULL, TRUE, 0, NULL,
                        workingPath.empty() ? NULL : std::wstring(workingPath.begin(), workingPath.end()).c_str(), &si, &pi)) {
        DWORD errorCode = GetLastError();
        CloseHandle(hInputRead);
        CloseHandle(hInputWrite);
        free(cmd);
        return mcsm::Result({mcsm::ResultType::MCSM_FAIL, {"CreateProcess failed. Error code: " + std::to_string(errorCode)}});
    }

    this->pid = pi.dwProcessId;
    this->inputFd = _open_osfhandle((intptr_t)hInputWrite, _O_WRONLY);
    this->active = true;
    this->pi = pi;
    CloseHandle(hInputRead);
    free(cmd);

    return mcsm::Result({mcsm::ResultType::MCSM_SUCCESS, {"Starting process with pid " + std::to_string(pid)}});
}
#else
mcsm::Result mcsm::ServerProcess::start() {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        return mcsm::Result({mcsm::ResultType::MCSM_FAIL, {"Pipe failed."}});
    }

    pid_t pid = fork();
    if (pid == -1) {
        close(pipefd[0]);
        close(pipefd[1]);
        return mcsm::Result({mcsm::ResultType::MCSM_FAIL, {"Fork failed."}});
    } else if (pid > 0) {
        close(pipefd[0]); // Close the read end in the parent
        this->pid = pid;
        this->inputFd = pipefd[1];
        this->active = true;
        return mcsm::Result({mcsm::ResultType::MCSM_SUCCESS, {"Starting process with pid " + std::to_string(pid)}});
    } else {
        close(pipefd[1]); // Close the write end in the child
        dup2(pipefd[0], STDIN_FILENO); // Redirect standard input to the read end of the pipe
        close(pipefd[0]);

        if (!workingPath.empty() && chdir(workingPath.c_str()) != 0) {
            exit(2);
        }

        execl("/bin/sh", "sh", "-c", command.c_str(), (char *) NULL);
        exit(3); // execl only returns on failure
    }
}
#endif
int mcsm::ServerProcess::getPID() const {
    return this->pid;
}

int mcsm::ServerProcess::getInputFd() const {
    return this->inputFd;
}

bool mcsm::ServerProcess::isActivate() const {
    return this->active;
}

mcsm::Result mcsm::ServerProcess::send(const std::string& input){

}

mcsm::Result mcsm::ServerProcess::stop(){

}