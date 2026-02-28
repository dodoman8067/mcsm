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
#include <toml++/impl/table.hpp>

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
    mcsm::TomlOption option(optionPath, "server");
    auto exists = option.exists();
    if(!exists) return tl::unexpected(exists.error());
    if(!exists.value()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::SERVER_NOT_CONFIGURED, {optionPath});
        return tl::unexpected(err);
    }

    auto optLoadRes = option.load(mcsm::GeneralOption::getGeneralOption().advancedParseEnabled());
    if(!optLoadRes) return tl::unexpected(optLoadRes.error());

    auto customSpecific = option.getValue("custom");
    if(!customSpecific) return tl::unexpected(customSpecific.error());
    if(customSpecific.value() == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::TOML_NOT_FOUND, {"\"custom\"", option.getName()});
        return tl::unexpected(err);
    }
    if(!customSpecific.value()->is_table()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::TOML_WRONG_TYPE, {"\"custom\"", "table"});
        return tl::unexpected(err);
    }

    toml::table tCustom = *customSpecific.value()->as_table();
    if(!tCustom.contains("jarfile_source_location")){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::TOML_NOT_FOUND, {"\"jarfile_source_location\"", option.getName()});
        return tl::unexpected(err);     
    }
    if(!tCustom.get("jarfile_source_location")->is_string()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::TOML_WRONG_TYPE, {"\"jarfile_source_location\"", "string"});
        return tl::unexpected(err);
    }

    auto jarLocVal = *tCustom.get("jarfile_source_location")->as_string();
    if(mcsm::startsWith(jarLocVal.get(), "current") && mcsm::endsWith(jarLocVal.get(), "current")){
        return mcsm::getCurrentPath();
    }
    return jarLocVal.get();
}

mcsm::VoidResult mcsm::CustomServer::setFileLocation(mcsm::TomlOption* option, const std::string& location) {
    toml::table tCustom;
    tCustom.insert_or_assign("jarfile_source_location", location);
    mcsm::VoidResult setRes = option->setValue("custom", tCustom);
    if(!setRes) return setRes;
    return option->save();
}

mcsm::VoidResult mcsm::CustomServer::setupServerJarFile(const std::string& jarName, const std::string& path, const std::string& optionPath){
    auto locRes = getFileLocation(optionPath);
    if(!locRes) return tl::unexpected(locRes.error());
    std::string location = locRes.value();

    // how it works:
    // 1. check if the location is url, will try to download if it is
    // 2. non urls will be treated as files and it will be searched by the program.
    // 3. copy the file to `path` if present, otherwise throw error
    bool url = isURL(location);
    
    if(url){
        return mcsm::download(jarName, location, path, true);
    }else{
        auto file = isFile(location);
        if(!file) return tl::unexpected(file.error());
        if(file.value()){
            auto fileExists = mcsm::fileExists(location);
            if(!fileExists) return tl::unexpected(fileExists.error());

            if(!fileExists.value()){
                mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, {700, "Cannot copy a file that doesn't exist.", ""}, {});
                return tl::unexpected(err);
            }

            std::error_code copyEC;
            std::filesystem::copy_file(location, mcsm::joinPath(path, jarName), copyEC);
            if(copyEC){
                mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, {700, "Copying jarfile from " + location + " to " + path + "/" + jarName + " failed for reason: " + copyEC.message(), ""}, {});
                return tl::unexpected(err);
            }
            return {};
        }
    }
    mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, {700, "The following server jarfile wasn't in a vaild location : " + location, ""}, {});
    return tl::unexpected(err);
}

mcsm::VoidResult mcsm::CustomServer::obtainJarFile(const std::string& /* version */, const std::string& path, const std::string& name, const std::string& optionPath){
    return setupServerJarFile(name, path, optionPath);
}

mcsm::VoidResult mcsm::CustomServer::generate(const std::string& name, mcsm::JvmOption& option, const std::string& path, const std::string& version, const bool& autoUpdate, const std::map<std::string, std::string>& extraValues){
    return generate(name, option, path, version, autoUpdate, extraValues.find("server_file_location")->second, extraValues);
}

mcsm::VoidResult mcsm::CustomServer::generate(const std::string& name, mcsm::JvmOption& option, const std::string& path, const std::string& /* version */, const bool& /* autoUpdate */, const std::string& fileLocation, const std::map<std::string, std::string>& /* extraValues */){
    mcsm::ServerConfigGenerator serverOption(path);
    mcsm::ServerDataOption sDOpt(path);

    // No need to call opt.load() here. create() in ServerDataOption will call it eventually
    
    mcsm::VoidResult sRes = serverOption.generate("ignored", this, &sDOpt, name, option, false);
    if(!sRes) return sRes;

    mcsm::VoidResult fileRes = setFileLocation(serverOption.getHandle(), fileLocation);
    if(!fileRes) return fileRes;

    mcsm::ServerConfigLoader loader(path);
    
    mcsm::VoidResult loadRes = loader.loadConfig();
    if(!loadRes) return loadRes;

    mcsm::success("Custom configured server's information : ");
    mcsm::info("Server name : " + mcsm::safeString(name));
    mcsm::info("Server type : custom");
    mcsm::info("Server JVM launch profile : " + option.getProfileName());
    return {};
}

