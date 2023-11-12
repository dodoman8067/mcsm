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

#include <mcsm/data/option.h>

mcsm::Option::Option(const std::string& path, const std::string& name){
    this->path = path;

    std::string name1 = name;
    if(!mcsm::endsWith(name1, ".json")){
        name1 = name1.append(".json");
    }
    this->name = name1;
}

mcsm::Option::~Option(){
}

bool mcsm::Option::createDirectories(std::string const &dirName, std::error_code &err) const{
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
    if(!std::filesystem::exists(fullPath)){
        std::error_code ec;
        createDirectories(this->path, ec);
        std::ofstream ofs(fullPath);
        if(!ofs.is_open()){
            mcsm::error("Cannot create directory/file " + fullPath);
            mcsm::error("This might have caused because of permissions or file name problems.");
            mcsm::error("If you believe that this is from name problems, try regenerating the file again with only English characters.");
            std::exit(1);
        }
        
        ofs << "{}";
        ofs.close();
    }

    std::ifstream fileStream(fullPath);
    if (!fileStream.is_open()) {
        mcsm::error("Cannot load file " + fullPath);
        std::exit(1);
    }

    std::string content((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
    fileStream.close();
    nlohmann::json finalValue = nlohmann::json::parse(content, nullptr, false);
    if(finalValue.is_discarded()) {
        mcsm::error("Failed to parse json " + fullPath);
        mcsm::error("There might be many reasons why you see this error.");
        mcsm::error("1. Comments: Comments are not allowed in JSON. If you used the comment syntax like other programming languages, you must remove them and try again.");
        mcsm::error("2. Unclosed Brackets: Check if all curly braces '{}' and square brackets '[]' are properly closed.");
        mcsm::error("3. Trailing Commas: Trailing Commas are not allowed in JSON. Make sure that end of the element doesn't end with comma.");
        mcsm::error("4. Incorrect Escape Sequences: Check if all escaped are used properly.");
        mcsm::error("5. Data types: Double check each element is storing the correct data types.");
        mcsm::error("6. File Access Permissions: Check if your OS or security software is blocking the access to the file. Creating the file in system directories may cause this problem.");
        std::exit(1);
    }
    return finalValue;
}

nlohmann::json mcsm::Option::getValue(const std::string& key) const {
    nlohmann::json jsonData = load();
    if(jsonData.find(key) != jsonData.end()){
        return jsonData[key];
    }else{
        return nullptr;
    }
}

bool mcsm::Option::hasValue(const std::string& key) const {
    nlohmann::json data = load();
    return data.find(key) != data.end();
}

bool mcsm::Option::exists() const {
    std::string fullPath = this->path + "/" + this->name;
    return std::filesystem::exists(fullPath);
}

void mcsm::Option::setValue(const std::string& key, const nlohmann::json& value) const {
    std::string fullPath = this->path + "/" + this->name;
    nlohmann::json jsonData = load();

    std::ofstream fileStream(fullPath);
    if (!fileStream.is_open()) {
        mcsm::error("Cannot open file " + fullPath);
        std::exit(1);
    }

    jsonData[key] = value;
    save(jsonData);
}

void mcsm::Option::save(const nlohmann::json& jsonData) const {
    std::string fullPath = this->path + "/" + this->name;
    std::ofstream outFile(fullPath);
    if (outFile.is_open()) {
        outFile << jsonData.dump(4); 
        outFile.close();
    }else{
        mcsm::error("Cannot save file " + fullPath);
        std::exit(1);
    }
}

void mcsm::Option::reset() const {
    std::string fullPath = this->path + "/" + this->name;
    std::ofstream outFile(fullPath);
    if (outFile.is_open()) {
        outFile << "{}";
        outFile.close();
    }else{
        mcsm::error("Cannot save file " + fullPath);
        std::exit(1);
    }    
}

std::string mcsm::Option::getName(){
    return this->name;
}

std::string mcsm::Option::getPath(){
    return this->path;
}
