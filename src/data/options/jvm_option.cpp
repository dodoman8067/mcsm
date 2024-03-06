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

#include <mcsm/data/options/jvm_option.h>

mcsm::JvmOption::JvmOption(const std::string& name) : JvmOption(name, mcsm::SearchTarget::ALL) {}

mcsm::JvmOption::JvmOption(const std::string& name, const mcsm::SearchTarget& target){
    this->name = name;
    switch (target){
        case mcsm::SearchTarget::ALL: {
            mcsm::GlobalOption globalOption("/jvm/profiles", name);
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return;
            if(globalOption.exists()){
                auto optPtr = new mcsm::GlobalOption("/jvm/profiles", name);
                if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return;
                this->option.reset(optPtr);
            }else{
                this->option.reset(new mcsm::Option(mcsm::getCurrentPath() + "/.mcsm/jvm/profiles", name));
            }
            break;
        }
        case mcsm::SearchTarget::GLOBAL: {
            mcsm::GlobalOption globalOption("/jvm/profiles", name);
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return;
            if(globalOption.exists()){
                auto optPtr = new mcsm::GlobalOption("/jvm/profiles", name);
                if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return;
                this->option.reset(optPtr);
            }
            break;
        }
        case mcsm::SearchTarget::CURRENT: {
            mcsm::Option option(mcsm::getCurrentPath() + "/.mcsm/jvm/profiles", name);
            if(option.exists()){
                this->option.reset(new mcsm::Option(mcsm::getCurrentPath() + "/.mcsm/jvm/profiles", name));
            }
            break;
        }
    }
    mcsm::Result res({mcsm::ResultType::MCSM_OK, {"Success"}});
}

mcsm::Result mcsm::JvmOption::create(){
    std::string jvm = mcsm::detectJava();
    std::vector<std::string> jvmArgs = {
        "-Xms2G",
        "-Xmx2G",
        "-jar"
    };
    std::vector<std::string> serverArgs = {
        "nogui"
    };
    if(mcsm::isWhitespaceOrEmpty(jvm)){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jvmDetectionFailed()});
        return res;
    }
    return create(jvm, jvmArgs, serverArgs, mcsm::SearchTarget::GLOBAL);
}

mcsm::Result mcsm::JvmOption::create(const std::string& jvmPath, const mcsm::SearchTarget& target){
    std::vector<std::string> jvmArgs = {
        "-Xms2G",
        "-Xmx2G",
        "-jar"
    };
    std::vector<std::string> serverArgs = {
        "nogui"
    };
    return create(jvmPath, jvmArgs, serverArgs, target);
}

mcsm::Result mcsm::JvmOption::create(const std::string& jvmPath, const std::vector<std::string>& jvmOptions, const mcsm::SearchTarget& target){
    std::vector<std::string> serverArgs = {
        "nogui"
    };
    return create(jvmPath, jvmOptions, serverArgs, target);
}