mcsm::StringResult mcsm::CustomServer::start(mcsm::JvmOption& option, const std::string& path, const std::string& optionPath){
    return start(option, path, optionPath, {});
}

mcsm::StringResult mcsm::CustomServer::start(mcsm::JvmOption& option, const std::string& path, const std::string& optionPath, const std::vector<std::string>& cliArgs){
    // ServerOption class handles the data file stuff
/*
    mcsm::StringResult customCommand = getCustomStartCommand(loader.getHandle()->getPath());
    if(!customCommand) return customCommand;

    bool hasIgnoreFlag = false;
    for(auto& str : cliArgs){
        if(str == "--force-default-launch-command" || str == "-force-default-launch-command"){
            hasIgnoreFlag = true;
        }
    }

    if(!mcsm::isWhitespaceOrEmpty(customCommand.value()) && !hasIgnoreFlag){
        mcsm::info("NOTE: JVM profile based launch system is currently overridden by \"custom_run_command\" value inside server.json.");
        mcsm::info("Leave it empty to use default launch system.");
        mcsm::info("Running command : " + customCommand.value());
        mcsm::IntResult result = mcsm::runCommand(customCommand.value());
        if(!result) return tl::unexpected(result.error());
        if(result.value() != 0){
            return "\033[38;2;255;0;0mServer exited with error code : " + std::to_string(result.value());
        }
        return "\033[38;2;0;255;0mServer exited with error code : 0";
    }else if(hasIgnoreFlag){
        mcsm::warning("\"custom_run_command\" value temporarily ignored by --force-default-launch-command flag.");
    }
        */
    
    mcsm::StringResult jar = loader.getServerJarFile();
    if(!jar) return jar;

    mcsm::BoolResult fileExists = mcsm::fileExists(path + "/" + jar.value());
    if(!fileExists) return tl::unexpected(fileExists.error());

    auto locRes = getFileLocation(optionPath);
    if(!locRes) return tl::unexpected(locRes.error());
    std::string location = locRes.value();

    if(!fileExists.value()){
        mcsm::info("Setting up jarfile in " + path + "/" + jar.value() + " from " + location + "...");
        mcsm::info("\"server_jar\" will be used as the copied/downloaded file path. File's name must be included at the end in order to store the file at specified path.");
        mcsm::StringResult sVer = loader.getServerVersion();
        if(!sVer) return sVer;

        mcsm::VoidResult res = setupServerJarFile(jar.value(), path, optionPath);
        if(!res) return tl::unexpected(res.error());
    }
    return Server::start(option, path, optionPath, cliArgs);
}

mcsm::BoolResult mcsm::CustomServer::isFile(const std::string& location) const {
    std::error_code ec;
    bool isRegularFile = std::filesystem::is_regular_file(location, ec);
    if(ec){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::FILE_EXIST_CHECK_FAILED, {location + " is a file", ec.message()});
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
                {"server_jarfile", "custom.jar"},
                {"server_file_location", ""}, // (url/filepath)
                {"custom_run_command", ""} // Overrides server JVM profile based start system.
        }
    };
}

const tl::expected<std::vector<mcsm::ServerOptionSpec>, mcsm::Error> mcsm::CustomServer::getRequiredOptions() const {
    std::vector<mcsm::ServerOptionSpec> spec = {
        {
            .key = "name",
            .type = mcsm::OptionType::STRING,
            .required = true,
            .defaultValue = ""
        },
        {
            .key = "default_jvm_launch_profile_search_path",
            .type = mcsm::OptionType::ENUM,
            .required = false,
            .defaultValue = "current",
            .enumValues = { "current", "global" }
        },
        {
            .key = "default_jvm_launch_profile_name",
            .type = mcsm::OptionType::STRING,
            .required = true,
            .defaultValue = ""
        },
        {
            .key = "server_jarfile",
            .type = mcsm::OptionType::STRING,
            .required = false,
            .defaultValue = getTypeAsString() + ".jar"
        },
        {
            .key = "server_file_location",
            .type = mcsm::OptionType::ENUM,
            .required = true,
            .defaultValue = "",
            .enumValues = {"url", "filepath"}
        }
    };
    return spec;
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