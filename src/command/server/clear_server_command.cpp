#include <mcsm/command/server/clear_server_command.h>
#include <mcsm/data/options/general_option.h>

mcsm::ClearServerCommand::ClearServerCommand(const std::string& name, const std::string& description) : mcsm::Command(name, description) {}

void mcsm::ClearServerCommand::execute(const std::vector<std::string>& /* args */){
    if(!isConfigured()){
        mcsm::warning("Server not configured.");
        mcsm::warning("Task aborted.");
        std::exit(1);
    }
    const std::string& path = mcsm::unwrapOrExit(mcsm::getCurrentPath());

    mcsm::ServerConfigLoader loader(path);
    mcsm::unwrapOrExit(loader.loadConfig());

    // we need server jarfile's name and validate if the file is actually present

    const std::string& jarName = mcsm::unwrapOrExit(loader.getServerJar());

    bool file = mcsm::unwrapOrExit(mcsm::fileExists(jarName));

    if(file) {
        mcsm::unwrapOrExit(mcsm::removeFile(jarName));
    }else mcsm::info(jarName + " file not found; skipping...");

    const std::string& type = mcsm::unwrapOrExit(loader.getServerType()); // fabric servers require additional steps to remove jarfile which is why i need to get the server's type

    bool advp = mcsm::GeneralOption::getGeneralOption().advancedParseEnabled();
    if(type == "fabric"){
        mcsm::FabricServerDataOption option(path);

        // reset all server launch related datas

        mcsm::unwrapOrExit(option.load(advp));
        mcsm::unwrapOrExit(option.updateLastDownloadedBuild(""));
        mcsm::unwrapOrExit(option.updateInstallerVersion(""));
        mcsm::unwrapOrExit(option.updateLoaderVersion(""));
    }else{
        mcsm::ServerDataOption option(path);

        mcsm::unwrapOrExit(option.load(advp));
        mcsm::unwrapOrExit(option.updateLastDownloadedBuild(""));
    }
    mcsm::success("Clean sucessful.");
}

inline bool mcsm::ClearServerCommand::isConfigured(){
    std::string path = mcsm::unwrapOrExit(mcsm::getCurrentPath());
    bool fileExists = mcsm::unwrapOrExit(mcsm::fileExists(path + "/server.json"));
    return fileExists;
}