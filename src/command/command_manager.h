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

#ifndef __MCSM_COMMAND_MANAGER_H__
#define __MCSM_COMMAND_MANAGER_H__

#include <curl/curl.h>
#include <memory>
#include <vector>
#include "base/command.h"
#include <iostream>

namespace mcsm {
    class CommandManager {
    private:
        static bool initialized;
        static std::unique_ptr<std::vector<std::unique_ptr<mcsm::Command>>> commands;
        CommandManager();
    public:
        ~CommandManager();
        static void init();
        static std::vector<std::unique_ptr<mcsm::Command>>& getCommands();
        static void addCommand(std::unique_ptr<mcsm::Command> command);
        static bool hasCommand(const std::string& name);
        static bool hasAliases(const std::string& command, const std::string& value);
        static bool hasAliasesInGlobal(const std::string& value);
        static std::unique_ptr<mcsm::Command> getCommand(const std::string& name);
    };
};

#endif