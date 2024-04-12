/*
MIT License

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

#include <mcsm/data/options/multi_server_option.h>

mcsm::MultiServerOption::MultiServerOption(const std::string& path, const std::string& name){
    this->name = mcsm::safeString(name);
    mcsm::fileExists(path);
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return;
    }
    std::error_code ec;
    bool isPath = std::filesystem::is_directory(path, ec);
    if(ec){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
            "Checking if path " + path + "is a path operation failed : " + ec.message(), 
            "Please report this to GitHub (https://github.com/dodoman8067/mcsm) if you think this is a software issue."
        }});
        return;
    }
    if(!isPath){
        mcsm::Result res({mcsm::ResultType::MCSM_WARN, {
            "Non-directory path was passed : " + path, 
            "Please report this to GitHub (https://github.com/dodoman8067/mcsm) if you think this is a software issue."
        }});
        return;
    }
    this->option = std::make_unique<mcsm::Option>(path, "multi_server_" + this->name);
}

mcsm::Result mcsm::MultiServerOption::create() const {
    bool exists = this->option->exists();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    if(exists){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverAlreadyConfigured()});
        return res;
    }

    mcsm::Result res1 = this->option->setValue("name", this->name);
    if(!res1.isSuccess()) return res1;
    mcsm::Result res2 = this->option->setValue("servers", nlohmann::json::array());
    if(!res2.isSuccess()) return res2;

    mcsm::Result res3({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
    return res3;
}

bool mcsm::MultiServerOption::canBeTaken(const std::string& serverName) const {
    std::string name;
    for(auto& v : this->servers){
        if(mcsm::ServerOption* sPtr = std::get_if<mcsm::ServerOption>(&*v)){
            bool exists = sPtr->exists();
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return false;

            name = sPtr->getServerName();
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return false;

            if(name == serverName) return false;
        }else if (mcsm::FabricServerOption* fsPtr = std::get_if<mcsm::FabricServerOption>(&*v)){
            bool exists = fsPtr->exists();
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return false;

            name = fsPtr->getServerName();
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return false;

            if(name == serverName) return false;
        }else{
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
                "std::variant<mcsm::ServerOption, mcsm::FabricServerOption> test didn't pass.",
                "Open an issue in GitHub (https://github.com/dodoman8067/mcsm) and tell us how did you got this message."
            }});
            return false;
        }
    }
    return true;
}

mcsm::Result mcsm::MultiServerOption::addServer(std::unique_ptr<std::variant<mcsm::ServerOption, mcsm::FabricServerOption>> server) const {
    std::string name, path;
    if(mcsm::ServerOption* sPtr = std::get_if<mcsm::ServerOption>(&*server)){
        bool exists = sPtr->exists();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
            std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
            mcsm::Result res(resp.first, resp.second);
            return res;
        }

        name = sPtr->getServerName();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
            std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
            mcsm::Result res(resp.first, resp.second);
            return res;
        }

        path = sPtr->getOptionPath();

    }else if (mcsm::FabricServerOption* fsPtr = std::get_if<mcsm::FabricServerOption>(&*server)){
        bool exists = fsPtr->exists();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
            std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
            mcsm::Result res(resp.first, resp.second);
            return res;
        }

        name = fsPtr->getServerName();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
            std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
            mcsm::Result res(resp.first, resp.second);
            return res;
        }

        path = fsPtr->getOptionPath();
    }else{
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
            "std::variant<mcsm::ServerOption, mcsm::FabricServerOption> test didn't pass.",
            "Open an issue in GitHub (https://github.com/dodoman8067/mcsm) and tell us how did you get this message."
        }});
        return res;
    }

    //TODO : Check if the server option's name matches with other linked servers and add the server option instance 
}

mcsm::MultiServerOption::~MultiServerOption(){

}