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

// My plan for downloading different server files per different server configurations : 
// create a method that uses detectServerType per server configs and use the server class's download method. there's going to be a threaded download system and one thread will contain 3~4 processes.

#include <mcsm/data/options/multi_server_option.h>

mcsm::MultiServerOption::MultiServerOption(const std::string& path){
    std::error_code ec;
    bool isPath = std::filesystem::is_directory(path, ec);
    if(ec){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
            "Checking if " + path + " is a path operation failed : " + ec.message(), 
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
    this->option = std::make_unique<mcsm::Option>(path, "multi_server");

    bool exists = this->option->exists();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return;
    }

    if(exists){
        mcsm::Result res = load();
        if(!res.isSuccess()) return;
    }
}

const std::vector<mcsm::ServerOption*> mcsm::MultiServerOption::getServerOptions() const {
    std::vector<mcsm::ServerOption*> sOpts;
    for(auto &v : this->servers){
        if(mcsm::ServerOption* sPtr = std::get_if<mcsm::ServerOption>(&*v)){
            sOpts.push_back(sPtr);
        }
    }
    return sOpts;
}

const std::vector<mcsm::FabricServerOption*> mcsm::MultiServerOption::getFabricServerOptions() const {
    std::vector<mcsm::FabricServerOption*> sOpts;
    for(auto &v : this->servers){
        if(mcsm::FabricServerOption* sPtr = std::get_if<mcsm::FabricServerOption>(&*v)){
            sOpts.push_back(sPtr);
        }
    }
    return sOpts;
}

