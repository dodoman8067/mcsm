#include <mcsm/command/server/group/subcommands/add.h>

mcsm::GroupAddSubCommand::GroupAddSubCommand(mcsm::ServerGroupLoader* loader): loader(loader){}

void mcsm::GroupAddSubCommand::execute(const std::vector<std::string>& args){
    if(args.empty()){
        mcsm::warning("No servers to add.");
        mcsm::warning("Example usage: mcsm group add /path/to/server1 /path/to/server2");
        std::exit(1);
    }

    bool strict = false;
    std::vector<std::string> serverArgs;

    for(const std::string& arg : args){
        if(arg == "--strict" || arg == "-strict" || arg == "--s" || arg == "-s"){
            strict = true;
        }else{
            serverArgs.push_back(arg);
        }
    }

    if(serverArgs.empty()){
        mcsm::warning("No servers to add.");
        mcsm::warning("Example usage: mcsm group add /path/to/server1 /path/to/server2");
        std::exit(1);
    }

    int addedServers = 0;
    size_t requestedServers = serverArgs.size();

    for(std::string arg : serverArgs){
        arg = mcsm::normalizePath(arg);

        if(!std::filesystem::exists(arg)){
            if(strict){
                mcsm::warning("Invalid path \"" + arg + "\" that cannot be added as a server detected.");
                mcsm::warning("NOTE: Strict mode is currently enabled.");
                std::exit(1);
            }
            mcsm::warning("Invalid path \"" + arg + "\". Skipping...");
            continue;
        }
        std::unique_ptr<mcsm::ServerConfigLoader> sl = std::make_unique<mcsm::ServerConfigLoader>(arg);
        mcsm::VoidResult loadRes = sl->loadConfig();

        if(!loadRes){
            if(strict){
                mcsm::error("Failed to load server on \"" + arg + "\".");
                mcsm::error("Below lines are the output from the interal system.");
                mcsm::error("");
                mcsm::printError(loadRes);
                mcsm::warning("NOTE: Strict mode is currently enabled.");
                std::exit(1);
            }
            mcsm::warning("Failed to load server on \"" + arg + "\". Run with --strict flag for more information.");
            continue;
        }

        auto serverName = sl->getServerName();
        if(!serverName){
            if(strict){
                mcsm::error("Failed to load server's name on \"" + arg + "\".");
                mcsm::error("Below lines are the output from the interal system.");
                mcsm::error("");
                mcsm::printError(serverName);
                mcsm::warning("NOTE: Strict mode is currently enabled.");
                std::exit(1);
            }
            mcsm::warning("Failed to load server's name on \"" + arg + "\". Run with --strict flag for more information.");
            continue;
        }

        mcsm::VoidResult addRes = this->loader->addServer(std::move(sl));
        if(!addRes){
            if(strict){
                mcsm::error("Failed to add server \"" + arg + "\".");
                mcsm::error("Below lines are the output from the interal system.");
                mcsm::error("");
                mcsm::printError(addRes);
                mcsm::warning("NOTE: Strict mode is currently enabled.");
                std::exit(1);
            }
            mcsm::warning("Failed to add server on \"" + arg + "\". Run with --strict flag for more information.");
            continue;
        }
        if(requestedServers < 15){
            mcsm::info("Server " + serverName.value() + " on \"" + arg + "\" added.");
        }
        addedServers++;
    }

    mcsm::info(addedServers == 0 ? "No servers added." : std::to_string(addedServers) + "/" + std::to_string(requestedServers) + " server(s) added.");
    std::exit(addedServers > 0 ? 0 : 1);
}