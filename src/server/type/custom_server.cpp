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

#include <mcsm/server/type/custom_server.h>
#include <mcsm/data/options/general_option.h>

mcsm::CustomServer::CustomServer(){

}

mcsm::CustomServer::~CustomServer(){

}

mcsm::ServerType mcsm::CustomServer::getType() const {
    return mcsm::ServerType::CUSTOM;
}

std::string mcsm::CustomServer::getTypeAsString() const {
    return "custom";
}

std::string mcsm::CustomServer::getSupportedVersions() const {
    return "unknown";
}

std::string mcsm::CustomServer::getFileLocation(const std::string& optionPath) const {
    mcsm::Option option(optionPath, "server");
    bool exists = option.exists();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";
    if(!exists){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverNotConfigured()});
        return "";
    }

    option.load(mcsm::GeneralOption::getGeneralOption().advancedParseEnabled());
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";

    if(option.getValue("jar_location") == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFound("\"jar_location\"", "server.json")});
        return "";
    }
    if(!option.getValue("jar_location").is_string()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"jar_location\"", "string")});
        return "";
    }
    std::string value = option.getValue("jar_location");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";
    if(mcsm::startsWith(value, "current") && mcsm::endsWith(value, "current")){
        return mcsm::getCurrentPath();
    }
    return value;
}

mcsm::Result mcsm::CustomServer::setFileLocation(mcsm::Option* option, const std::string& location) {
    mcsm::Result setRes = option->setValue("jar_location", location);
    if(!setRes.isSuccess()) return setRes;
    return option->save();
}

mcsm::Result mcsm::CustomServer::setupServerJarFile(const std::string& path, const std::string& optionPath){
    std::string location = getFileLocation(optionPath);
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }
    std::string jar = getJarFile(optionPath);

    // how it works:
    // 1. check if the location is url, will try to download if it is
    // 2. non urls will be treated as files and it will be searched by the program.
    // 3. copy the file to `path` if present, otherwise throw error
    bool url = isURL(location);
    
    if(url){
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
            std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
            mcsm::Result res(resp.first, resp.second);
            return res;
        }
        return mcsm::download(jar, location, path, true);
    }else{
        bool file = isFile(location);
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
            std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
            mcsm::Result res(resp.first, resp.second);
            return res;
        }
        if(file){
            bool fileExists = mcsm::fileExists(location);
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
                std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
                mcsm::Result res(resp.first, resp.second);
                return res;
            }

            if(!fileExists){
                mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
                    "Cannot copy a file that doesn't exist.",
                    "Please report this to GitHub (https://github.com/dodoman8067/mcsm) if you think this is a software issue."
                }});
                return res;
            }

            std::error_code copyEC;
            std::filesystem::copy_file(location, path + "/" + jar, copyEC);
            if(copyEC){
                mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
                    "Copying jarfile from " + location + " to " + path + "/" + jar + " failed for reason: " + copyEC.message(),
                    "Please report this to GitHub (https://github.com/dodoman8067/mcsm) if you think this is a software issue."
                }});
                return res;
            }

            return {mcsm::ResultType::MCSM_SUCCESS, {"Success"}};
        }
    }
    mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
        "The following server jarfile wasn't in a vaild location : " + location,
        "Please report this to GitHub (https://github.com/dodoman8067/mcsm) if you think this is a software issue."
    }});
    return res;
}

mcsm::Result mcsm::CustomServer::obtainJarFile(const std::string& /* version */, const std::string& path, const std::string& /* name */, const std::string& optionPath){
    return setupServerJarFile(path, optionPath);
}

mcsm::Result mcsm::CustomServer::generate(const std::string& name, mcsm::JvmOption& option, const std::string& path, const std::string& version, const bool& autoUpdate, const std::map<std::string, std::string>& extraValues){
    return generate(name, option, path, version, autoUpdate, extraValues.find("server file location (url/filepath)")->second, extraValues);
}

mcsm::Result mcsm::CustomServer::generate(const std::string& name, mcsm::JvmOption& option, const std::string& path, const std::string& /* version */, const bool& /* autoUpdate */, const std::string& fileLocation, const std::map<std::string, std::string>& extraValues){
    mcsm::ServerConfigGenerator serverOption(path);
    mcsm::ServerDataOption sDOpt(path);

    // No need to call opt.load() here. create() in ServerDataOption will call it eventually
    
    mcsm::Result sRes = serverOption.generate("ignored", this, &sDOpt, name, option, false);
    if(!sRes.isSuccess()) return sRes;

    mcsm::Result fileRes = setFileLocation(&(*serverOption.getHandle()), fileLocation);
    if(!fileRes.isSuccess()) return fileRes;

    mcsm::ServerConfigLoader loader(path);
    
    mcsm::Result loadRes = loader.loadConfig();
    if(!loadRes.isSuccess()) return loadRes;

    mcsm::success("Custom configured server's information : ");
    mcsm::info("Server name : " + mcsm::safeString(name));
    mcsm::info("Server type : custom");
    mcsm::info("Server JVM launch profile : " + option.getProfileName());
    mcsm::warning("NOTE: Custom servers are currently in beta state.");
    mcsm::warning("We are not responsible for the consequences of using beta features.");

    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
    return res;
}

mcsm::Result mcsm::CustomServer::start(mcsm::ServerConfigLoader* loader, mcsm::JvmOption& option, const std::string& path, const std::string& optionPath){
    // ServerOption class handles the data file stuff
    
    std::string jar = loader->getServerJarFile();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    bool fileExists = mcsm::fileExists(path + "/" + jar);
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    if(!fileExists){
        mcsm::info("Setting up " + jar + "...");
        mcsm::info("\"server_jar_name\" will be used as the copied/downloaded file name. Make sure you don't have characters like \"/\".");
        std::string sVer = loader->getServerVersion();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
            std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
            mcsm::Result res(resp.first, resp.second);
            return res;
        }

        mcsm::Result res = setupServerJarFile(path, optionPath);
        if(!res.isSuccess()) return res;
    }
    return Server::start(loader, option, path, optionPath);
}

bool mcsm::CustomServer::isFile(const std::string& location) const {
    std::error_code ec;
    bool isRegularFile = std::filesystem::is_regular_file(location, ec);
    if(ec){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
            "Checking if " + location + " is a file operation failed : " + ec.message(), 
            "Please report this to GitHub (https://github.com/dodoman8067/mcsm) if you think this is a software issue."
            }});
        return false;
    }
    return isRegularFile;
}


//TODO: Update regex. the program thinks url is a file rn
bool mcsm::CustomServer::isURL(const std::string& location) const {
    std::regex urlPattern(
        R"(^https?://[0-9a-z\.-]+(:[1-9][0-9]*)?(/[^\s]*)*$)"
    );
    return std::regex_match(location, urlPattern);
}

const std::map<std::string, std::string> mcsm::CustomServer::getRequiredValues() const {
    return {
        {"name", "" },
        {"default JVM launch profile search path (current/global)", "current"},
        {"default JVM launch profile name", ""},
        {"server jarfile name", "custom.jar"},
        {"server file location (url/filepath)", ""}
    };
}

std::string mcsm::CustomServer::getBasedServer() const {
    return "unknown";
}

bool mcsm::CustomServer::isBasedAs(const std::string& /* input */) const {
    return false;
}

std::string mcsm::CustomServer::getWebSite() const {
    return "unknown";
}

std::string mcsm::CustomServer::getGitHub() const {
    return "unknown";
}