mcsm::Result mcsm::JvmOption::create(const std::string& jvmPath, const std::vector<std::string>& jvmOptions, const std::vector<std::string>& serverOptions, const mcsm::SearchTarget& target){
    std::string filePath;
    std::string optionName;

    if(target == mcsm::SearchTarget::CURRENT){
        mcsm::Option option(mcsm::getCurrentPath() + "/.mcsm/jvm/profiles", this->name);
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
            std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
            mcsm::Result res(resp.first, resp.second);
            return res;
        }
        filePath = option.getPath();
        optionName = option.getName();
    }else{
        mcsm::GlobalOption globalOption("./jvm/profiles", this->name);
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
            std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
            mcsm::Result res(resp.first, resp.second);
            return res;
        }
        filePath = globalOption.getPath();
        optionName = globalOption.getName();
    }

    bool fileExists = mcsm::fileExists(filePath + "/" + optionName);
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }
    if(fileExists){
        mcsm::removeFile(filePath + "/" + optionName);
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
            std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
            mcsm::Result res(resp.first, resp.second);
            return res;
        }
    }

    mcsm::Result invaildPath({mcsm::ResultType::MCSM_SUCCESS, {"Invaild jvm path.", "High chance to be a software issue, please report this to GitHub (https://github.com/dodoman8067/mcsm)."}});
    if(target == mcsm::SearchTarget::ALL) return invaildPath;
    if(target == mcsm::SearchTarget::CURRENT){
        mcsm::Option* cOpt = new mcsm::Option(mcsm::getCurrentPath() + "/.mcsm/jvm/profiles", optionName);
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
            std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
            mcsm::Result res(resp.first, resp.second);
            return res;
        }
        auto res1 = cOpt->setValue("path", jvmPath);
        if(!res1.isSuccess()) return res1;

        auto res2 = cOpt->setValue("args", jvmOptions);
        if(!res2.isSuccess()) return res2;

        auto res3 = cOpt->setValue("server_args", serverOptions);
        if(!res3.isSuccess()) return res3;
        
        this->option.reset(cOpt);
        
        mcsm::Result res({mcsm::ResultType::MCSM_OK, {"Success"}});
        return res;
    }
    mcsm::GlobalOption* opt = new mcsm::GlobalOption("/jvm/profiles", optionName);
    auto res1 = opt->setValue("path", jvmPath);
    if(!res1.isSuccess()) return res1;

    auto res2 = opt->setValue("args", jvmOptions);
    if(!res2.isSuccess()) return res2;

    auto res3 = opt->setValue("server_args", serverOptions);
    if(!res3.isSuccess()) return res3;
    
    this->option.reset(opt);

    mcsm::Result res({mcsm::ResultType::MCSM_OK, {"Success"}});
    return res;
}

mcsm::Result mcsm::JvmOption::reset(){
    if(this->option->isGlobal()){
        mcsm::GlobalOption* opt = static_cast<mcsm::GlobalOption*>(this->option.get());
        return opt->reset();
    }else{
        mcsm::Option* opt2 = static_cast<mcsm::Option*>(this->option.get());
        return opt2->reset();
    }
}

bool mcsm::JvmOption::exists(){
    if(this->option.get() == nullptr) return false;
    return this->option->exists();
}

std::vector<std::string> mcsm::JvmOption::getJvmArguments(){
    bool profileExists = exists();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return {};
    if(!profileExists){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jvmProfileNotFound()});
        return {};
    }
    if(this->option->isGlobal()){
        mcsm::GlobalOption* opt = static_cast<mcsm::GlobalOption*>(this->option.get());
        
        nlohmann::json value = opt->getValue("args");
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return {};
        
        if(value == nullptr){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFound("\"args\"", opt->getName())});
            return {};
        }
        if(!value.is_array()){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"args\"", "array of string")});
            return {};
        }
        for(auto& v : value){
            if(!v.is_string()){
                mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"args\"", "array of string")});
                return {};
            }
        }
        mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
        std::vector<std::string> args = value;
        return args;
    }else{
        mcsm::Option* opt2 = static_cast<mcsm::Option*>(this->option.get());
        
        nlohmann::json value = opt2->getValue("args");
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return {};
        
        if(value == nullptr){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFound("\"args\"", opt2->getName())});
            return {};
        }
        if(!value.is_array()){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"args\"", "array of string")});
            return {};
        }
        for(auto& v : value){
            if(!v.is_string()){
                mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"args\"", "array of string")});
                return {};
            }
        }
        mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
        std::vector<std::string> args = value;
        return args;
    }
}

mcsm::Result mcsm::JvmOption::setJvmArguments(const std::vector<std::string>& jvmArgs){
    bool profileExists = exists();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        mcsm::Result res({mcsm::getLastResult().first, mcsm::getLastResult().second});
        return res;
    }
    if(!profileExists){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jvmProfileNotFound()});
        return res;
    }
    if(this->option->isGlobal()){
        mcsm::GlobalOption* opt = static_cast<mcsm::GlobalOption*>(this->option.get());
        return opt->setValue("args", jvmArgs);
    }else{
        mcsm::Option* opt2 = static_cast<mcsm::Option*>(this->option.get());
        return opt2->setValue("args", jvmArgs);
    }
}