bool mcsm::MultiServerOption::exists() const {
    bool rt = this->option->exists();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return false;
    return rt;
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

    mcsm::Result res2 = this->option->setValue("servers", nlohmann::json::array());
    if(!res2.isSuccess()) return res2;

    mcsm::Result res3({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
    return res3;
}

mcsm::Result mcsm::MultiServerOption::load(){
    bool exists = this->option->exists();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    if(!exists){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverNotConfigured()});
        return res;
    }

    nlohmann::json serverArray = this->option->getValue("servers");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    if(serverArray == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFound("\"servers\"", this->option->getName())});
        return res;
    }
    if(!serverArray.is_array()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"servers\"", "array of json object")});
        return res;
    }
    
    for(nlohmann::json serverObj : serverArray){
        if(serverObj == nullptr){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFound("json object inside \"servers\"", this->option->getName())});
            return res;
        }
        if(!serverObj.is_object()){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("json object inside \"servers\"", "array of json object")});
            return res;
        }
        if(serverObj["path"] == nullptr){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFound("json object's \"path\"", this->option->getName())});
            return res;
        }
        if(!serverObj["path"].is_string()){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("json object's \"path\"", "string")});
            return res;
        }

        std::string path = serverObj["path"];

        std::error_code ec;
        bool isPath = std::filesystem::is_directory(path, ec);
        if(ec){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
                "Checking if " + path + " is a path operation failed : " + ec.message(), 
                "Please report this to GitHub (https://github.com/dodoman8067/mcsm) if you think this is a software issue."
            }});
            return res;
        }
        if(!isPath){
            mcsm::Result res({mcsm::ResultType::MCSM_WARN, {
                "Non-directory path was passed : " + path, 
                "Please report this to GitHub (https://github.com/dodoman8067/mcsm) if you think this is a software issue."
            }});
            return res;
        }

        // Usually C++ runtime library fails to check if there's no such directory

        bool pathExists = mcsm::fileExists(path);
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
            std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
            mcsm::Result res(resp.first, resp.second);
            return res;
        }

        if(!pathExists){
            mcsm::Result res({mcsm::ResultType::MCSM_WARN, {
                "Cannot find server path " + path + ".",
                "Please report this to GitHub (https://github.com/dodoman8067/mcsm) if you believe that this is a software issue."
            }});
            return res;
        }

        if(path == this->option->getPath()){
            mcsm::Result res({mcsm::ResultType::MCSM_WARN, {
                "Multi server configuration file and its linked server configuration file cannot exist in the same path.",
                "Please report this to GitHub (https://github.com/dodoman8067/mcsm) if you believe that this is a software issue."
            }});
            return res;
        }

        mcsm::Option opt(path, "server");

        bool optExists = opt.exists();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
            std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
            mcsm::Result res(resp.first, resp.second);
            return res;
        }
        
        if(!optExists){
            mcsm::Result res({mcsm::ResultType::MCSM_WARN, {
                "Cannot load a server configuration file that doesn't exist.",
                "Please report this to GitHub (https://github.com/dodoman8067/mcsm) if you believe that this is a software issue."
            }});
            return res;
        }
        
        nlohmann::json optServ = opt.getValue("type");
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
            std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
            mcsm::Result res(resp.first, resp.second);
            return res;
        }

        if(optServ == nullptr){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFound("\"type\"", opt.getName())});
            return res;
        }
        if(!optServ.is_string()){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"type\"", "string")});
            return res;
        }

        std::string type = opt.getValue("type");
        if(type == "fabric"){
            std::unique_ptr<mcsm::FabricServerOption> fabric = std::make_unique<mcsm::FabricServerOption>(path);
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
                std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
                mcsm::Result res(resp.first, resp.second);
                return res;
            }

            std::string name = fabric->getServerName();
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
                std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
                mcsm::Result res(resp.first, resp.second);
                return res;
            }

            bool canTake = canBeTaken(name);
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
                std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
                mcsm::Result res(resp.first, resp.second);
                return res;
            }

            if(canTake) this->servers.push_back(std::make_unique<std::variant<mcsm::ServerOption, mcsm::FabricServerOption>>(std::move(*fabric)));
        }else{
            std::unique_ptr<mcsm::ServerOption> base = std::make_unique<mcsm::ServerOption>(path);
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
                std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
                mcsm::Result res(resp.first, resp.second);
                return res;
            }

            std::string name = base->getServerName();
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
                std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
                mcsm::Result res(resp.first, resp.second);
                return res;
            }

            bool canTake = canBeTaken(name);
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
                std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
                mcsm::Result res(resp.first, resp.second);
                return res;
            }

            if(canTake) this->servers.push_back(std::make_unique<std::variant<mcsm::ServerOption, mcsm::FabricServerOption>>(std::move(*base)));
        }
    }

    mcsm::Result successfulRes({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
    return successfulRes;
}

mcsm::Result mcsm::MultiServerOption::save(){
    std::string sName, sPath;

    bool exists = this->option->exists();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    if(!exists){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverNotConfigured()});
        return res;
    }

    std::vector<nlohmann::json> objArr;

    for(auto& v : this->servers){
        if(mcsm::ServerOption* sPtr = std::get_if<mcsm::ServerOption>(&*v)){
            bool exists = sPtr->exists();
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
                std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
                mcsm::Result res(resp.first, resp.second);
                return res;
            }

            if(!exists){
                mcsm::Result res({mcsm::ResultType::MCSM_WARN, {
                    "Cannot load a server configuration file that doesn't exist.",
                    "Please report this to GitHub (https://github.com/dodoman8067/mcsm) if you believe that this is a software issue."
                }});
                return res;
            }

            sName = sPtr->getServerName();
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
                std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
                mcsm::Result res(resp.first, resp.second);
                return res;
            }

            sPath = sPtr->getOptionPath();

            if(sPath == this->option->getPath()){
                mcsm::Result res({mcsm::ResultType::MCSM_WARN, {
                    "Multi server configuration file and its linked server configuration file cannot exist in the same path.",
                    "Please report this to GitHub (https://github.com/dodoman8067/mcsm) if you believe that this is a software issue."
                }});
                return res;
            }

            nlohmann::json jObj;
            jObj["name"] = sName;
            jObj["path"] = sPath;

            objArr.push_back(jObj);
        }else if (mcsm::FabricServerOption* fsPtr = std::get_if<mcsm::FabricServerOption>(&*v)){
            bool exists = fsPtr->exists();
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
                std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
                mcsm::Result res(resp.first, resp.second);
                return res;
            }

            if(!exists){
                mcsm::Result res({mcsm::ResultType::MCSM_WARN, {
                    "Cannot load a server configuration file that doesn't exist.",
                    "Please report this to GitHub (https://github.com/dodoman8067/mcsm) if you believe that this is a software issue."
                }});
                return res;
            }

            sName = fsPtr->getServerName();
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
                std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
                mcsm::Result res(resp.first, resp.second);
                return res;
            }

            sPath = fsPtr->getOptionPath();

            if(sPath == this->option->getPath()){
                mcsm::Result res({mcsm::ResultType::MCSM_WARN, {
                    "Multi server configuration file and its linked server configuration file cannot exist in the same path.",
                    "Please report this to GitHub (https://github.com/dodoman8067/mcsm) if you believe that this is a software issue."
                }});
                return res;
            }

            nlohmann::json jObj;
            jObj["name"] = sName;
            jObj["path"] = sPath;

            objArr.push_back(jObj);
        }else{
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
                "std::variant<mcsm::ServerOption, mcsm::FabricServerOption> test didn't pass.",
                "Open an issue in GitHub (https://github.com/dodoman8067/mcsm) and tell us how did you get this message."
            }});
            return res;
        }
    }

    return this->option->setValue("servers", objArr);
}

