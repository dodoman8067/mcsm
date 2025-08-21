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

#include <mcsm/jvm/jvm_option.h>
#include <mcsm/data/options/general_option.h>

#include <memory>

mcsm::JvmOption::JvmOption(const std::string& name) : JvmOption(name, mcsm::SearchTarget::ALL, mcsm::getCurrentPath().value()) {}

mcsm::JvmOption::JvmOption(const std::string& name, const mcsm::SearchTarget& target) : JvmOption(name, target, mcsm::getCurrentPath().value()) {}

mcsm::JvmOption::JvmOption(const std::string& name, const mcsm::SearchTarget& target, const std::string& workingPath){
    this->name = name;
    this->workingDir = workingPath;
    this->target = target;
}

mcsm::VoidResult mcsm::JvmOption::init(){
    if(this->initialized) {
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::INTERNAL_FUNC_EXECUTION_FAILED, {"mcsm::JvmOption::init()", "Called more than once"});
        return tl::unexpected(err);
    }
    if(!mcsm::isSafeString(this->name)){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::UNSAFE_STRING, {name});
        return tl::unexpected(err);
    }
    switch (this->target){
        case mcsm::SearchTarget::ALL: {
            mcsm::Option globalOption(mcsm::asGlobalDataPath("/jvm/profiles"), name);
            auto exists = globalOption.exists();
            if(!exists) return tl::unexpected(exists.error());
            if(exists.value()){
                // this->option->load will always create an empty "{}" file regardless of it existing which would result in an error in many jvm generation related command executions.
                // as jvmoption doesnt provide a way to load its internal option pointer we would load it when the file is present
                this->option = std::make_unique<mcsm::Option>(globalOption);
                auto lRes = this->option->load(mcsm::GeneralOption::getGeneralOption().advancedParseEnabled());
                if(!lRes) return lRes;
                this->target = mcsm::SearchTarget::GLOBAL;
            }else{
                this->option = std::make_unique<mcsm::Option>(this->workingDir + "/.mcsm/jvm/profiles", name);
                this->target = mcsm::SearchTarget::CURRENT;
            }
            break;
        }
        case mcsm::SearchTarget::GLOBAL: {
            this->option = std::make_unique<mcsm::Option>(mcsm::asGlobalDataPath("/jvm/profiles"), name);
            // this->option->load will always create an empty "{}" file regardless of it existing which would result in an error in many jvm generation related command executions.
            // as jvmoption doesnt provide a way to load its internal option pointer we would load it when the file is present
            auto exists = this->option->exists();
            if(!exists) return tl::unexpected(exists.error());
            if(exists.value()){
                auto lRes = this->option->load(mcsm::GeneralOption::getGeneralOption().advancedParseEnabled());
                if(!lRes) return lRes;
            }
            break;
        }
        case mcsm::SearchTarget::CURRENT: {
            this->option = std::make_unique<mcsm::Option>(this->workingDir + "/.mcsm/jvm/profiles", name);
            auto exists = this->option->exists();
            if(!exists) return tl::unexpected(exists.error());
            if(exists.value()){
                // this->option->load will always create an empty "{}" file regardless of it existing which would result in an error in many jvm generation related command executions.
                // as jvmoption doesnt provide a way to load its internal option pointer we would load it when the file is present
                auto lRes = this->option->load(mcsm::GeneralOption::getGeneralOption().advancedParseEnabled());
                if(!lRes) return lRes;
            }
            break;
        }
    }
    initialized = true;
    return {};
}

mcsm::VoidResult mcsm::JvmOption::create(){
    if(!this->initialized) {
        auto customTemp = mcsm::errors::INTERNAL_FUNC_EXECUTION_FAILED;
        customTemp.message = "JvmOption function called without load.";
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, customTemp, {});
        return tl::unexpected(err);
    }
    auto jvm = mcsm::detectJava();
    if(!jvm) return tl::unexpected(jvm.error());
    
    std::vector<std::string> jvmArgs = {
        "-Xms2G",
        "-Xmx2G",
        "-jar"
    };
    std::vector<std::string> serverArgs = {
        "nogui"
    };
    if(mcsm::isWhitespaceOrEmpty(jvm.value())){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JVM_DETECTION_FAILED, {});
        return tl::unexpected(err);
    }
    return create(jvm.value(), jvmArgs, serverArgs);
}

// Reason why the program takes SearchTarget as a parameter (was also taken in the constructor) is because there's chance that constructor's specified SearchTarget might be ALL.
// update jul 2025: dropped searchtarget as a parameter in create() overloads

mcsm::VoidResult mcsm::JvmOption::create(const std::string& jvmPath){
    if(!this->initialized) {
        auto customTemp = mcsm::errors::INTERNAL_FUNC_EXECUTION_FAILED;
        customTemp.message = "JvmOption function called without load.";
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, customTemp, {});
        return tl::unexpected(err);
    }
    std::vector<std::string> jvmArgs = {
        "-Xms2G",
        "-Xmx2G",
        "-jar"
    };
    std::vector<std::string> serverArgs = {
        "nogui"
    };
    return create(jvmPath, jvmArgs, serverArgs);
}