std::string mcsm::JvmOption::getJvmPath(){
    bool profileExists = exists();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";
    if(!profileExists){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jvmProfileNotFound()});
        return "";
    }
    if(this->option->isGlobal()){
        mcsm::GlobalOption* opt = static_cast<mcsm::GlobalOption*>(this->option.get());
        
        nlohmann::json value = opt->getValue("path");
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";
        
        if(value == nullptr){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFound("\"path\"", opt->getName())});
            return "";
        }
        if(!value.is_string()){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"path\"", "string")});
            return "";
        }
        mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
        std::string path = value;
        return path;
    }else{
        mcsm::Option* opt2 = static_cast<mcsm::Option*>(this->option.get());
        
        nlohmann::json value = opt2->getValue("path");
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return {};
        
        if(value == nullptr){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFound("\"path\"", opt2->getName())});
            return "";
        }
        if(!value.is_string()){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"path\"", "string")});
            return "";
        }
        mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
        std::string path = value;
        return path;
    }
}

mcsm::Result mcsm::JvmOption::setJvmPath(const std::string& jvmPath){
    bool profileExists = exists();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        mcsm::Result res({mcsm::getLastResult().first, mcsm::getLastResult().second});
        return res;
    }
    if(!profileExists){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jvmProfileNotFound()});
        return res;
    }
    if(this->option->isGlobal()){
        mcsm::GlobalOption* opt = static_cast<mcsm::GlobalOption*>(this->option.get());
        return opt->setValue("path", jvmPath);
    }else{
        mcsm::Option* opt2 = static_cast<mcsm::Option*>(this->option.get());
        return opt2->setValue("path", jvmPath);
    }
}

std::vector<std::string> mcsm::JvmOption::getServerArguments(){
    bool profileExists = exists();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return {};
    if(!profileExists){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jvmProfileNotFound()});
        return {};
    }
    if(this->option->isGlobal()){
        mcsm::GlobalOption* opt = static_cast<mcsm::GlobalOption*>(this->option.get());
        
        nlohmann::json value = opt->getValue("server_args");
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return {};
        
        if(value == nullptr){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFound("\"server_args\"", opt->getName())});
            return {};
        }
        if(!value.is_array()){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"server_args\"", "array of string")});
            return {};
        }
        for(auto& v : value){
            if(!v.is_string()){
                mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"server_args\"", "array of string")});
                return {};         
            }
        }
        mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
        std::vector<std::string> args = value;
        return args;
    }else{
        mcsm::Option* opt2 = static_cast<mcsm::Option*>(this->option.get());
        
        nlohmann::json value = opt2->getValue("server_args");
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return {};
        
        if(value == nullptr){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFound("\"server_args\"", opt2->getName())});
            return {};
        }
        if(!value.is_array()){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"server_args\"", "array of string")});
            return {};
        }
        for(auto& v : value){
            if(!v.is_string()){
                mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"server_args\"", "array of string")});
                return {};
            }
        }
        mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
        std::vector<std::string> args = value;
        return args;
    }
}

mcsm::Result mcsm::JvmOption::setServerArguments(const std::vector<std::string>& serverArgs){
    bool profileExists = exists();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        mcsm::Result res({mcsm::getLastResult().first, mcsm::getLastResult().second});
        return res;
    }
    if(!profileExists){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jvmProfileNotFound()});
        return res;
    }
    if(this->option->isGlobal()){
        mcsm::GlobalOption* opt = static_cast<mcsm::GlobalOption*>(this->option.get());
        return opt->setValue("server_args", serverArgs);
    }else{
        mcsm::Option* opt2 = static_cast<mcsm::Option*>(this->option.get());
        return opt2->setValue("server_args", serverArgs);
    }
}

std::string mcsm::JvmOption::getProfileName() const {
    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
    return this->name;
}

std::string mcsm::JvmOption::getProfilePath() const {
    if(this->option->isGlobal()){
        mcsm::GlobalOption* opt = static_cast<mcsm::GlobalOption*>(this->option.get());
        mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
        return opt->getPath();
    }else{
        mcsm::Option* opt2 = static_cast<mcsm::Option*>(this->option.get());
        mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
        return opt2->getPath();
    }
}

mcsm::SearchTarget mcsm::JvmOption::getSearchTarget() const {
    if(this->option->isGlobal()){
        mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
        return mcsm::SearchTarget::GLOBAL;
    }else{
        mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
        return mcsm::SearchTarget::CURRENT;
    }
}

mcsm::JvmOption::~JvmOption(){

}
