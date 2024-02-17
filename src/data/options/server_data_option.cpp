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

mcsm::ServerDataOption::ServerDataOption() : ServerDataOption(mcsm::getCurrentPath()){}

mcsm::ServerDataOption::ServerDataOption(const std::string& path){
    if(!mcsm::fileExists(path)){
        if(!mcsm::mkdir(path)){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::fileCreateFailed(path)});
            return;
        }
    }
    this->option = std::make_unique<mcsm::Option>(path + "/.mcsm/", "server_datas");
}

mcsm::ServerDataOption::~ServerDataOption(){

}

mcsm::Result mcsm::ServerDataOption::create(const std::string& lastTimeLaunched){
    if(this->option->exists()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverDataAlreadyConfigured()});
    }
    return this->option->setValue("last_time_launched", lastTimeLaunched);
}

mcsm::Result mcsm::ServerDataOption::reset(){
    return this->option->reset();
}

std::string mcsm::ServerDataOption::getLastTimeLaunched() const {
    if(!this->option->exists()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::fileNotFound(this->option->getName())});
        return "";
    }
    if(this->option->getValue("last_time_launched") == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFound("\"last_time_launched\"", this->option->getName())});
        return "";
    }
    if(!this->option->getValue("last_time_launched").is_string()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"last_time_launched\"", "string")});
        return "";
    }
    return this->option->getValue("last_time_launched");
}

mcsm::Result mcsm::ServerDataOption::updateLastTimeLaunched(){
    char buffer[80];

    auto currentTimePoint = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(currentTimePoint);

    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&currentTime));

    return this->option->setValue("last_time_launched", buffer);
}

std::string mcsm::ServerDataOption::getServerTimeCreated() const {
    if(!this->option->exists()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::fileNotFound(this->option->getName())});
        return "";
    }
    if(this->option->getValue("server_time_created") == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFound("\"server_time_created\"", this->option->getName())});
        return "";
    }
    if(!this->option->getValue("server_time_created").is_string()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"server_time_created\"", "string")});
        return "";
    }
    return this->option->getValue("server_time_created");
}

mcsm::Result mcsm::ServerDataOption::updateServerTimeCreated(){
    char buffer[80];

    auto currentTimePoint = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(currentTimePoint);

    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&currentTime));

    return this->option->setValue("server_time_created", buffer);
}

std::string mcsm::ServerDataOption::getLastDownloadedBuild() const {
    if(!this->option->exists()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::fileNotFound(this->option->getName())});
        return "";
    }
    if(this->option->getValue("last_downloaded_build") == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFound("\"last_downloaded_build\"", this->option->getName())});
        return "";
    }
    if(!this->option->getValue("last_downloaded_build").is_string()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"last_downloaded_build\"", "string")});
        return "";
    }
    return this->option->getValue("last_downloaded_build");
}

mcsm::Result mcsm::ServerDataOption::updateLastDownloadedBuild(const std::string& build){
    return this->option->setValue("last_downloaded_build", build);
}

bool mcsm::ServerDataOption::exists() const {
    return this->option->exists();
}