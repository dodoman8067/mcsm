#include <mcsm/command/server/clear_server_command.h>

mcsm::ClearServerCommand::ClearServerCommand(const std::string& name, const std::string& description) : mcsm::Command(name, description) {}

void mcsm::ClearServerCommand::execute(const std::vector<std::string>& /* args */){
    if(!isConfigured()){
        mcsm::warning("Server not configured.");
        mcsm::warning("Task aborted.");
        std::exit(1);
    }
    const std::string& path = mcsm::getCurrentPath();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        mcsm::printResultMessage();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_WARN_NOEXIT) std::exit(1);
    }

    mcsm::ServerConfigLoader loader(path);
    mcsm::Result loadRes = loader.loadConfig();
    if(!loadRes.isSuccess()){
        loadRes.printMessage();
        if(loadRes.getResultPair().first != mcsm::ResultType::MCSM_WARN_NOEXIT) std::exit(1);
    }

    const std::string& jarName = loader.getServerJarFile();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        mcsm::printResultMessage();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_WARN_NOEXIT) std::exit(1);
    }

    bool file = mcsm::fileExists(loader.getHandle()->getPath() + "/" + jarName);
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        mcsm::printResultMessage();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_WARN_NOEXIT) std::exit(1);
    }

    if(file) {
        mcsm::removeFile(loader.getHandle()->getPath() + "/" + jarName);
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
            mcsm::printResultMessage();
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_WARN_NOEXIT) std::exit(1);
        }
    }else mcsm::info(jarName + " file not found; skipping...");

    const std::string& type = loader.getServerType();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        mcsm::printResultMessage();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_WARN_NOEXIT) std::exit(1);
    }

    if(type == "fabric"){
        mcsm::FabricServerDataOption* option = new mcsm::FabricServerDataOption(path);
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
            mcsm::printResultMessage();
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_WARN_NOEXIT) std::exit(1);
        }

        mcsm::Result res1 = option->updateLastDownloadedBuild("");
        if(!res1.isSuccess()){
            res1.printMessage();
            if(res1.getResultPair().first != mcsm::ResultType::MCSM_WARN_NOEXIT) std::exit(1);
        }

        mcsm::Result res2 = option->updateInstallerVersion("");
        if(!res2.isSuccess()){
            res2.printMessage();
            if(res2.getResultPair().first != mcsm::ResultType::MCSM_WARN_NOEXIT) std::exit(1);
        }

        mcsm::Result res3 = option->updateLoaderVersion("");
        if(!res3.isSuccess()){
            res3.printMessage();
            if(res3.getResultPair().first != mcsm::ResultType::MCSM_WARN_NOEXIT) std::exit(1);
        }

        option = nullptr;
        delete option;
    }else{
        mcsm::ServerDataOption* option = new mcsm::ServerDataOption(path);
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
            mcsm::printResultMessage();
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_WARN_NOEXIT) std::exit(1);
        }

        mcsm::Result res1 = option->updateLastDownloadedBuild("");
        if(!res1.isSuccess()){
            res1.printMessage();
            if(res1.getResultPair().first != mcsm::ResultType::MCSM_WARN_NOEXIT) std::exit(1);
        }

        option = nullptr;
        delete option;
    }
    mcsm::success("Clean sucessful.");
}

inline bool mcsm::ClearServerCommand::isConfigured(){
    std::string path = mcsm::getCurrentPath();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        mcsm::printResultMessage();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_WARN_NOEXIT) std::exit(1);
    }
    bool fileExists = mcsm::fileExists(path + "/server.json");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        mcsm::printResultMessage();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_WARN_NOEXIT) std::exit(1);
    }
    return fileExists;
}