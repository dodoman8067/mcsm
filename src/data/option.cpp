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

#include <mcsm/data/options/general_option.h>
#include <mcsm/data/option.h>

mcsm::Option::Option(const std::string& path, const std::string& name){
    this->path = path;

    std::string name1 = name;
    mcsm::replaceAll(name1, "..", "__");
    mcsm::replaceAll(name1, "/", "_");
    if(!mcsm::endsWith(name1, ".json")){
        name1 = name1.append(".json");
    }
    this->name = name1;
    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
}

mcsm::Option::~Option(){
    this->data = nullptr;
}

bool mcsm::Option::createDirectories(const std::string &dirName, std::error_code &err) const{
    err.clear();
    if(!std::filesystem::create_directories(dirName, err)){
        if(std::filesystem::exists(dirName)){
            err.clear();
            return true;
        }
        return false;
    }
    return true;
}

mcsm::Result mcsm::Option::load() const {
    return load(false);
}

mcsm::Result mcsm::Option::load(const bool& advancedParse) const {
    const std::string& fullPath = this->path + "/" + this->name;
    if(!std::filesystem::exists(fullPath)){
        std::error_code ec;
        createDirectories(this->path, ec);
        std::ofstream ofs(fullPath);
        if(!ofs.is_open()){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::fileCreateFailed(fullPath)});
            return res;
        }
        
        ofs << "{}";
        ofs.close();
    }

    std::ifstream fileStream(fullPath);
    if(!fileStream.is_open()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::fileOpenFailed(fullPath)});
        return res;
    }

    std::string content((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
    fileStream.close();

    nlohmann::json finalValue;

    if(advancedParse){
        try {
            finalValue = nlohmann::json::parse(content);
        }catch (const nlohmann::json::parse_error& e){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
                "Json parsed failed in " + fullPath + " with reason: " + std::string(e.what()),
                "Byte position of error: " + std::to_string(e.byte),
                "Report this to Github if you believe that this is an error."
            }});
            return res;
        }
    }else{
        finalValue = nlohmann::json::parse(content, nullptr, false);
        if(finalValue.is_discarded()) {
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonParseFailed(fullPath)});
            return res;
        }
    }

    this->data = finalValue;
    mcsm::Result res({mcsm::ResultType::MCSM_OK, {"Success"}});
    return res;
}

nlohmann::json mcsm::Option::getValue(const std::string& key) const {
    if(this->data == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {"Option's get/set function called without being loaded.", "Please report this to Github."}});
        return nullptr;
    }
    const nlohmann::json& jsonData = this->data;
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return nullptr;
    if(jsonData.find(key) != jsonData.end()){
        mcsm::Result res({mcsm::ResultType::MCSM_OK, {"Success"}});
        if(jsonData[key].is_string()) return jsonData[key];
        return jsonData[key];
    }else{
        return nullptr;
    }
}

bool mcsm::Option::hasValue(const std::string& key) const {
    if(this->data == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {"Option's get/set function called without being loaded.", "Please report this to Github."}});
        return false;
    }
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return false;
    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
    return this->data.find(key) != this->data.end();
}

bool mcsm::Option::exists() const {
    const std::string& fullPath = this->path + "/" + this->name;
    return mcsm::fileExists(fullPath);
}

bool mcsm::Option::isGlobal() const {
    return false;
}

mcsm::Result mcsm::Option::setValue(const std::string& key, const nlohmann::json& value) const {
    if(this->data == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {"Option's get/set function called without being loaded.", "Please report this to Github."}});
        return res;
    }
    const std::string& fullPath = this->path + "/" + this->name;
    nlohmann::json& jsonData = this->data;
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    jsonData[key] = value;
    return {mcsm::ResultType::MCSM_OK, {"Success"}};
}

mcsm::Result mcsm::Option::save() const {
    const std::string& fullPath = this->path + "/" + this->name;
    std::ofstream outFile(fullPath);
    if (outFile.is_open()) {
        outFile << this->data.dump(2); 
        outFile.close();
    }else{
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::fileSaveFailed(fullPath)});
        return res;
    }
    mcsm::Result res({mcsm::ResultType::MCSM_OK, {"Success"}});
    return res;
}

mcsm::Result mcsm::Option::save(const nlohmann::json& json) const {
    this->data = json;
    const std::string& fullPath = this->path + "/" + this->name;
    std::ofstream outFile(fullPath);
    if (outFile.is_open()) {
        outFile << this->data.dump(2); 
        outFile.close();
    }else{
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::fileSaveFailed(fullPath)});
        return res;
    }
    mcsm::Result res({mcsm::ResultType::MCSM_OK, {"Success"}});
    return res;
}

mcsm::Result mcsm::Option::reset() const {
    const std::string& fullPath = this->path + "/" + this->name;
    std::ofstream outFile(fullPath);
    if (outFile.is_open()) {
        outFile << "{}";
        outFile.close();
    }else{
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::fileSaveFailed(fullPath)});
        return res;
    }
    mcsm::Result res({mcsm::ResultType::MCSM_OK, {"Success"}});
    return res; 
}

std::string mcsm::Option::getName(){
    return this->name;
}

std::string mcsm::Option::getPath(){
    return this->path;
}

nlohmann::json& mcsm::Option::getData() const {
    return this->data;
}
