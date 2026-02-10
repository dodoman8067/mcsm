#include "mcsm/util/cli/result.h"
#include <mcsm/data/options/general_option.h>
#include <mcsm/data/toml_option.h>

mcsm::TomlOption::TomlOption(const std::string& path, const std::string& name){
    this->path = mcsm::normalizePath(path);

    std::string name1 = name;
    mcsm::replaceAll(name1, "..", "__");
    mcsm::replaceAll(name1, "/", "_");
    if(!mcsm::endsWith(name1, ".toml")){
        name1 = name1.append(".toml");
    }
    this->name = name1;
}

mcsm::TomlOption::~TomlOption(){
}

mcsm::BoolResult mcsm::TomlOption::createDirectories(const std::string &dirName, std::error_code &err) const { // TODO: Return boolresult and not clear error code
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

mcsm::VoidResult mcsm::TomlOption::create(){
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
        
        ofs << "";
        ofs.close();
        return {};
    }
    mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::FILE_CREATE_FAILED, {fullPath});
    return tl::unexpected(err);
}

mcsm::VoidResult mcsm::TomlOption::load(){
    return load(false);
}

mcsm::VoidResult mcsm::TomlOption::load(const bool& /* advancedParse */){
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

    toml::parse_result parseRes = toml::parse_file(fullPath);
    if(parseRes.failed()){
        mcsm::ErrorTemplate et = mcsm::errors::TOML_PARSE_FAILED;
        et.solution = "";
        et.message = "Parsing file " + fullPath + " failed with reason: " + std::string(parseRes.error().description());
        return tl::unexpected(mcsm::makeError(ErrorStatus::MCSM_FAIL, et));
    }

    this->data = std::move(parseRes).table();
    loaded = true;
    return {};
}

mcsm::Result<toml::node*> mcsm::TomlOption::getValue(const std::string& key) const {
    if(!loaded){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, {700, "Option's get/set function called without being loaded.", ""}, {});
        return tl::unexpected(err);
    }
    if(auto* node = this->data.get(key)) return node;

    return nullptr;
}

mcsm::BoolResult mcsm::TomlOption::hasValue(const std::string& key) const {
    if(!loaded){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, {700, "Option's get/set function called without being loaded.", ""}, {});
        return tl::unexpected(err);
    }
    return this->data.contains(key);
}

mcsm::BoolResult mcsm::TomlOption::exists() const {
    const std::string& fullPath = this->path + "/" + this->name;
    return mcsm::fileExists(fullPath);
}

bool mcsm::TomlOption::isGlobal() const {
    return mcsm::startsWith(this->path, mcsm::getDataPathPerOS());
}

mcsm::VoidResult mcsm::TomlOption::setValue(const std::string& key, const toml::node& value){
    if(!loaded){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, {700, "Option's get/set function called without being loaded.", ""}, {});
        return tl::unexpected(err);
    }
    this->data.insert_or_assign(key, value);
    return {};
}

mcsm::VoidResult mcsm::TomlOption::save(){
    const std::string& fullPath = this->path + "/" + this->name;
    std::ofstream outFile(fullPath);
    if (outFile.is_open()) {
        outFile << this->data;
        return {};
    }else{
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::FILE_SAVE_FAILED, {fullPath});
        return tl::unexpected(err);
    }
    return {};
}

mcsm::VoidResult mcsm::TomlOption::save(const toml::table& table){
    const std::string& fullPath = this->path + "/" + this->name;
    std::ofstream outFile(fullPath);
    if (outFile.is_open()) {
        outFile << table;
        outFile.close();
    }else{
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::FILE_SAVE_FAILED, {fullPath});
        return tl::unexpected(err);
    }
    return {};
}

mcsm::VoidResult mcsm::TomlOption::reset(){
    const std::string& fullPath = this->path + "/" + this->name;
    std::ofstream outFile(fullPath);
    if (outFile.is_open()) {
        outFile << "";
        outFile.close();
    }else{
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::FILE_SAVE_FAILED, {fullPath});
        return tl::unexpected(err);
    }
    return {};
}

std::string mcsm::TomlOption::getName() const {
    std::string nameCp = this->name;
    mcsm::replaceAll(nameCp, ".json", "");
    return nameCp;
}

std::string mcsm::TomlOption::getPath() const {
    return this->path;
}

toml::table& mcsm::TomlOption::getData() const {
    return this->data;
}