bool mcsm::MultiServerOption::canBeTaken(const std::string& serverName) const {
    std::string name;
    for(auto& v : this->servers){
        if(mcsm::ServerOption* sPtr = std::get_if<mcsm::ServerOption>(&*v)){
            bool exists = sPtr->exists();
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return false;
            if(!exists){
                mcsm::Result res({mcsm::ResultType::MCSM_WARN, {
                    "Cannot load a server configuration file that doesn't exist.",
                    "Please report this to GitHub (https://github.com/dodoman8067/mcsm) if you believe that this is a software issue."
                }});
                return false;
            }

            name = sPtr->getServerName();
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return false;

            if(name == serverName) return false;
        }else if (mcsm::FabricServerOption* fsPtr = std::get_if<mcsm::FabricServerOption>(&*v)){
            bool exists = fsPtr->exists();
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return false;
            if(!exists){
                mcsm::Result res({mcsm::ResultType::MCSM_WARN, {
                    "Cannot load a server configuration file that doesn't exist.",
                    "Please report this to GitHub (https://github.com/dodoman8067/mcsm) if you believe that this is a software issue."
                }});
                return false;
            }

            name = fsPtr->getServerName();
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return false;

            if(name == serverName) return false;
        }else{
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
                "std::variant<mcsm::ServerOption, mcsm::FabricServerOption> test didn't pass.",
                "Open an issue in GitHub (https://github.com/dodoman8067/mcsm) and tell us how did you get this message."
            }});
            return false;
        }
    }
    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
    return true;
}

mcsm::Result mcsm::MultiServerOption::addServer(std::unique_ptr<std::variant<mcsm::ServerOption, mcsm::FabricServerOption>>& server){
    std::string name;
    if(mcsm::ServerOption* sPtr = std::get_if<mcsm::ServerOption>(&*server)){
        bool exists = sPtr->exists();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
            std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
            mcsm::Result res(resp.first, resp.second);
            return res;
        }
        if(!exists){
            mcsm::Result res({mcsm::ResultType::MCSM_WARN, {
                "Cannot load a server configuration file that doesn't exist.",
                "Please report this to GitHub (https://github.com/dodoman8067/mcsm) if you believe that this is a software issue."
            }});
            return res;
        }

        name = sPtr->getServerName();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
            std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
            mcsm::Result res(resp.first, resp.second);
            return res;
        }

        //path = sPtr->getOptionPath();
        
    }else if (mcsm::FabricServerOption* fsPtr = std::get_if<mcsm::FabricServerOption>(&*server)){
        bool exists = fsPtr->exists();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
            std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
            mcsm::Result res(resp.first, resp.second);
            return res;
        }
        if(!exists){
            mcsm::Result res({mcsm::ResultType::MCSM_WARN, {
                "Cannot load a server configuration file that doesn't exist.",
                "Please report this to GitHub (https://github.com/dodoman8067/mcsm) if you believe that this is a software issue."
            }});
            return res;
        }

        name = fsPtr->getServerName();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
            std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
            mcsm::Result res(resp.first, resp.second);
            return res;
        }

        //path = fsPtr->getOptionPath();
    }else{
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
            "std::variant<mcsm::ServerOption, mcsm::FabricServerOption> test didn't pass.",
            "Open an issue in GitHub (https://github.com/dodoman8067/mcsm) and tell us how did you get this message."
        }});
        return res;
    }

    //TODO : Check if the server option's name matches with other linked servers and add the server option instance 
    bool validName = canBeTaken(name);
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    if(!validName){
        mcsm::Result res({mcsm::ResultType::MCSM_WARN_NOEXIT, {
            "Current server name \"" + name + "\" is already taken."
        }});
        return res;
    }
    
    this->servers.push_back(std::move(server));

    return save();
}

