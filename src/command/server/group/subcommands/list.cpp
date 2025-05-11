#include <mcsm/command/server/group/subcommands/list.h>

mcsm::GroupListSubCommand::GroupListSubCommand(mcsm::ServerGroupManager* manager): manager(manager){}

void mcsm::GroupListSubCommand::execute(const std::vector<std::string>& args){
    bool running = false;
    if(!args.empty()){
        for(auto& str : args){
            if(str == "-r" || str == "--r" || str == "-running" || str == "--running"){
                running = true;
            }else{
                mcsm::info("Unknown value: " + str);
                mcsm::info("Avaliable options: --running and --r");
                std::exit(1);
            }
        }
    }

    std::map<std::string, bool> strmap; // server name(server path) and bool to check if it's running
    auto arr = manager->getGroupLoader()->getServers();
    auto arr2 = manager->getRunningServers();
    for(const mcsm::ServerConfigLoader* loader : arr){
        if(loader == nullptr){
            if(mcsm::isDebug()) mcsm::warning("Null server config on manager->getGroupLoader()->getServers() detected.");
            continue;
        }
        bool running = false;
        std::string name = loader->getServerName();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
            mcsm::printResultMessage();
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_WARN_NOEXIT) std::exit(1);
        }

        std::string path = loader->getHandle()->getPath();
        for(const mcsm::ServerConfigLoader* runningLoader : arr2){
            if(runningLoader == nullptr){
                if(mcsm::isDebug()) mcsm::warning("Null server config on manager->getRunningServers() detected.");
                continue;
            }
            if(runningLoader->getHandle()->getPath() == path){
                running = true;
                break;
            }
        }

        strmap.emplace(name + " on " + path, running);
    }

    mcsm::info(running ? "List of servers (running): " : "List of servers: ");

    if(running){
        for(auto& [serverstr, isRunning] : strmap){
            if(isRunning){
                std::cout << " * " + serverstr << "\n";
            }
        }
    }else{
        for(auto& [serverstr, isRunning] : strmap){
            if(isRunning){
                mcsm::setcol(mcsm::TextColor::GREEN);
                std::cout << " * " + serverstr + " (running)\n";
                mcsm::setcol(mcsm::TextColor::RESET);
            }else{
                std::cout << " * " + serverstr << "\n";
            }
        }
    }
    std::exit(0);
}