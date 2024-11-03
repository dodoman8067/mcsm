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

mcsm::ServerDataOption::ServerDataOption() : ServerDataOption(mcsm::getCurrentPath()){}

mcsm::ServerDataOption::ServerDataOption(const std::string& path){
    bool fileExists = mcsm::fileExists(path);
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return;

    if(!fileExists){
        if(!mcsm::mkdir(path)){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::fileCreateFailed(path)});
            return;
        }
    }
    this->option = std::make_unique<mcsm::Option>(path + "/.mcsm/", "server_datas");

    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
}

mcsm::ServerDataOption::~ServerDataOption(){

}

mcsm::Result mcsm::ServerDataOption::load(){
    bool advp = mcsm::GeneralOption::getGeneralOption().advancedParseEnabled();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    return this->option->load(advp);
}

mcsm::Result mcsm::ServerDataOption::load(const bool& advp){
    return this->option->load(advp);
}

mcsm::Result mcsm::ServerDataOption::create(const std::string& lastTimeLaunched){
    if(!mcsm::isSafeString(lastTimeLaunched)){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::unsafeString(lastTimeLaunched)});
        return res;
    }
    bool optExists = this->option->exists();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }
    if(optExists){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverAlreadyConfigured(this->option->getPath())});
        return res;
    }

    bool advp = mcsm::GeneralOption::getGeneralOption().advancedParseEnabled();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    mcsm::Result sLoadRes = load(advp);
    if(!sLoadRes.isSuccess()) return sLoadRes;

    mcsm::Result res1 = this->option->setValue("last_time_launched", lastTimeLaunched);
    if(!res1.isSuccess()) return res1;
    mcsm::Result res2 = this->option->setValue("last_downloaded_build", "0");
    if(!res2.isSuccess()) return res2;

    return this->option->save();
}

mcsm::Result mcsm::ServerDataOption::reset(){
    return this->option->reset();
}

std::string mcsm::ServerDataOption::getLastTimeLaunched() const {
    bool optExists = this->option->exists();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";
    if(!optExists){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::fileNotFound(this->option->getName())});
        return "";
    }

    const nlohmann::json& value = this->option->getValue("last_time_launched");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";

    if(value == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFound("\"last_time_launched\"", this->option->getName())});
        return "";
    }
    if(!value.is_string()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"last_time_launched\"", "string")});
        return "";
    }

    if(!mcsm::isSafeString(value)){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::unsafeString(value)});
        return "";
    }

    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
    return value;
}

mcsm::Result mcsm::ServerDataOption::updateLastTimeLaunched(){
    char buffer[80];

    auto currentTimePoint = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(currentTimePoint);

    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&currentTime));

    mcsm::Result setRes = this->option->setValue("last_time_launched", buffer);
    if(!setRes.isSuccess()) return setRes;

    return this->option->save();
}

std::string mcsm::ServerDataOption::getServerTimeCreated() const {
    bool optExists = this->option->exists();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";
    if(!optExists){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::fileNotFound(this->option->getName())});
        return "";
    }

    const nlohmann::json& value = this->option->getValue("server_time_created");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";

    if(value == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFound("\"server_time_created\"", this->option->getName())});
        return "";
    }
    if(!value.is_string()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"server_time_created\"", "string")});
        return "";
    }

    if(!mcsm::isSafeString(value)){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::unsafeString(value)});
        return "";
    }

    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
    return value;
}

mcsm::Result mcsm::ServerDataOption::updateServerTimeCreated(){
    char buffer[80];

    auto currentTimePoint = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(currentTimePoint);

    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&currentTime));

    mcsm::Result setRes = this->option->setValue("server_time_created", buffer);
    if(!setRes.isSuccess()) return setRes;

    return this->option->save();
}

std::string mcsm::ServerDataOption::getLastDownloadedBuild() const {
    bool optExists = this->option->exists();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";
    if(!optExists){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::fileNotFound(this->option->getName())});
        return "";
    }

    const nlohmann::json& value = this->option->getValue("last_downloaded_build");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";

    if(value == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFound("\"last_downloaded_build\"", this->option->getName())});
        return "";
    }
    if(!value.is_string()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"last_downloaded_build\"", "string")});
        return "";
    }

    if(!mcsm::isSafeString(value)){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::unsafeString(value)});
        return "";
    }

    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
    return value;
}

mcsm::Result mcsm::ServerDataOption::updateLastDownloadedBuild(const std::string& build){
    if(!mcsm::isSafeString(build)){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::unsafeString(build)});
        return res;
    }
    mcsm::Result setRes = this->option->setValue("last_downloaded_build", build);
    if(!setRes.isSuccess()) return setRes;

    return this->option->save();
}

bool mcsm::ServerDataOption::exists() const {
    return this->option->exists();
}