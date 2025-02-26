#include <mcsm/command/server/group/subcommands/remove.h>

mcsm::GroupRemoveSubCommand::GroupRemoveSubCommand(mcsm::ServerGroupLoader* loader): loader(loader){}

void mcsm::GroupRemoveSubCommand::execute(const std::vector<std::string>& args){
    if(args.empty()){
        mcsm::warning("No servers to remove.");
        mcsm::warning("Example usage: mcsm group add /path/to/server1 /path/to/server2");
        std::exit(1);
    }

    bool strict = false;
    std::vector<std::string> serverArgs; // list of server paths

    for(const std::string& arg : args){
        if(arg == "--strict" || arg == "-strict" || arg == "--s" || arg == "-s"){
            strict = true;
        }else{
            serverArgs.push_back(mcsm::normalizePath(arg));
        }
    }

    if(serverArgs.empty()){
        mcsm::warning("No servers to remove.");
        mcsm::warning("Example usage: mcsm group add /path/to/server1 /path/to/server2");
        std::exit(1);
    }

    int removedServers = 0;
    size_t requestedServers = serverArgs.size();

    std::unordered_map<std::string, std::vector<std::string>> namePathMap; // pair of name and multiple path

    // populate namePathMap with all servers
    for(auto* config : this->loader->getServers()){
        std::string cPath = config->getHandle()->getPath();
        std::string cName = config->getServerName();
    
        mcsm::Result sNGRes({mcsm::getLastResult().first, mcsm::getLastResult().second});
        if(!sNGRes.isSuccess()){
            if(strict){
                mcsm::error("Failed to load server's name on \"" + cPath + "\".");
                mcsm::error("Below lines are the output from the internal system.");
                mcsm::error("");
                sNGRes.printMessage();
                mcsm::warning("NOTE: Strict mode is currently enabled.");
                std::exit(1);
            }
            mcsm::warning("Failed to load server's name on \"" + cPath + "\". Run with --strict flag for more information.");
            continue;
        }
    
        namePathMap[cName].push_back(cPath);
    }
    
    // check user inputs (serverArgs) against namePathMap
    std::vector<std::string> pathsToRemove;
    for(const std::string& arg : serverArgs){
        if(std::filesystem::exists(arg)){
            pathsToRemove.push_back(arg);
        }else{
            // Name-based lookup
            auto it = namePathMap.find(arg);
            if(it != namePathMap.end()){
                if(it->second.size() == 1){
                    // remove directly
                    pathsToRemove.push_back(it->second.front());
                }else{
                    // prompt user
                    mcsm::warning("Multiple servers found with the name \"" + arg + "\":");
                    for(size_t i = 0; i<it->second.size(); i++){
                        mcsm::info(std::to_string(i + 1) + ": " + it->second[i]);
                    }
                    mcsm::info("Enter the number of the server to remove, or enter 'all' to remove all instances:");
    
                    std::string choice;
                    std::cin >> choice;
    
                    if(choice == "all"){
                        pathsToRemove.insert(pathsToRemove.end(), it->second.begin(), it->second.end());
                    }else{
                        int index = std::stoi(choice) - 1;
                        if(index >= 0 && index < static_cast<int>(it->second.size())){
                            pathsToRemove.push_back(it->second[index]);
                        }else{
                            mcsm::warning("Invalid selection. Skipping...");
                        }
                    }
                }
            }else{
                mcsm::warning("No server found with name \"" + arg + "\". Skipping...");
            }
        }
    }
    
    for(const std::string& path : pathsToRemove){
        mcsm::Result rmRes = this->loader->removeServer(path);
        if(!rmRes.isSuccess()){
            if(strict){
                mcsm::error("Failed to remove server \"" + path + "\".");
                mcsm::error("Below lines are the output from the internal system.");
                mcsm::error("");
                rmRes.printMessage();
                mcsm::warning("NOTE: Strict mode is currently enabled.");
                std::exit(1);
            }
            mcsm::warning("Removing server \"" + path + "\" failed. Run with --strict option for more info.");
            continue;
        }
        if(requestedServers < 15){
            mcsm::info("Server on \"" + path + "\" removed.");
        }
        removedServers++;
    }

    mcsm::info(removedServers == 0 ? "No servers removed." : std::to_string(removedServers) + "/" + std::to_string(requestedServers) + " server(s) removed.");
    std::exit(removedServers > 0 ? 0 : 1);
}