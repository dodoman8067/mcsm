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

mcsm::ServerProcess::ServerProcess(const std::string& command, const std::string& workingPath){
    this->command = command;
    this->workingPath = workingPath;
}

mcsm::ServerProcess::~ServerProcess(){}

mcsm::Result mcsm::ServerProcess::start(){

}

int mcsm::ServerProcess::getPID() const {

}

int mcsm::ServerProcess::getInputFd() const {

}

bool mcsm::ServerProcess::isActivate() const {

}

mcsm::Result mcsm::ServerProcess::send(const std::string& input){

}

mcsm::Result mcsm::ServerProcess::stop(){

}