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

#include <mcsm/server/server.h>
#include <mcsm/data/options/general_option.h>

mcsm::Result mcsm::Server::start(mcsm::ServerConfigLoader* loader, mcsm::JvmOption& option){
    return start(loader, option, mcsm::getCurrentPath(), mcsm::getCurrentPath());
}

mcsm::Result mcsm::Server::start(mcsm::ServerConfigLoader* loader, mcsm::JvmOption& option, const std::string& path, const std::string& optionPath){
    std::string jvmOpt = " ";
    auto jArgs = option.getJvmArguments();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    for(auto& s : jArgs){
        jvmOpt = jvmOpt + s + " ";
    }

    std::string svrOpt = " ";
    auto sArgs = option.getServerArguments();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    for(auto& s : sArgs){
        svrOpt = svrOpt + s + " ";
    }

    std::string jPath = option.getJvmPath();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    std::string jar = loader->getServerJarFile();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    std::string command = jPath + jvmOpt + path + "/" + jar + svrOpt;
    mcsm::info("Running command : " + command);
    
    std::error_code ec;
    std::filesystem::current_path(optionPath, ec);
    if(ec){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
            "Server starting failed : " + ec.message(),
            "Please report this to GitHub (https://github.com/dodoman8067/mcsm) if you believe that this is a software issue."
        }});
        return res;
    }

    int result = mcsm::runCommand(command.c_str());
    if(result != 0){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
            "Server exited with error code : " + std::to_string(result)
        }});
        return res;
    }
    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {
        "Server exited with error code : 0"
    }});
    return res;
}

mcsm::Result mcsm::Server::configure(const std::string &version, mcsm::Server* server, mcsm::ServerDataOption *sDataOpt, const std::string& path, const std::string& name, mcsm::JvmOption& option, const bool& autoUpdate){
    mcsm::ServerConfigGenerator serverOption(path);
    
    mcsm::Result sRes = serverOption.generate(version, server, sDataOpt, name, option, autoUpdate);
    if(!sRes.isSuccess()) return sRes;

    mcsm::ServerConfigLoader loader(path);
    
    mcsm::Result loadRes = loader.loadConfig();
    if(!loadRes.isSuccess()) return loadRes;

    mcsm::success("Configured server's information : ");
    mcsm::info("Server name : " + mcsm::safeString(name));
    mcsm::info("Server type : " + server->getTypeAsString());
    mcsm::info("Server version : " + version);
    mcsm::info("Server JVM launch profile : " + option.getProfileName());
    if(!autoUpdate) mcsm::info("Automatic updates : disabled");

    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
    return res;
}

std::string mcsm::Server::getJarFile() const {
    std::string path = mcsm::getCurrentPath();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";
    return getJarFile(path);
}

std::string mcsm::Server::getJarFile(const std::string& checkDir) const {
    //this method is usually for getting the configured server's jarfile.
    mcsm::Option opt(checkDir, "server");
    bool exists = opt.exists();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";

    opt.load(mcsm::GeneralOption::getGeneralOption().advancedParseEnabled());
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";

    if(exists){
        nlohmann::json value = opt.getValue("server_jar_name");
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";

        if(value == nullptr){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFound("\"server_jar_name\"", opt.getName())});
            return "";
        }

        if(!value.is_string()){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"server_jar_name\"", "string")});
            return "";
        }
        mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
        return value;
    }
    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
    return getTypeAsString() + ".jar";
}

bool mcsm::Server::isBasedAs(const std::string& input) const {
    if(!mcsm::isWhitespaceOrEmpty(getBasedServer())) return false;
    return getBasedServer() == input;
}