mcsm::VoidResult mcsm::JvmOption::create(const std::string& jvmPath, const std::vector<std::string>& jvmOptions){
    if(!this->initialized) {
        auto customTemp = mcsm::errors::INTERNAL_FUNC_EXECUTION_FAILED;
        customTemp.message = "JvmOption function called without load.";
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, customTemp, {});
        return tl::unexpected(err);
    }
    std::vector<std::string> serverArgs = {
        "nogui"
    };
    return create(jvmPath, jvmOptions, serverArgs);
}

mcsm::VoidResult mcsm::JvmOption::create(const std::string& jvmPath, const std::vector<std::string>& jvmOptions, const std::vector<std::string>& serverOptions){
    if(!this->initialized) {
        auto customTemp = mcsm::errors::INTERNAL_FUNC_EXECUTION_FAILED;
        customTemp.message = "JvmOption function called without load.";
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, customTemp, {});
        return tl::unexpected(err);
    }
    if(target == mcsm::SearchTarget::ALL) {
        auto customTemp = mcsm::errors::ILLEGAL_PARAMETER;
        customTemp.message = "mcsm::SearchTarget::ALL was detected inside mcsm::JvmOption::create(). Report this to GitHub.";
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, customTemp, {});
        return tl::unexpected(err);
    }
    std::string filePath;
    std::string optionName;

    auto fileExists = this->option->exists();
    if(!fileExists) return tl::unexpected(fileExists.error());
    if(fileExists.value()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JVM_PROFILE_ALREADY_CONFIGURED, {this->name, this->option->getPath()});
        return tl::unexpected(err);
    }
    mcsm::Option* opt = this->option.get();

    bool advp = mcsm::GeneralOption::getGeneralOption().advancedParseEnabled();
        
    mcsm::VoidResult loadRes = opt->load(advp);
    if(!loadRes) return loadRes;

    auto res1 = opt->setValue("path", jvmPath);
    if(!res1) return res1;

    auto res2 = opt->setValue("args", jvmOptions);
    if(!res2) return res2;

    auto res3 = opt->setValue("server_args", serverOptions);
    if(!res3) return res3;

    return opt->save();
}

mcsm::VoidResult mcsm::JvmOption::reset(){
    if(!this->initialized) {
        auto customTemp = mcsm::errors::INTERNAL_FUNC_EXECUTION_FAILED;
        customTemp.message = "JvmOption function called without load.";
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, customTemp, {});
        return tl::unexpected(err);
    }
    return this->option->reset();
}

mcsm::BoolResult mcsm::JvmOption::exists(){
    if(!this->initialized) {
        auto customTemp = mcsm::errors::INTERNAL_FUNC_EXECUTION_FAILED;
        customTemp.message = "JvmOption function called without load.";
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, customTemp, {});
        return tl::unexpected(err);
    }
    if(this->option.get() == nullptr) return false;
    return this->option->exists();
}

tl::expected<std::vector<std::string>, mcsm::Error> mcsm::JvmOption::getJvmArguments(){
    if(!this->initialized) {
        auto customTemp = mcsm::errors::INTERNAL_FUNC_EXECUTION_FAILED;
        customTemp.message = "JvmOption function called without load.";
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, customTemp, {});
        return tl::unexpected(err);
    }
    auto profileExists = this->exists();
    if(!profileExists) return tl::unexpected(profileExists.error());
    if(!profileExists.value()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JVM_PROFILE_NOT_FOUND, {this->option->getName(), this->option->getPath()});
        return tl::unexpected(err);
    }

    mcsm::Option* opt2 = this->option.get();
    auto valueRes = opt2->getValue("args");
    if(!valueRes) return tl::unexpected(valueRes.error());
    const nlohmann::json& value = valueRes.value();
    
    if(value == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_NOT_FOUND, {"\"args\"", opt2->getName()});
        return tl::unexpected(err);
    }
    if(!value.is_array()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_WRONG_TYPE, {"\"args\"", "array of string"});
        return tl::unexpected(err);
    }
    for(auto& v : value){
        if(!v.is_string()){
            mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_WRONG_TYPE, {"\"args\"", "array of string"});
            return tl::unexpected(err);
        }
    }
    const std::vector<std::string>& args = value;
    return args;    
}

mcsm::VoidResult mcsm::JvmOption::setJvmArguments(const std::vector<std::string>& jvmArgs){
    if(!this->initialized) {
        auto customTemp = mcsm::errors::INTERNAL_FUNC_EXECUTION_FAILED;
        customTemp.message = "JvmOption function called without load.";
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, customTemp, {});
        return tl::unexpected(err);
    }
    auto profileExists = this->exists();
    if(!profileExists) return tl::unexpected(profileExists.error());
    if(!profileExists.value()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JVM_PROFILE_NOT_FOUND, {this->option->getName(), this->option->getPath()});
        return tl::unexpected(err);
    }

    mcsm::Option* opt2 = this->option.get();
    mcsm::VoidResult sRes = opt2->setValue("args", jvmArgs);
    if(!sRes) return sRes;
    return opt2->save();
}

