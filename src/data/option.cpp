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
    this->path = mcsm::normalizePath(path);

    std::string name1 = name;
    mcsm::replaceAll(name1, "..", "__");
    mcsm::replaceAll(name1, "/", "_");
    if(!mcsm::endsWith(name1, ".json")){
        name1 = name1.append(".json");
    }
    this->name = name1;
}

mcsm::Option::~Option(){
    this->data = nullptr;
}

mcsm::BoolResult mcsm::Option::createDirectories(const std::string &dirName, std::error_code &err) const { // TODO: Return boolresult and not clear error code
    err.clear();
    if(!std::filesystem::create_directories(dirName, err)){
        if(std::filesystem::exists(dirName)){
            err.clear();
            return true;
        }else {
            mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::FILE_CREATE_FAILED, {"directory " + dirName});
            return tl::unexpected(err);
        }
        return false;
    }
    return true;
}

mcsm::VoidResult mcsm::Option::create(){
    const std::string& fullPath = this->path + "/" + this->name;
    if(!std::filesystem::exists(fullPath)){
        std::error_code ec;
        mcsm::BoolResult cdRes = createDirectories(this->path, ec);
        if(!cdRes) return tl::unexpected(cdRes.error());
        std::ofstream ofs(fullPath);
        if(!ofs.is_open()){
            mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::FILE_CREATE_FAILED, {fullPath});
            return tl::unexpected(err);
        }
        
        ofs << "{}";
        ofs.close();
        return {};
    }
    mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::FILE_CREATE_FAILED, {fullPath});
    return tl::unexpected(err);
}

mcsm::VoidResult mcsm::Option::load(){
    return load(false);
}

mcsm::VoidResult mcsm::Option::load(const bool& advancedParse){
    const std::string& fullPath = this->path + "/" + this->name;
    if(!std::filesystem::exists(fullPath)){
        mcsm::VoidResult cRes = create();
        if(!cRes) return cRes;
        /*
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::fileNotFound(fullPath)});
        return res;
        */
    }

    std::ifstream fileStream(fullPath);
    if(!fileStream.is_open()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::FILE_OPEN_FAILED, {fullPath});
        return tl::unexpected(err);
    }

    std::string content((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
    fileStream.close();

    nlohmann::json finalValue;

    if(advancedParse){
        try {
            finalValue = nlohmann::json::parse(content);
        }catch (const nlohmann::json::parse_error& e){
            mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, {203, "Json parsed failed in " + fullPath + " with reason: " + std::string(e.what()) + "\nByte position of error: " + std::to_string(e.byte), ""}, {});
            return tl::unexpected(err);
        }
    }else{
        finalValue = nlohmann::json::parse(content, nullptr, false);
        if(finalValue.is_discarded()) {
            mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_PARSE_FAILED, {fullPath});
            return tl::unexpected(err);
        }
    }

    this->data = finalValue;
    return {};
}

tl::expected<nlohmann::json, mcsm::Error> mcsm::Option::getValue(const std::string& key) const {
    if(this->data == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, {700, "Option's get/set function called without being loaded.", ""}, {});
        return tl::unexpected(err);
    }
    const nlohmann::json& jsonData = this->data;
    if(jsonData.find(key) != jsonData.end()){
        if(jsonData[key].is_string()) return jsonData[key];
        return jsonData[key];
    }else{
        return nullptr;
    }
}

mcsm::BoolResult mcsm::Option::hasValue(const std::string& key) const {
    if(this->data == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, {700, "Option's get/set function called without being loaded.", ""}, {});
        return tl::unexpected(err);
    }
    return this->data.find(key) != this->data.end();
}

mcsm::BoolResult mcsm::Option::exists() const {
    const std::string& fullPath = this->path + "/" + this->name;
    return mcsm::fileExists(fullPath);
}

bool mcsm::Option::isGlobal() const {
    return mcsm::startsWith(this->path, mcsm::getDataPathPerOS());
}

mcsm::VoidResult mcsm::Option::setValue(const std::string& key, const nlohmann::json& value){
    if(this->data == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, {700, "Option's get/set function called without being loaded.", ""}, {});
        return tl::unexpected(err);
    }
    const std::string& fullPath = this->path + "/" + this->name;
    nlohmann::json& jsonData = this->data;

    jsonData[key] = value;
    return {};
}

mcsm::VoidResult mcsm::Option::save(){
    const std::string& fullPath = this->path + "/" + this->name;
    std::ofstream outFile(fullPath);
    if (outFile.is_open()) {
        outFile << this->data.dump(2); 
        outFile.close();
    }else{
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::FILE_SAVE_FAILED, {fullPath});
        return tl::unexpected(err);
    }
    return {};
}

mcsm::VoidResult mcsm::Option::save(const nlohmann::json& json){
    this->data = json;
    const std::string& fullPath = this->path + "/" + this->name;
    std::ofstream outFile(fullPath);
    if (outFile.is_open()) {
        outFile << this->data.dump(2); 
        outFile.close();
    }else{
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::FILE_SAVE_FAILED, {fullPath});
        return tl::unexpected(err);
    }
    return {};
}

mcsm::VoidResult mcsm::Option::reset(){
    const std::string& fullPath = this->path + "/" + this->name;
    std::ofstream outFile(fullPath);
    if (outFile.is_open()) {
        outFile << "{}";
        outFile.close();
    }else{
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::FILE_SAVE_FAILED, {fullPath});
        return tl::unexpected(err);
    }
    return {};
}

std::string mcsm::Option::getName() const {
    std::string nameCp = this->name;
    mcsm::replaceAll(nameCp, ".json", "");
    return nameCp;
}

std::string mcsm::Option::getPath() const {
    return this->path;
}

nlohmann::json& mcsm::Option::getData() const {
    return this->data;
}
