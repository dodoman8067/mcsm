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

mcsm::MultiServerOption::~MultiServerOption(){

}