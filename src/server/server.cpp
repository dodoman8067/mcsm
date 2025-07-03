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

mcsm::StringResult mcsm::Server::start(mcsm::ServerConfigLoader* loader, mcsm::JvmOption& option){
    return start(loader, option, mcsm::getCurrentPath(), mcsm::getCurrentPath());
}

mcsm::StringResult mcsm::Server::start(mcsm::ServerConfigLoader* loader, mcsm::JvmOption& option, const std::string& path, const std::string& optionPath){
    std::string jvmOpt = " ";
    auto jArgs = option.getJvmArguments();
    if(!jArgs) return jArgs;

    for(auto& s : jArgs.value()){
        jvmOpt = jvmOpt + s + " ";
    }

    std::string svrOpt = " ";
    auto sArgs = option.getServerArguments();
    if(!sArgs) return sArgs;

    for(auto& s : sArgs.value()){
        svrOpt = svrOpt + s + " ";
    }

    mcsm::StringResult jPath = option.getJvmPath();
    if(!jPath) return jPath;

    mcsm::StringResult jar = loader->getServerJarFile();
    if(!jar) return jar;

    std::string command = jPath.value() + jvmOpt + mcsm::normalizePath(path) + "/" + jar.value() + svrOpt;
    mcsm::info("Running command : " + command);
    
    std::error_code ec;
    std::filesystem::current_path(optionPath, ec);
    if(ec){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, {700, "Server starting failed : %s", ""}, {ec.message()});
        return tl::unexpected(err);
    }

    int result = mcsm::runCommand(command);
    if(result != 0){
        return "\033[38;2;255;0;0mServer exited with error code : " + std::to_string(result);
    }
    return "\033[38;2;0;255;0mServer exited with error code : 0";
}

mcsm::VoidResult mcsm::Server::configure(const std::string &version, mcsm::Server* server, mcsm::ServerDataOption *sDataOpt, const std::string& path, const std::string& name, mcsm::JvmOption& option, const bool& autoUpdate){
    return configure(version, server, sDataOpt, path, name, option, autoUpdate, "latest");
}

mcsm::VoidResult mcsm::Server::configure(const std::string &version, mcsm::Server* server, mcsm::ServerDataOption *sDataOpt, const std::string& path, const std::string& name, mcsm::JvmOption& option, const bool& autoUpdate, const std::string& build){
    mcsm::ServerConfigGenerator serverOption(path);
    
    mcsm::VoidResult sRes = serverOption.generate(version, server, sDataOpt, name, option, autoUpdate, build);
    if(!sRes) return sRes;

    mcsm::ServerConfigLoader loader(path);
    
    mcsm::VoidResult loadRes = loader.loadConfig();
    if(!loadRes) return loadRes;

    mcsm::success("Configured server's information : ");
    mcsm::info("Server name : " + mcsm::safeString(name));
    mcsm::info("Server type : " + server->getTypeAsString());
    mcsm::info("Server version : " + version);
    mcsm::info("Server build version : " + build);
    mcsm::info("Server JVM launch profile : " + option.getProfileName());
    if(!autoUpdate) mcsm::info("Automatic updates : disabled");

    return {};
}

mcsm::StringResult mcsm::Server::getJarFile() const {
    auto path = mcsm::getCurrentPath();
    if(!path) return path;
    return getJarFile(path.value());
}

mcsm::StringResult mcsm::Server::getJarFile(const std::string& checkDir) const {
    //this method is usually for getting the configured server's jarfile.
    mcsm::Option opt(checkDir, "server");
    mcsm::BoolResult exists = opt.exists();
    if(!exists){
        return tl::unexpected(exists.error());
    }

    mcsm::VoidResult optLoadRes = opt.load(mcsm::GeneralOption::getGeneralOption().advancedParseEnabled());
    if(!optLoadRes){
        return tl::unexpected(optLoadRes.error());
    }

    if(exists.value()){
        auto value = opt.getValue("server_jar_name");
        if(!value) return value;

        if(value.value() == nullptr){
            mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_NOT_FOUND, {"\"server_jar_name\"", opt.getName()});
            return tl::unexpected(err);
        }

        if(!value.value().is_string()){
            mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_WRONG_TYPE, {"\"server_jar_name\"", "string"});
            return tl::unexpected(err);
        }
        return value.get<std::string>();
    }
    return getTypeAsString() + ".jar";
}

bool mcsm::Server::isBasedAs(const std::string& input) const {
    if(!mcsm::isWhitespaceOrEmpty(getBasedServer())) return false;
    return getBasedServer() == input;
}

const tl::expected<std::map<std::string, std::string>, mcsm::Error> mcsm::Server::getRequiredValues() const {
    return {
        {"name", "" },
        {"minecraft_version", ""},
        {"default_jvm_launch_profile_search_path", "current"},
        {"default_jvm_launch_profile_name", ""},
        {"server_jarfile_name", getTypeAsString() + ".jar"},
        {"server_build_version", "latest"},
        {"auto_server_jar_update", "true"}
    };
}