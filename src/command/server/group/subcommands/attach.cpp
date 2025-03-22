#include <mcsm/command/server/group/subcommands/attach.h>

mcsm::GroupAttachSubCommand::GroupAttachSubCommand(mcsm::ServerGroupManager* manager): manager(manager){}

void mcsm::GroupAttachSubCommand::execute(const std::vector<std::string>& args){
    if(args.empty()){
        mcsm::warning("No server to attatch.");
        mcsm::warning("Example usage: mcsm group attatch /path/to/server");
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
        mcsm::warning("No server to attatch.");
        mcsm::warning("Example usage: mcsm group attatch /path/to/server1");
        std::exit(1);
    }

    this->loader = this->manager->getGroupLoader();

    bool attatched = false;

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
    std::vector<std::string> pathsToAttach;
    for(const std::string& arg : serverArgs){
        if(std::filesystem::exists(arg)){
            pathsToAttach.push_back(arg);
        }else{
            // Name-based lookup
            auto it = namePathMap.find(arg);
            if(it != namePathMap.end()){
                if(it->second.size() == 1){
                    pathsToAttach.push_back(it->second.front());
                }else{
                    // prompt user
                    mcsm::warning("Multiple servers found with the name \"" + arg + "\":");
                    for(size_t i = 0; i<it->second.size(); i++){
                        mcsm::info(std::to_string(i + 1) + ": " + it->second[i]);
                    }
                    mcsm::info("Enter the number of the server to start:");
    
                    std::string choice;
                    std::cin >> choice;

                    if(!mcsm::is_number(choice)){ mcsm::warning("Invalid selection. Skipping..."); continue; }
                    int index = std::stoi(choice) - 1;
                    if(index >= 0 && index < static_cast<int>(it->second.size())){
                        pathsToAttach.push_back(it->second[index]);
                    }else{
                        mcsm::warning("Invalid selection. Skipping...");
                    }
                }
            }else{
                mcsm::warning("No server found with name \"" + arg + "\". Skipping...");
            }
        }
    }

    if(pathsToAttach.empty()){
        std::exit(1);
    }

    if(pathsToAttach.size() > 1){
        mcsm::warning("You may only attach to one running server.");
        std::exit(1);
    }

    std::string gName = this->manager->getGroupLoader()->getName();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        mcsm::printResultMessage();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_WARN_NOEXIT) std::exit(1);
    }

    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}}); // clear result

    std::string modifiedName = mcsm::safeString(pathsToAttach[0]);

    mcsm::ScreenSession session(gName + "." + modifiedName);
    mcsm::Result stRes = session.attach();
    if(!stRes.isSuccess()){
        if(strict){
            mcsm::error("Failed to attatch to server \"" + pathsToAttach[0] + "\".");
            mcsm::error("Below lines are the output from the internal system.");
            mcsm::error("");
            stRes.printMessage();
            mcsm::warning("NOTE: Strict mode is currently enabled.");
        }
        mcsm::warning("Starting server \"" + pathsToAttach[0] + "\" failed. Run with --strict option for more info.");
    }else{
        attatched = true;
    }
    
    std::exit(attatched ? 0 : 1); 
} 