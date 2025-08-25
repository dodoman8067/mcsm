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

mcsm::StringResult mcsm::CustomServer::getFileLocation(const std::string& optionPath) const {
    mcsm::Option option(optionPath, "server");
    auto exists = option.exists();
    if(!exists) return tl::unexpected(exists.error());
    if(!exists.value()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::SERVER_NOT_CONFIGURED, {});
        return tl::unexpected(err);
    }

    auto optLoadRes = option.load(mcsm::GeneralOption::getGeneralOption().advancedParseEnabled());
    if(!optLoadRes) return tl::unexpected(optLoadRes.error());

    auto jarLocVal = option.getValue("jar_location");
    if(!jarLocVal) return tl::unexpected(jarLocVal.error());

    nlohmann::json jarLoc = jarLocVal.value();

    if(jarLoc == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_NOT_FOUND, {"\"jar_location\"", "server.json"});
        return tl::unexpected(err);
    }
    if(!jarLoc.is_string()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_WRONG_TYPE, {"\"jar_location\"", "string"});
        return tl::unexpected(err);
    }
    if(jarLoc != nullptr && (mcsm::startsWith(jarLoc.get<std::string>(), "current") && mcsm::endsWith(jarLoc.get<std::string>(), "current"))){
        return mcsm::getCurrentPath();
    }
    return jarLoc.get<std::string>();
}

mcsm::VoidResult mcsm::CustomServer::setFileLocation(mcsm::Option* option, const std::string& location) {
    mcsm::VoidResult setRes = option->setValue("jar_location", location);
    if(!setRes) return setRes;
    return option->save();
}

mcsm::StringResult mcsm::CustomServer::getCustomStartCommand(const std::string& optionPath) const {
    mcsm::Option option(optionPath, "server");
    auto exists = option.exists();
    if(!exists) return tl::unexpected(exists.error());
    if(!exists.value()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::SERVER_NOT_CONFIGURED, {});
        return tl::unexpected(err);
    }

    auto optLRes = option.load(mcsm::GeneralOption::getGeneralOption().advancedParseEnabled());
    if(!optLRes) return tl::unexpected(optLRes.error());

    auto crcRes = option.getValue("custom_run_command");
    if(!crcRes) return tl::unexpected(crcRes.error());

    nlohmann::json crcVal = crcRes.value();

    if(crcVal == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_NOT_FOUND, {"\"custom_run_command\"", "server.json"});
        return tl::unexpected(err);
    }
    if(!crcVal.is_string()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_WRONG_TYPE, {"\"custom_run_command\"", "string"});
        return tl::unexpected(err);
    }
    std::string value = crcVal;
    return value;
}

mcsm::VoidResult mcsm::CustomServer::setCustomStartCommand(mcsm::Option* option, const std::string& command){
    mcsm::VoidResult setRes = option->setValue("custom_run_command", command);
    if(!setRes) return setRes;
    return option->save();
}

mcsm::VoidResult mcsm::CustomServer::setupServerJarFile(const std::string& path, const std::string& optionPath){
    auto locRes = getFileLocation(optionPath);
    if(!locRes) return tl::unexpected(locRes.error());
    std::string location = locRes.value();

    auto jarRes = getJarFile(optionPath);
    if(!jarRes) return tl::unexpected(jarRes.error());
    std::string jar = jarRes.value();

    // how it works:
    // 1. check if the location is url, will try to download if it is
    // 2. non urls will be treated as files and it will be searched by the program.
    // 3. copy the file to `path` if present, otherwise throw error
    bool url = isURL(location);
    
    if(url){
        return mcsm::download(jar, location, path, true);
    }else{
        auto file = isFile(location);
        if(!file) return tl::unexpected(file.error());
        if(file.value()){
            auto fileExists = mcsm::fileExists(location);
            if(!fileExists) return tl::unexpected(fileExists.error());

            if(!fileExists.value()){
                mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, {700, "Cannot copy a file that doesn't exist.", ""}, {});
                return tl::unexpected(err);
            }

            std::error_code copyEC;
            std::filesystem::copy_file(location, path + "/" + jar, copyEC);
            if(copyEC){
                mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, {700, "Copying jarfile from " + location + " to " + path + "/" + jar + " failed for reason: " + copyEC.message(), ""}, {});
                return tl::unexpected(err);
            }
            return {};
        }
    }
    mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, {700, "The following server jarfile wasn't in a vaild location : " + location, ""}, {});
    return tl::unexpected(err);
}

mcsm::VoidResult mcsm::CustomServer::obtainJarFile(const std::string& /* version */, const std::string& path, const std::string& /* name */, const std::string& optionPath){
    return setupServerJarFile(path, optionPath);
}

mcsm::VoidResult mcsm::CustomServer::generate(const std::string& name, mcsm::JvmOption& option, const std::string& path, const std::string& version, const bool& autoUpdate, const std::map<std::string, std::string>& extraValues){
    return generate(name, option, path, version, autoUpdate, extraValues.find("server_file_location")->second, extraValues);
}

