#include <mcsm/command/server/group/subcommands/start.h>

mcsm::GroupStartSubCommand::GroupStartSubCommand(mcsm::ServerGroupManager* manager): manager(manager){}

void mcsm::GroupStartSubCommand::execute(const std::vector<std::string>& args){
    if(args.empty()){
        mcsm::warning("No servers to start.");
        mcsm::warning("Example usage: mcsm group start /path/to/server1 /path/to/server2");
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
        mcsm::warning("No servers to start.");
        mcsm::warning("Example usage: mcsm group start /path/to/server1 /path/to/server2 your_server_name");
        std::exit(1);
    }

    this->loader = this->manager->getGroupLoader();

    int startedServers = 0;

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
    std::vector<std::string> pathsToStart;
    for(const std::string& arg : serverArgs){
        if(std::filesystem::exists(arg)){
            pathsToStart.push_back(arg);
        }else{
            // Name-based lookup
            auto it = namePathMap.find(arg);
            if(it != namePathMap.end()){
                if(it->second.size() == 1){
                    pathsToStart.push_back(it->second.front());
                }else{
                    // prompt user
                    mcsm::warning("Multiple servers found with the name \"" + arg + "\":");
                    for(size_t i = 0; i<it->second.size(); i++){
                        mcsm::info(std::to_string(i + 1) + ": " + it->second[i]);
                    }
                    mcsm::info("Enter the number of the server to start, or enter 'all' to start all instances:");
    
                    std::string choice;
                    std::cin >> choice;
    
                    if(choice == "all"){
                        pathsToStart.insert(pathsToStart.end(), it->second.begin(), it->second.end());
                    }else{
                        if(!mcsm::is_number(choice)){ mcsm::warning("Invalid selection. Skipping..."); continue; }
                        int index = std::stoi(choice) - 1;
                        if(index >= 0 && index < static_cast<int>(it->second.size())){
                            pathsToStart.push_back(it->second[index]);
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

    size_t requestedServers = pathsToStart.size();
    
    for(const std::string& path : pathsToStart){
        mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}}); // clear result
        mcsm::Result stRes = this->manager->start(path);
        if(!stRes.isSuccess()){
            if(strict){
                mcsm::error("Failed to start server \"" + path + "\".");
                mcsm::error("Below lines are the output from the internal system.");
                mcsm::error("");
                stRes.printMessage();
                mcsm::warning("NOTE: Strict mode is currently enabled.");
            }
            mcsm::warning("Starting server \"" + path + "\" failed. Run with --strict option for more info.");
            continue;
        }
        startedServers++;
    }

    mcsm::info(startedServers == 0 ? "No servers started." : std::to_string(startedServers) + "/" + std::to_string(requestedServers) + " server(s) started.");
    std::exit(startedServers > 0 ? 0 : 1);
}