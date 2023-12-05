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

#include <mcsm/data/options/server_data_option.h>

mcsm::ServerDataOption::ServerDataOption(){
    this->option = std::make_unique<mcsm::Option>("./.mcsm/", "server_datas");
}

mcsm::ServerDataOption::~ServerDataOption(){

}

void mcsm::ServerDataOption::create(const std::string& lastTimeLaunched){
    if(this->option->exists()){
        mcsm::error("Server data config already in place.");
        mcsm::error("Task aborted.");
        std::exit(1);
    }
    this->option->setValue("last_time_launched", lastTimeLaunched);
}

void mcsm::ServerDataOption::reset(){
    this->option->reset();
}

std::string mcsm::ServerDataOption::getLastTimeLaunched() const {
    if(!this->option->exists()){
        mcsm::error("Option does not exist; Task aborted.");
        std::exit(1);
    }
    if(this->option->getValue("last_time_launched") == nullptr){
        mcsm::error("No \"last_time_launched\" value specified in file " + this->option->getName());
        mcsm::error("Manually editing the launch profile might have caused this issue.");
        mcsm::error("If you know what you're doing, I believe you that you know how to handle this issue.");
        mcsm::error("If you believe that this is a software issue, please report it to GitHub (https://github.com/dodoman8067/mcsm).");
        std::exit(1);
    }
    return this->option->getValue("last_time_launched");
}

void mcsm::ServerDataOption::updateLastTimeLaunched(){
    char buffer[80];

    auto currentTimePoint = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(currentTimePoint);

    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&currentTime));

    this->option->setValue("last_time_launched", buffer);
}

std::string mcsm::ServerDataOption::getFirstTimeLaunched() const {
    if(!this->option->exists()){
        mcsm::error("Option does not exist; Task aborted.");
        std::exit(1);
    }
    if(this->option->getValue("first_time_launched") == nullptr){
        mcsm::error("No \"first_time_launched\" value specified in file " + this->option->getName());
        mcsm::error("Manually editing the launch profile might have caused this issue.");
        mcsm::error("If you know what you're doing, I believe you that you know how to handle this issue.");
        mcsm::error("If you believe that this is a software issue, please report it to GitHub (https://github.com/dodoman8067/mcsm).");
        std::exit(1);
    }
    return this->option->getValue("first_time_launched");
}

void mcsm::ServerDataOption::updateFirstTimeLaunched(){
    char buffer[80];

    auto currentTimePoint = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(currentTimePoint);

    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&currentTime));

    this->option->setValue("first_time_launched", buffer);
}

std::string mcsm::ServerDataOption::getServerTimeCreated() const {
    if(!this->option->exists()){
        mcsm::error("Option does not exist; Task aborted.");
        std::exit(1);
    }
    if(this->option->getValue("server_time_created") == nullptr){
        mcsm::error("No \"server_time_created\" value specified in file " + this->option->getName());
        mcsm::error("Manually editing the launch profile might have caused this issue.");
        mcsm::error("If you know what you're doing, I believe you that you know how to handle this issue.");
        mcsm::error("If you believe that this is a software issue, please report it to GitHub (https://github.com/dodoman8067/mcsm).");
        std::exit(1);
    }
    return this->option->getValue("server_time_created");
}

void mcsm::ServerDataOption::updateServerTimeCreated(){
    char buffer[80];

    auto currentTimePoint = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(currentTimePoint);

    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&currentTime));

    this->option->setValue("server_time_created", buffer);
}

bool mcsm::ServerDataOption::exists() const {
    return this->option->exists();
}