mcsm::Result mcsm::MultiServerOption::removeServer(std::unique_ptr<std::variant<mcsm::ServerOption, mcsm::FabricServerOption>>& server){
    std::string name, name2;
    if(mcsm::ServerOption* sPtr = std::get_if<mcsm::ServerOption>(&*server)){
        bool exists = sPtr->exists();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
            std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
            mcsm::Result res(resp.first, resp.second);
            return res;
        }
        if(!exists){
            mcsm::Result res({mcsm::ResultType::MCSM_WARN, {
                "Cannot load a server configuration file that doesn't exist.",
                "Please report this to GitHub (https://github.com/dodoman8067/mcsm) if you believe that this is a software issue."
            }});
            return res;
        }

        name = sPtr->getServerName();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
            std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
            mcsm::Result res(resp.first, resp.second);
            return res;
        }

        //path = sPtr->getOptionPath();
        
    }else if (mcsm::FabricServerOption* fsPtr = std::get_if<mcsm::FabricServerOption>(&*server)){
        bool exists = fsPtr->exists();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
            std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
            mcsm::Result res(resp.first, resp.second);
            return res;
        }
        if(!exists){
            mcsm::Result res({mcsm::ResultType::MCSM_WARN, {
                "Cannot load a server configuration file that doesn't exist.",
                "Please report this to GitHub (https://github.com/dodoman8067/mcsm) if you believe that this is a software issue."
            }});
            return res;
        }

        name = fsPtr->getServerName();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
            std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
            mcsm::Result res(resp.first, resp.second);
            return res;
        }

        //path = fsPtr->getOptionPath();
    }else{
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
            "std::variant<mcsm::ServerOption, mcsm::FabricServerOption> test didn't pass.",
            "Open an issue in GitHub (https://github.com/dodoman8067/mcsm) and tell us how did you get this message."
        }});
        return res;
    }

    for(size_t i = 0; i<this->servers.size(); i++){
        if(mcsm::ServerOption* sPtr2 = std::get_if<mcsm::ServerOption>(&*this->servers[i])){
            bool exists = sPtr2->exists();
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
                std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
                mcsm::Result res(resp.first, resp.second);
                return res;
            }
            if(!exists){
                mcsm::Result res({mcsm::ResultType::MCSM_WARN, {
                    "Cannot load a server configuration file that doesn't exist.",
                    "Please report this to GitHub (https://github.com/dodoman8067/mcsm) if you believe that this is a software issue."
                }});
                return res;
            }

            name2 = sPtr2->getServerName();
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
                std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
                mcsm::Result res(resp.first, resp.second);
                return res;
            }

            if(name == name2){ this->servers.erase(this->servers.begin() + i); return save(); }
        }else if (mcsm::FabricServerOption* fsPtr2 = std::get_if<mcsm::FabricServerOption>(&*this->servers[i])){
            bool exists = fsPtr2->exists();
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
                std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
                mcsm::Result res(resp.first, resp.second);
                return res;
            }
            if(!exists){
                mcsm::Result res({mcsm::ResultType::MCSM_WARN, {
                    "Cannot load a server configuration file that doesn't exist.",
                    "Please report this to GitHub (https://github.com/dodoman8067/mcsm) if you believe that this is a software issue."
                }});
                return res;
            }

            name2 = fsPtr2->getServerName();
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
                std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
                mcsm::Result res(resp.first, resp.second);
                return res;
            }
            
            if(name == name2){ this->servers.erase(this->servers.begin() + i); return save(); }
        }else{
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
                "std::variant<mcsm::ServerOption, mcsm::FabricServerOption> test didn't pass.",
                "Open an issue in GitHub (https://github.com/dodoman8067/mcsm) and tell us how did you get this message."
            }});
            return res;
        }
    }
    mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
        "Cannot remove a element that doesn't exist.",
        "Please report this to GitHub (https://github.com/dodoman8067/mcsm) if you believe that this is a software issue."
    }});
    return res;
}