mcsm::StringResult mcsm::JvmOption::getJvmPath(){
    if(!this->initialized) {
        auto customTemp = mcsm::errors::INTERNAL_FUNC_EXECUTION_FAILED;
        customTemp.message = "JvmOption function called without load.";
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, customTemp, {});
        return tl::unexpected(err);
    }
    auto profileExists = this->exists();
    if(!profileExists) return tl::unexpected(profileExists.error());
    if(!profileExists.value()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JVM_PROFILE_NOT_FOUND, {this->option->getName(), this->option->getPath()});
        return tl::unexpected(err);
    }
    mcsm::Option* opt2 = this->option.get();

    auto valueRes = opt2->getValue("path");
    if(!valueRes) return tl::unexpected(valueRes.error());
    const nlohmann::json& value = valueRes.value();
    
    if(value == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_NOT_FOUND, {"\"path\"", opt2->getName()});
        return tl::unexpected(err);
    }
    if(!value.is_string()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_WRONG_TYPE, {"\"path\"", "string"});
        return tl::unexpected(err);
    }
    const std::string& path = value;
    return path;
}

mcsm::VoidResult mcsm::JvmOption::setJvmPath(const std::string& jvmPath){
    if(!this->initialized) {
        auto customTemp = mcsm::errors::INTERNAL_FUNC_EXECUTION_FAILED;
        customTemp.message = "JvmOption function called without load.";
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, customTemp, {});
        return tl::unexpected(err);
    }
    auto profileExists = this->exists();
    if(!profileExists) return tl::unexpected(profileExists.error());
    if(!profileExists.value()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JVM_PROFILE_NOT_FOUND, {this->option->getName(), this->option->getPath()});
        return tl::unexpected(err);
    }
    mcsm::Option* opt2 = this->option.get();
    auto sRes = opt2->setValue("path", jvmPath);
    if(!sRes) return sRes;
    return opt2->save();
}

tl::expected<std::vector<std::string>, mcsm::Error> mcsm::JvmOption::getServerArguments(){
    if(!this->initialized) {
        auto customTemp = mcsm::errors::INTERNAL_FUNC_EXECUTION_FAILED;
        customTemp.message = "JvmOption function called without load.";
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, customTemp, {});
        return tl::unexpected(err);
    }
    auto profileExists = this->exists();
    if(!profileExists) return tl::unexpected(profileExists.error());
    if(!profileExists.value()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JVM_PROFILE_NOT_FOUND, {this->option->getName(), this->option->getPath()});
        return tl::unexpected(err);
    }
    mcsm::Option* opt2 = this->option.get();

    auto valueRes = opt2->getValue("server_args");
    if(!valueRes) return tl::unexpected(valueRes.error());
    const nlohmann::json& value = valueRes.value();

    if(value == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_NOT_FOUND, {"\"server_args\"", opt2->getName()});
        return tl::unexpected(err);
    }
    if(!value.is_array()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_WRONG_TYPE, {"\"server_args\"", "array of string"});
        return tl::unexpected(err);
    }
    for(auto& v : value){
        if(!v.is_string()){
            mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_WRONG_TYPE, {"\"server_args\"", "array of string"});
            return tl::unexpected(err);
        }
    }
    const std::vector<std::string>& args = value;
    return args;
}

mcsm::VoidResult mcsm::JvmOption::setServerArguments(const std::vector<std::string>& serverArgs){
    if(!this->initialized) {
        auto customTemp = mcsm::errors::INTERNAL_FUNC_EXECUTION_FAILED;
        customTemp.message = "JvmOption function called without load.";
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, customTemp, {});
        return tl::unexpected(err);
    }
    auto profileExists = this->exists();
    if(!profileExists) return tl::unexpected(profileExists.error());
    if(!profileExists.value()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JVM_PROFILE_NOT_FOUND, {this->option->getName(), this->option->getPath()});
        return tl::unexpected(err);
    }
    mcsm::Option* opt2 = this->option.get();
    auto sRes = opt2->setValue("server_args", serverArgs);
    if(!sRes) return sRes;
    return opt2->save();
}

std::string mcsm::JvmOption::getProfileName() const {
    return this->name;
}

mcsm::StringResult mcsm::JvmOption::getProfilePath() const {
    if(!this->initialized) {
        auto customTemp = mcsm::errors::INTERNAL_FUNC_EXECUTION_FAILED;
        customTemp.message = "JvmOption function called without load.";
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, customTemp, {});
        return tl::unexpected(err);
    }
    mcsm::Option* opt2 = this->option.get();
    return opt2->getPath();
}

mcsm::SearchTarget mcsm::JvmOption::getSearchTarget() const {
    return this->target;
}

mcsm::JvmOption::~JvmOption(){

}
