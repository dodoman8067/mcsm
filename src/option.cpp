#include "option.h"

mcsm::Option::Option(const std::string& path, const std::string& name){
    std::string path1 = path;
    std::replace(path1.begin(), path1.end(), '.', '/');
    std::string finalPath = "./.mcsm" + path1;
    this->path = finalPath;

    std::string name1 = name;
    name1 = name1.append(".json");
    this->name = name1;

    std::string fullPath = this->path + "/" + this->name;

    std::error_code ec;

    if(!std::filesystem::exists(fullPath)){
        createDirectories(this->path, ec);
        std::ofstream ofs(fullPath);
        if(!ofs.is_open()){
            std::cerr << "Error: Cannot create directory " << fullPath << std::endl;
            std::exit(1);
        }
        
        ofs << "{}";
        ofs.close();
    }
}

mcsm::Option::~Option(){
}

bool mcsm::Option::createDirectories(std::string const &dirName, std::error_code &err){
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

nlohmann::json mcsm::Option::load() const {
    std::string fullPath = this->path + "/" + this->name;
    std::ifstream fileStream(fullPath);
    if (!fileStream.is_open()) {
        std::cerr << "Error: Cannot load file " << fullPath << std::endl;
        std::exit(1);
    }

    std::string content((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());

    fileStream.close();

    return nlohmann::json::parse(content);
}

nlohmann::json mcsm::Option::getValue(const std::string& key) const {
    nlohmann::json jsonData = load();
    if (jsonData.find(key) != jsonData.end()) {
        return jsonData[key];
    } else {
        return nullptr;
    }
}

void mcsm::Option::setValue(const std::string& key, const nlohmann::json& value){
    nlohmann::json jsonData = load();
    std::string fullPath = this->path + "/" + this->name;

    jsonData[key] = value;

    std::ofstream fileStream(fullPath);
    if (!fileStream.is_open()) {
        std::cerr << "Error: Cannot open file " << fullPath << std::endl;
        std::exit(1);
    }

    fileStream << jsonData.dump(4); 
    fileStream.close();
}

void mcsm::Option::save(const nlohmann::json& jsonData) const {
    std::string fullPath = this->path + "/" + this->name;
    std::ofstream outFile(fullPath);
    if (outFile.is_open()) {
        outFile << jsonData.dump(4); 
        outFile.close();
    }else{
        std::cerr << "Error: Cannot save file " << fullPath << std::endl;
        std::exit(1);
    }
}

std::string mcsm::Option::getName(){
    return this->name;
}

std::string mcsm::Option::getPath(){
    return this->path;
}