const std::vector<std::unique_ptr<std::variant<mcsm::ServerOption, mcsm::FabricServerOption>>>& mcsm::MultiServerOption::getServers() const {
    return this->servers;
}

std::string mcsm::MultiServerOption::getServerStartCommand(std::variant<mcsm::ServerOption, mcsm::FabricServerOption>& server) const {
    std::string jvmPath, jvmArgs, sArgs, file;
    if(mcsm::ServerOption* sPtr = std::get_if<mcsm::ServerOption>(&server)){
        auto opt = sPtr->getDefaultOption();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";

        jvmPath = opt->getJvmPath();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";

        auto tempJvmArgs = opt->getJvmArguments();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";
        for(auto& s : tempJvmArgs){
            jvmArgs = jvmArgs + s + " ";
        }

        auto tempServerArgs = opt->getServerArguments();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";
        for(auto& s1 : tempServerArgs){
            sArgs = sArgs + s1 + " ";
        }

        file = sPtr->getServerJarFile();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";

        
    }else if (mcsm::FabricServerOption* fsPtr = std::get_if<mcsm::FabricServerOption>(&server)){
        auto opt = fsPtr->getDefaultOption();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";

        jvmPath = opt->getJvmPath();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";

        auto tempJvmArgs = opt->getJvmArguments();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";
        for(auto& s : tempJvmArgs){
            jvmArgs = jvmArgs + s + " ";
        }

        auto tempServerArgs = opt->getServerArguments();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";
        for(auto& s1 : tempServerArgs){
            sArgs = sArgs + s1 + " ";
        }

        file = fsPtr->getServerJarFile();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";
    }else{
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
            "std::variant<mcsm::ServerOption, mcsm::FabricServerOption> test didn't pass.",
            "Open an issue in GitHub (https://github.com/dodoman8067/mcsm) and tell us how did you get this message."
        }});
        return "";
    }

    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
    return jvmPath + " " + jvmArgs + file + " " + sArgs;
}