mcsm::VoidResult mcsm::CustomServer::generate(const std::string& name, mcsm::JvmOption& option, const std::string& path, const std::string& /* version */, const bool& /* autoUpdate */, const std::string& fileLocation, const std::map<std::string, std::string>& extraValues){
    mcsm::ServerConfigGenerator serverOption(path);
    mcsm::ServerDataOption sDOpt(path);

    // No need to call opt.load() here. create() in ServerDataOption will call it eventually
    
    mcsm::VoidResult sRes = serverOption.generate("ignored", this, &sDOpt, name, option, false);
    if(!sRes) return sRes;

    mcsm::VoidResult fileRes = setFileLocation(serverOption.getHandle(), fileLocation);
    if(!fileRes) return fileRes;

    std::string customCommand = extraValues.find("custom_run_command")->second;

    mcsm::VoidResult cRCRes = setCustomStartCommand(serverOption.getHandle(), customCommand);
    if(!cRCRes) return cRCRes;

    mcsm::ServerConfigLoader loader(path);
    
    mcsm::VoidResult loadRes = loader.loadConfig();
    if(!loadRes) return loadRes;

    mcsm::success("Custom configured server's information : ");
    mcsm::info("Server name : " + mcsm::safeString(name));
    mcsm::info("Server type : custom");
    if(!mcsm::isWhitespaceOrEmpty(customCommand)){
        mcsm::info("Server run command : " + customCommand);
        mcsm::warning("This overrides JVM launch profile based server launch system.");
        mcsm::warning("Please leave it empty if you don't know what you're doing.");
    }else{
        mcsm::info("Server JVM launch profile : " + option.getProfileName());
    }
    mcsm::warning("NOTE: Custom servers are currently in beta state.");
    mcsm::warning("We are not responsible for the consequences of using beta features.");

    return {};
}

mcsm::StringResult mcsm::CustomServer::start(mcsm::ServerConfigLoader* loader, mcsm::JvmOption& option, const std::string& path, const std::string& optionPath){
    // ServerOption class handles the data file stuff

    mcsm::StringResult customCommand = getCustomStartCommand(loader->getHandle()->getPath());
    if(!customCommand) return customCommand;

    if(!mcsm::isWhitespaceOrEmpty(customCommand.value())){
        mcsm::info("NOTE: JVM profile based launch system is currently overrided by \"custom_run_command\" value inside server.json.");
        mcsm::info("Leave it empty to use default launch system.");
        mcsm::info("Running command : " + customCommand.value());
        mcsm::IntResult result = mcsm::runCommand(customCommand.value());
        if(!result) return tl::unexpected(result.error());
        if(result.value() != 0){
            return "\033[38;2;255;0;0mServer exited with error code : " + std::to_string(result.value());
        }
        return "\033[38;2;0;255;0mServer exited with error code : 0";
    }
    
    mcsm::StringResult jar = loader->getServerJarFile();
    if(!jar) return jar;

    mcsm::BoolResult fileExists = mcsm::fileExists(path + "/" + jar.value());
    if(!fileExists) return tl::unexpected(fileExists.error());

    if(!fileExists.value()){
        mcsm::info("Setting up " + jar.value() + "...");
        mcsm::info("\"server_jar\" will be used as the copied/downloaded file name. Make sure you don't have characters like \"/\".");
        mcsm::StringResult sVer = loader->getServerVersion();
        if(!sVer) return sVer;

        mcsm::VoidResult res = setupServerJarFile(path, optionPath);
        if(!res) return tl::unexpected(res.error());
    }
    return Server::start(loader, option, path, optionPath);
}

mcsm::BoolResult mcsm::CustomServer::isFile(const std::string& location) const {
    std::error_code ec;
    bool isRegularFile = std::filesystem::is_regular_file(location, ec);
    if(ec){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::FILE_EXIST_CHECK_FAILED, {location + " is a file", ec.message()});
        return tl::unexpected(err);
    }
    return isRegularFile;
}

bool mcsm::CustomServer::isURL(const std::string& location) const {
    std::regex urlPattern(
        R"(^https?://[0-9a-z\.-]+(:[1-9][0-9]*)?(/[^\s]*)*$)"
    );
    return std::regex_match(location, urlPattern);
}

const tl::expected<std::map<std::string, std::string>, mcsm::Error> mcsm::CustomServer::getRequiredValues() const {
    return tl::expected<std::map<std::string, std::string>, mcsm::Error>{
        std::map<std::string, std::string>{
                {"name", "" },
                {"default_jvm_launch_profile_search_path", "current"}, // (current/global)
                {"default_jvm_launch_profile_name", ""},
                {"server_jarfile_name", "custom.jar"},
                {"server_file_location", ""}, // (url/filepath)
                {"custom_run_command", ""} // Overrides server JVM profile based start system.
        }
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