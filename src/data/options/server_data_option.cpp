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
#include <mcsm/data/options/general_option.h>

mcsm::ServerDataOption::ServerDataOption() : ServerDataOption(mcsm::getCurrentPath().value()){}

mcsm::ServerDataOption::ServerDataOption(const std::string& path){
    this->path = path;
}

mcsm::ServerDataOption::~ServerDataOption(){

}

mcsm::VoidResult mcsm::ServerDataOption::load(){
    if(this->loaded){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::SERVER_DATA_ALREADY_CONFIGURED, {});
        return tl::unexpected(err);
    }
    auto fileExists = mcsm::fileExists(path);
    if(!fileExists) return tl::unexpected(fileExists.error());

    if(!fileExists.value()){
        if(!mcsm::mkdir(path)){
            mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::FILE_CREATE_FAILED, {path});
            return tl::unexpected(err);
        }
    }
    this->option = std::make_unique<mcsm::Option>(path + "/.mcsm/", "server_datas");

    bool advp = mcsm::GeneralOption::getGeneralOption().advancedParseEnabled();

    auto optExists = this->option->exists();
    if(!optExists) return tl::unexpected(optExists.error());
    if(optExists.value()) {
        auto optLRes = this->option->load(advp);
        if(!optLRes) return optLRes;
    }
    
    this->loaded = true;
    return {};
}

mcsm::VoidResult mcsm::ServerDataOption::load(const bool& advp){
    if(this->loaded){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::SERVER_DATA_ALREADY_CONFIGURED, {});
        return tl::unexpected(err);
    }
    auto fileExists = mcsm::fileExists(path);
    if(!fileExists) return tl::unexpected(fileExists.error());

    if(!fileExists.value()){
        if(!mcsm::mkdir(path)){
            mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::FILE_CREATE_FAILED, {path});
            return tl::unexpected(err);
        }
    }
    this->option = std::make_unique<mcsm::Option>(path + "/.mcsm/", "server_datas");

    auto optExists = this->option->exists();
    if(!optExists) return tl::unexpected(optExists.error());
    if(optExists.value()) {
        auto optLRes = this->option->load(advp);
        if(!optLRes) return optLRes;
    }

    this->loaded = true;
    return {};
}

mcsm::VoidResult mcsm::ServerDataOption::create(const std::string& lastTimeLaunched){
    if(!mcsm::isSafeString(lastTimeLaunched)){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::UNSAFE_STRING, {lastTimeLaunched});
        return tl::unexpected(err);
    }
    auto optExists = this->option->exists();
    if(!optExists) return tl::unexpected(optExists.error());
    if(optExists.value()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::SERVER_DATA_ALREADY_CONFIGURED, {this->option->getPath()});
        return tl::unexpected(err);
    }

    bool advp = mcsm::GeneralOption::getGeneralOption().advancedParseEnabled();

    mcsm::VoidResult sLoadRes = load(advp);
    if(!sLoadRes) return sLoadRes;

    mcsm::VoidResult res1 = this->option->setValue("last_time_launched", lastTimeLaunched);
    if(!res1) return res1;
    mcsm::VoidResult res2 = this->option->setValue("last_downloaded_build", "0");
    if(!res2) return res2;

    return this->option->save();
}

mcsm::VoidResult mcsm::ServerDataOption::reset(){
    return this->option->reset();
}

mcsm::StringResult mcsm::ServerDataOption::getLastTimeLaunched() const {
    auto optExists = this->option->exists();
    if(!optExists) return tl::unexpected(optExists.error());
    if(!optExists.value()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::SERVER_DATA_NOT_CONFIGURED, {this->option->getPath()});
        return tl::unexpected(err);
    }

    auto valueRes = this->option->getValue("last_time_launched");
    if(!valueRes) return tl::unexpected(valueRes.error());
    const nlohmann::json& value = valueRes.value();

    if(value == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_NOT_FOUND, {"\"last_time_launched\"", this->option->getName()});
        return tl::unexpected(err);
    }
    if(!value.is_string()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_NOT_FOUND, {"\"last_time_launched\"", "string"});
        return tl::unexpected(err);
    }

    if(!mcsm::isSafeString(value)){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::UNSAFE_STRING, {value});
        return tl::unexpected(err);
    }

    return value;
}

mcsm::VoidResult mcsm::ServerDataOption::updateLastTimeLaunched(){
    char buffer[80];

    auto currentTimePoint = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(currentTimePoint);

    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&currentTime));

    mcsm::VoidResult setRes = this->option->setValue("last_time_launched", buffer);
    if(!setRes) return setRes;

    return this->option->save();
}

mcsm::StringResult mcsm::ServerDataOption::getServerTimeCreated() const {
    auto optExists = this->option->exists();
    if(!optExists) return tl::unexpected(optExists.error());
    if(!optExists.value()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::SERVER_DATA_NOT_CONFIGURED, {this->option->getPath()});
        return tl::unexpected(err);
    }

    auto valueRes = this->option->getValue("server_time_created");
    if(!valueRes) return tl::unexpected(valueRes.error());
    const nlohmann::json& value = valueRes.value();

    if(value == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_NOT_FOUND, {"\"server_time_created\"", this->option->getName()});
        return tl::unexpected(err);
    }
    if(!value.is_string()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_WRONG_TYPE, {"\"server_time_created\"", "string"});
        return tl::unexpected(err);
    }

    if(!mcsm::isSafeString(value)){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::UNSAFE_STRING, {value});
        return tl::unexpected(err);
    }

    return value;
}

mcsm::VoidResult mcsm::ServerDataOption::updateServerTimeCreated(){
    char buffer[80];

    auto currentTimePoint = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(currentTimePoint);

    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&currentTime));

    mcsm::VoidResult setRes = this->option->setValue("server_time_created", buffer);
    if(!setRes) return setRes;

    return this->option->save();
}

mcsm::StringResult mcsm::ServerDataOption::getLastDownloadedBuild() const {
    auto optExists = this->option->exists();
    if(!optExists) return tl::unexpected(optExists.error());
    if(!optExists.value()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::SERVER_DATA_NOT_CONFIGURED, {this->option->getPath()});
        return tl::unexpected(err);
    }

    auto valueRes = this->option->getValue("last_downloaded_build");
    if(!valueRes) return tl::unexpected(valueRes.error());
    const nlohmann::json& value = valueRes.value();

    if(value == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_NOT_FOUND, {"\"last_downloaded_build\"", this->option->getName()});
        return tl::unexpected(err);
    }
    if(!value.is_string()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_WRONG_TYPE, {"\"last_downloaded_build\"", "string"});
        return tl::unexpected(err);
    }

    if(!mcsm::isSafeString(value)){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::UNSAFE_STRING, {value});
        return tl::unexpected(err);
    }

    return value;
}

mcsm::VoidResult mcsm::ServerDataOption::updateLastDownloadedBuild(const std::string& build){
    if(!mcsm::isSafeString(build)){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::UNSAFE_STRING, {build});
        return tl::unexpected(err);
    }
    mcsm::VoidResult setRes = this->option->setValue("last_downloaded_build", build);
    if(!setRes) return setRes;

    return this->option->save();
}

mcsm::BoolResult mcsm::ServerDataOption::exists() const {
    return this->option->exists();
}