#include "tl/expected.hpp"
#include <mcsm/jvm/jvm_option.h>
#include <mcsm/server/server.h>
#include <mcsm/data/options/server_starter.h>
#include <mcsm/server/server_registry.h>

mcsm::ServerStarter::ServerStarter(mcsm::ServerConfigLoader* loader){
    this->loader = loader;
}
        
mcsm::ServerStarter::~ServerStarter(){

}

mcsm::VoidResult mcsm::ServerStarter::startServer(mcsm::JvmOption& option, const std::string& path, const std::vector<std::string>& cliArgs, const std::string& optionPath){
    mcsm::ServerDataOption serverDataOpt(optionPath);

    auto sLoadRes = serverDataOpt.load();
    if(!sLoadRes) return sLoadRes;
    auto fileExists = mcsm::fileExists(optionPath + "/server.toml");
    if(!fileExists) return tl::unexpected(fileExists.error());

    if(!fileExists.value()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::SERVER_NOT_CONFIGURED, {optionPath});
        return tl::unexpected(err);
    }

    std::cout << this->loader->getServerJar().value() << "\n";

    auto server = this->loader->getServerType();
    if(!server) return tl::unexpected(server.error());

    auto sp = mcsm::ServerRegistry::getServerRegistry().getServer(server.value(), *loader);
    if(!sp) return tl::unexpected(sp.error());

    auto name = this->loader->getServerName();
    if(!name) return tl::unexpected(name.error());

    auto version = this->loader->getServerVersion();
    if(!version) return tl::unexpected(version.error());

    const std::string& profileName = option.getProfileName();

    mcsm::success("Starting server..");
    mcsm::info("Server name : " + name.value());
    mcsm::info("Server MC version : " + version.value());
    mcsm::info("Server JVM launch profile : " + profileName);
    mcsm::VoidResult res = serverDataOpt.updateLastTimeLaunched();
    if(!res) return res;
    mcsm::StringResult res2 = sp.value()->start(option, mcsm::normalizePath(path), mcsm::normalizePath(optionPath), cliArgs);
    if(!res2) return tl::unexpected(res2.error());
    return {};
}

mcsm::VoidResult mcsm::ServerStarter::startServer(mcsm::JvmOption& option, const std::string& path, const std::vector<std::string>& cliArgs, const std::string& optionPath, const std::string& groupOptionPath) {
    mcsm::ServerGroupLoader gLoader(groupOptionPath);
    auto gLoadRes = gLoader.load();
    if(!gLoadRes){
        mcsm::printError(gLoadRes.error());
        mcsm::exitIfFail(gLoadRes.error());
    }

    std::vector<const mcsm::ServerConfigLoader*> gServers = gLoader.getServers();
    auto cPath = mcsm::getCurrentPath();
    if(!cPath) return tl::unexpected(cPath.error());

    for(const mcsm::ServerConfigLoader* gServer : gServers){
        if(gServer == nullptr) continue;
        if(gServer->getHandle()->getPath() != cPath.value()) continue;
        // add server session file
        return startServer(option, path, cliArgs, optionPath);
        // remove server session file
    }

    mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, {700, "Failed to validate server group " + groupOptionPath, ""}, {});
    return tl::unexpected(err);
}