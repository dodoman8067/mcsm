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

#include "global_option.h"

mcsm::GlobalOption::GlobalOption(const std::string& path, const std::string& name){
    std::string path1 = path;
    std::replace(path1.begin(), path1.end(), '.', '/');
    std::string finalPath = getDataPathPerOS() + path1;
    this->path = finalPath;

    std::string name1 = name;
    if(!mcsm::endsWith(name1, ".json")){
        name1 = name1.append(".json");
    }
    this->name = name1;
}

mcsm::GlobalOption::~GlobalOption(){
}

std::string mcsm::GlobalOption::getDataPathPerOS(){
    std::string returnPath;
    switch(mcsm::getCurrentOS()){
        case mcsm::OS::WINDOWS: {
            std::string windowsPath = getenv("USERPROFILE");
            windowsPath.append("/AppData/Local/mcsm");
            mcsm::replaceAll(windowsPath, "\\", "/");
            returnPath = std::move(windowsPath);
            break;
        }

        case mcsm::OS::LINUX: {
            std::string linuxPath = "~/.local/share/mcsm";
            returnPath = std::move(linuxPath);
            break;
        }

        default: {
            std::cerr << "Error: This platform isn't supported.\n";
            std::cerr << "This program only supports Linux and Windows.\n";
            std::exit(1);
            break;
        }
    }
    return returnPath;
}

bool mcsm::GlobalOption::createDirectories(std::string const &dirName, std::error_code &err) const{
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

nlohmann::json mcsm::GlobalOption::load() const {
    std::string fullPath = this->path + "/" + this->name;

    if(!std::filesystem::exists(fullPath)){
        std::error_code ec;
        createDirectories(this->path, ec);
        std::ofstream ofs(fullPath);
        if(!ofs.is_open()){
            std::cerr << "Error: Cannot create directory " << fullPath << "\n";
            std::exit(1);
        }
        
        ofs << "{}";
        ofs.close();
    }

    std::ifstream fileStream(fullPath);
    if (!fileStream.is_open()) {
        std::cerr << "Error: Cannot load file " << fullPath << "\n";
        std::exit(1);
    }

    std::string content((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());

    fileStream.close();

    return nlohmann::json::parse(content);
}

nlohmann::json mcsm::GlobalOption::getValue(const std::string& key) const {
    nlohmann::json jsonData = load();
    if(jsonData.find(key) != jsonData.end()){
        return jsonData[key];
    }else{
        return nullptr;
    }
}

bool mcsm::GlobalOption::hasValue(const std::string& key) const {
    nlohmann::json data = load();
    return data.find(key) != data.end();
}

bool mcsm::GlobalOption::exists() const {
    std::string fullPath = this->path + "/" + this->name;
    return std::filesystem::exists(fullPath);
}

void mcsm::GlobalOption::setValue(const std::string& key, const nlohmann::json& value) const {
    std::string fullPath = this->path + "/" + this->name;
    nlohmann::json jsonData = load();

    std::ofstream fileStream(fullPath);
    if (!fileStream.is_open()) {
        std::cerr << "Error: Cannot open file " << fullPath << "\n";
        std::exit(1);
    }

    jsonData[key] = value;
    save(jsonData);
}

void mcsm::GlobalOption::save(const nlohmann::json& jsonData) const {
    std::string fullPath = this->path + "/" + this->name;
    std::ofstream outFile(fullPath);
    if (outFile.is_open()) {
        outFile << jsonData.dump(4); 
        outFile.close();
    }else{
        std::cerr << "Error: Cannot save file " << fullPath << "\n";
        std::exit(1);
    }
}

void mcsm::GlobalOption::reset() const {
    nlohmann::json jsonObj;
    std::string fullPath = this->path + "/" + this->name;
    std::ofstream outFile(fullPath);
    if (outFile.is_open()) {
        outFile << "{}";
        outFile.close();
    }else{
        std::cerr << "Error: Cannot save file " << fullPath << "\n";
        std::exit(1);
    }    
}

std::string mcsm::GlobalOption::getName(){
    return this->name;
}

std::string mcsm::GlobalOption::getPath(){
    return this->path;
}