mcsm::Result mcsm::MultiServerOption::addProcesses() const {
    std::string command, path, name;
    for(auto &v : this->servers){
        if(mcsm::ServerOption* sPtr = std::get_if<mcsm::ServerOption>(&*v)){
            bool exists = sPtr->exists();
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
                std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
                mcsm::Result res(resp.first, resp.second);
                return res;
            }
            if(!exists){
                mcsm::Result res({mcsm::ResultType::MCSM_WARN, {
                    "Cannot load a server configuration file that doesn't exist.",
                    "Please report this to GitHub (https://github.com/dodoman8067/mcsm) if you believe that this is a software issue."
                }});
                return res;
            }

            name = sPtr->getServerName();
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
                std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
                mcsm::Result res(resp.first, resp.second);
                return res;
            }

            path = sPtr->getOptionPath();
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
                std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
                mcsm::Result res(resp.first, resp.second);
                return res;
            }

            #ifdef _WIN32
                command = "cmd /c " + getServerStartCommand(*v);
            #else
                command = getServerStartCommand(*v);
            #endif
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
                std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
                mcsm::Result res(resp.first, resp.second);
                return res;
            }
        }else if (mcsm::FabricServerOption* fsPtr = std::get_if<mcsm::FabricServerOption>(&*v)){
            bool exists = fsPtr->exists();
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
                std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
                mcsm::Result res(resp.first, resp.second);
                return res;
            }
            if(!exists){
                mcsm::Result res({mcsm::ResultType::MCSM_WARN, {
                    "Cannot load a server configuration file that doesn't exist.",
                    "Please report this to GitHub (https://github.com/dodoman8067/mcsm) if you believe that this is a software issue."
                }});
                return res;
            }

            name = fsPtr->getServerName();
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
                std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
                mcsm::Result res(resp.first, resp.second);
                return res;
            }

            path = fsPtr->getOptionPath();
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
                std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
                mcsm::Result res(resp.first, resp.second);
                return res;
            }
            #ifdef _WIN32
                command = "cmd /c " + getServerStartCommand(*v);
            #else
                command = getServerStartCommand(*v);
            #endif
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
                std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
                mcsm::Result res(resp.first, resp.second);
                return res;
            }
        }else{
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
                "std::variant<mcsm::ServerOption, mcsm::FabricServerOption> test didn't pass.",
                "Open an issue in GitHub (https://github.com/dodoman8067/mcsm) and tell us how did you get this message."
            }});
            return res;
        }
        //std::cout << path << std::endl;
        std::shared_ptr<mcsm::ServerProcess> sp = std::make_shared<mcsm::ServerProcess>(command, path);
        processes.push_back({name, std::move(sp)});
    }

    return mcsm::Result({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
}

mcsm::Result mcsm::MultiServerOption::downloadPerServer() const {
    std::string version, path, name;

    for(auto &v : this->servers){
        if(mcsm::ServerOption* sPtr = std::get_if<mcsm::ServerOption>(&*v)){
            bool exists = sPtr->exists();
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
                std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
                mcsm::Result res(resp.first, resp.second);
                return res;
            }
            if(!exists){
                mcsm::Result res({mcsm::ResultType::MCSM_WARN, {
                    "Cannot load a server configuration file that doesn't exist.",
                    "Please report this to GitHub (https://github.com/dodoman8067/mcsm) if you believe that this is a software issue."
                }});
                return res;
            }

            version = sPtr->getServerVersion();
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
                std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
                mcsm::Result res(resp.first, resp.second);
                return res;
            }

            path = sPtr->getOptionPath();
            
            name = sPtr->getServerType() + ".jar";
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
                std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
                mcsm::Result res(resp.first, resp.second);
                return res;
            }

            bool fileExists = mcsm::fileExists(path + "/" + name);
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
                std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
                mcsm::Result res(resp.first, resp.second);
                return res;
            }

            if(fileExists) continue;

            auto sIPtr = sPtr->getServer();
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
                std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
                mcsm::Result res(resp.first, resp.second);
                return res;
            }
            
            std::string sName = sPtr->getServerName();
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
                std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
                mcsm::Result res(resp.first, resp.second);
                return res;
            }
            mcsm::info("Obtaining server " + sName + "'s " + path + "/" + name);

            mcsm::Result jarRes = sIPtr->obtainJarFile(version, path, name, path);
            if(!jarRes.isSuccess()) return jarRes;
        }else if (mcsm::FabricServerOption* fsPtr = std::get_if<mcsm::FabricServerOption>(&*v)){
            bool exists = fsPtr->exists();
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
                std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
                mcsm::Result res(resp.first, resp.second);
                return res;
            }
            if(!exists){
                mcsm::Result res({mcsm::ResultType::MCSM_WARN, {
                    "Cannot load a server configuration file that doesn't exist.",
                    "Please report this to GitHub (https://github.com/dodoman8067/mcsm) if you believe that this is a software issue."
                }});
                return res;
            }
            
            version = fsPtr->getServerVersion();
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
                std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
                mcsm::Result res(resp.first, resp.second);
                return res;
            }

            path = fsPtr->getOptionPath();
            
            name = fsPtr->getServerType() + ".jar";
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
                std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
                mcsm::Result res(resp.first, resp.second);
                return res;
            }

            bool fileExists = mcsm::fileExists(path + "/" + name);
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
                std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
                mcsm::Result res(resp.first, resp.second);
                return res;
            }

            if(fileExists) continue;

            auto fsIPtr = fsPtr->getServer();
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
                std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
                mcsm::Result res(resp.first, resp.second);
                return res;
            }
            
            std::string sName = fsPtr->getServerName();
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
                std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
                mcsm::Result res(resp.first, resp.second);
                return res;
            }
            mcsm::info("Obtaining server " + sName + "'s " + path + "/" + name);

            mcsm::Result jarRes = fsIPtr->obtainJarFile(version, path, name, path);
            if(!jarRes.isSuccess()) return jarRes;
        }else{
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
                "std::variant<mcsm::ServerOption, mcsm::FabricServerOption> test didn't pass.",
                "Open an issue in GitHub (https://github.com/dodoman8067/mcsm) and tell us how did you get this message."
            }});
            return res;
        }
    }
    return mcsm::Result({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
}

mcsm::Result mcsm::MultiServerOption::start() const {
    mcsm::Result downloadRes = downloadPerServer();
    if(!downloadRes.isSuccess()) return downloadRes;
    mcsm::Result addPRes = addProcesses();
    if(this->processes.empty()){
        return mcsm::Result({mcsm::ResultType::MCSM_FAIL, {
            "Processes vector is empty.",
            "Open an issue in Github and tell us to check addProcesses() in mcsm::MultiServerOption."
        }});
    }

    std::atomic_bool stopFlag(false);

    std::vector<std::thread> monitorThreads;
    
    for(auto& pair : this->processes){
        std::string serverName = pair.first;
        std::shared_ptr<mcsm::ServerProcess> process = pair.second;
        mcsm::Result startResult = process->start();
        if(startResult.isSuccess()){
            mcsm::info("Starting server " + serverName + " with pid " + std::to_string(process->getPID()));
                monitorThreads.emplace_back([this, process = std::move(process), serverName]() mutable {    
                    mcsm::Result res = process->waitForCompletion();
                    if(!res.isSuccess()) res.printMessage();
                });
        }else{
            mcsm::error("Failed to start server " + serverName + ": " + startResult.getMessage()[0]);
        }
    }

    // Start input handler thread
    std::thread inputThread(&mcsm::MultiServerOption::inputHandler, this, std::ref(stopFlag));
    
    // Start process monitor thread
    std::thread monitorThread(&mcsm::MultiServerOption::processMonitor, this, std::ref(stopFlag));

    for(auto& t : monitorThreads){
        t.join();
    }

    if(inputThread.joinable()){
        inputThread.join();
    }

    if(monitorThread.joinable()){
        monitorThread.join();
    }

    return mcsm::Result({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
}

bool mcsm::MultiServerOption::anyRunning() const {
    for(const auto& pair : this->processes){
        if(pair.second && pair.second->isActivate()){
            return true;
        }
    }
    return false;
}

#ifdef _WIN32
void mcsm::MultiServerOption::inputHandler(std::atomic_bool& stopFlag) const {
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(hStdin, &mode);
    SetConsoleMode(hStdin, mode & ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT));

    std::string input;
    DWORD eventsRead = 0;
    INPUT_RECORD inputBuffer[128];

    while (!stopFlag.load()){
        if(anyRunning()){
            std::cerr << ">> " << std::flush;
            while (true){
                ReadConsoleInput(hStdin, inputBuffer, 128, &eventsRead);
                for(DWORD i = 0; i < eventsRead; ++i){
                    if(inputBuffer[i].EventType == KEY_EVENT && inputBuffer[i].Event.KeyEvent.bKeyDown){
                        char ch = inputBuffer[i].Event.KeyEvent.uChar.AsciiChar;
                        if(ch == '\r' || ch == '\n'){
                            if(!input.empty()){
                                std::cerr << "\n";
                                processInput(input, stopFlag);
                                input.clear();
                                std::cerr << ">> " << std::flush;
                            }else std::cerr << "\n>> " << std::flush;
                        }else if (ch == '\b'){
                            if(!input.empty()){
                                input.pop_back();
                                std::cerr << "\b \b" << std::flush;
                            }
                        }else{
                            input += ch;
                            std::cerr << ch << std::flush;
                        }
                    }
                }
                if(stopFlag.load() || !anyRunning()){
                    break;
                }
            }
        }else{
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        
        if(stopFlag.load() || !anyRunning()){
            break;
        }
    }

    if(!anyRunning()){
        std::cerr << "All server processes have either exited or been instructed to stop." << std::endl << std::flush;
    }

    SetConsoleMode(hStdin, mode);
}
#else
void mcsm::MultiServerOption::inputHandler(std::atomic_bool& stopFlag) const {
    std::string input;
    fd_set readfds;
    int stdin_fd = fileno(stdin);

    bool promptPrinted = false;

    while (!stopFlag.load()){
        if(!promptPrinted && anyRunning()){
            std::cerr << ">> " << std::flush;
            promptPrinted = true;
        }

        FD_ZERO(&readfds);
        FD_SET(stdin_fd, &readfds);

        timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 100000;

        int ret = select(stdin_fd + 1, &readfds, NULL, NULL, &timeout);
        if(ret > 0 && FD_ISSET(stdin_fd, &readfds)){
            if(std::getline(std::cin, input)){
                processInput(input, stopFlag);
                promptPrinted = false;
            }else{
                break;
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        {
            std::lock_guard<std::mutex> lock(mtx);
            if(stopFlag.load() || !anyRunning()){
                break;
            }
        }
    }

    if(!anyRunning()){
        std::cerr << "All server processes have either exited or been instructed to stop." << std::endl << std::flush;
    }
}
#endif

void mcsm::MultiServerOption::processInput(const std::string& input, std::atomic_bool& stopFlag) const {
    if(input == "stopall"){
        {
            std::lock_guard<std::mutex> lock(mtx);
            std::cerr << "Stopping all servers..." << std::endl << std::flush;
            fclose(stdin);
        }
        for(auto& pair : this->processes){
            if(!pair.second || !pair.second->isActivate()) continue;
            mcsm::Result stop1Res = pair.second->send("stop\n");
            if(!stop1Res.isSuccess()){
                std::cerr << "Stop failed for reason: " << stop1Res.getMessage()[0] << ".\n";
                if(stop1Res.getResult() != mcsm::ResultType::MCSM_FAIL) continue;
                std::cerr << "Force stopping: this might lead to data loss.\n";
                mcsm::Result stop2Res = pair.second->stop();
                if(!stop2Res.isSuccess()){
                    std::cerr << "Failed to force stop the server.\n";
                }
            }else{
                pair.second->closeInputFd();
            }
        }
        stopFlag.store(true);
    }else{
        if(!input.empty()){
            std::lock_guard<std::mutex> lock(mtx);
            std::cerr << "Unknown command \"" << input << "\"." << std::endl << std::flush;
        }
    }
}

void mcsm::MultiServerOption::processMonitor(std::atomic_bool& stopFlag) const {
    while (!stopFlag.load()){
        {
            std::lock_guard<std::mutex> lock(mtx);
            for(auto it = this->processes.begin(); it != this->processes.end(); ){
                if(it->second && !it->second->isActivate()){
                    std::cerr << "[mcsm/INFO] Server " << it->first << " (pid " << it->second->getPID() << ") stopped." << std::endl << std::flush;
                    it = this->processes.erase(it);
                }else if(!it->second){
                    std::cerr << termcolor::bright_yellow << "[mcsm/INFO] Server " << it->first << " (pid " << it->second->getPID() << ") server process instance not valid." << termcolor::reset << std::endl << std::flush;
                    it = this->processes.erase(it);
                }else{
                    ++it;
                }
            }

            if(!anyRunning()){
                stopFlag.store(true);
                fclose(stdin);
                break;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

mcsm::MultiServerOption::~MultiServerOption(){

}