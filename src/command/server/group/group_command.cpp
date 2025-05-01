#include <mcsm/command/server/group/group_command.h>
#include <mcsm/command/server/group/subcommands/add.h>
#include <mcsm/command/server/group/subcommands/attach.h>
#include <mcsm/command/server/group/subcommands/init.h>
#include <mcsm/command/server/group/subcommands/list.h>
#include <mcsm/command/server/group/subcommands/remove.h>
#include <mcsm/command/server/group/subcommands/start.h>
#include <mcsm/command/server/group/subcommands/stop.h>

mcsm::GroupCommand::GroupCommand(const std::string& name, const std::string& description) : mcsm::Command(name, description){}

mcsm::GroupCommand::~GroupCommand(){

}

void mcsm::GroupCommand::execute(const std::vector<std::string>& args){
    if(args.empty()){
        mcsm::warning("Invalid command.");
        mcsm::warning("Type mcsm group help for more information.");
        std::exit(1);
    }
    std::string subc = args[0];
    std::vector<std::string> subArgs;
    for(size_t i = 1; i < args.size(); i++){
        subArgs.push_back(args[i]);
    }

    // will handle string valid checks on command classes.
    // only handle them here if you must
    if(subc == "add"){
        mcsm::ServerGroupLoader gl(mcsm::getCurrentPath());
        mcsm::Result loadRes = gl.load();
        if(!loadRes.isSuccess()){
            loadRes.printMessage();
            if(loadRes.getResult() != mcsm::ResultType::MCSM_WARN_NOEXIT) std::exit(1);
        }
        
        mcsm::GroupAddSubCommand cmd(&gl);
        cmd.execute(subArgs);
    }
    if(subc == "remove"){
        mcsm::ServerGroupLoader gl(mcsm::getCurrentPath());
        mcsm::Result loadRes = gl.load();
        if(!loadRes.isSuccess()){
            loadRes.printMessage();
            if(loadRes.getResult() != mcsm::ResultType::MCSM_WARN_NOEXIT) std::exit(1);
        }
        
        mcsm::GroupRemoveSubCommand cmd(&gl);
        cmd.execute(subArgs); 
    }
    if(subc == "attach"){
        std::unique_ptr<mcsm::ServerGroupLoader> gl = std::make_unique<mcsm::ServerGroupLoader>(mcsm::getCurrentPath());
        mcsm::Result loadRes = gl->load();
        if(!loadRes.isSuccess()){
            loadRes.printMessage();
            if(loadRes.getResult() != mcsm::ResultType::MCSM_WARN_NOEXIT) std::exit(1);
        }

        mcsm::ServerGroupManager sm(std::move(gl));
        
        mcsm::GroupAttachSubCommand cmd(&sm);
        cmd.execute(subArgs);
    }
    if(subc == "init"){
        if(subArgs.empty() || subArgs.size() > 2 || subArgs.size() < 2){
            mcsm::warning("Invalid command.");
            mcsm::warning("Type mcsm group help for more information.");
            std::exit(1);            
        }
        std::string name = subArgs[0];
        std::string mode = subArgs[1];
        std::string path = mcsm::getCurrentPath(); // going to make my program able to specify where to configure the file. use subArgs[2] if implemented

        mcsm::ServerGroupGenerator gen(name, path);
        mcsm::GroupInitSubCommand cmd(&gen);
        cmd.execute(subArgs);
    }
    if(subc == "list"){
        std::unique_ptr<mcsm::ServerGroupLoader> gl = std::make_unique<mcsm::ServerGroupLoader>(mcsm::getCurrentPath());
        mcsm::Result loadRes = gl->load();
        if(!loadRes.isSuccess()){
            loadRes.printMessage();
            if(loadRes.getResult() != mcsm::ResultType::MCSM_WARN_NOEXIT) std::exit(1);
        }

        mcsm::ServerGroupManager sm(std::move(gl));
        
        mcsm::GroupListSubCommand cmd(&sm);
        cmd.execute(subArgs);
    }
    if(subc == "start"){
        std::unique_ptr<mcsm::ServerGroupLoader> gl = std::make_unique<mcsm::ServerGroupLoader>(mcsm::getCurrentPath());
        mcsm::Result loadRes = gl->load();
        if(!loadRes.isSuccess()){
            loadRes.printMessage();
            if(loadRes.getResult() != mcsm::ResultType::MCSM_WARN_NOEXIT) std::exit(1);
        }

        mcsm::ServerGroupManager sm(std::move(gl));
        
        mcsm::GroupStartSubCommand cmd(&sm);
        cmd.execute(subArgs);
    }
    if(subc == "stop"){
        std::unique_ptr<mcsm::ServerGroupLoader> gl = std::make_unique<mcsm::ServerGroupLoader>(mcsm::getCurrentPath());
        mcsm::Result loadRes = gl->load();
        if(!loadRes.isSuccess()){
            loadRes.printMessage();
            if(loadRes.getResult() != mcsm::ResultType::MCSM_WARN_NOEXIT) std::exit(1);
        }

        mcsm::ServerGroupManager sm(std::move(gl));
        
        mcsm::GroupStopSubCommand cmd(&sm);
        cmd.execute(subArgs);
    }
    if(subc == "help"){
        if(subArgs.empty()){
            std::cout << "Available Commands:\n\n";
    
            std::cout << "mcsm group add [server1] [server2] ...\n";
            std::cout << "  - Adds servers to a group.\n";
            std::cout << "  Example: mcsm group add /path/to/server1 /path/to/server2\n\n";
    
            std::cout << "mcsm group remove [server1] [server2] ... [server_name]\n";
            std::cout << "  - Removes servers from a group.\n";
            std::cout << "  Example: mcsm group remove /path/to/server1 /path/to/server2 random_server_name\n\n";
    
            std::cout << "mcsm group list (--running)\n";
            std::cout << "  - Lists all groups, optionally filtered by '--running'.\n";
            std::cout << "  Example: mcsm group list --running\n\n";
    
            std::cout << "mcsm group start [server1] [server2] ... [server_name]\n";
            std::cout << "  - Starts specified servers in a group.\n";
            std::cout << "  Example: mcsm group start /path/to/server1 /path/to/server2 random_server_name\n\n";
    
            std::cout << "mcsm group stop [server1] [server2] ... [server_name]\n";
            std::cout << "  - Stops specified servers in a group.\n";
            std::cout << "  Example: mcsm group stop /path/to/server1 /path/to/server2 random_server_name\n\n";
    
            std::cout << "mcsm group init [screen/default] [server_name]\n";
            std::cout << "  - Initializes a new group. You must specify the mode (screen/default) and a name.\n";
            std::cout << "  Example: mcsm group init screen my_server_group\n";
        }
        else{
            std::string command = subArgs[0];
    
            if(command == "add"){
                std::cout << "Usage: mcsm group add [server1] [server2] ...\n";
                std::cout << "Adds servers to a group.\n";
                std::cout << "Example: mcsm group add /path/to/server1 /path/to/server2\n";
            }
            else if(command == "remove"){
                std::cout << "Usage: mcsm group remove [server1] [server2] ... [server_name]\n";
                std::cout << "Removes servers from a group.\n";
                std::cout << "Example: mcsm group remove /path/to/server1 /path/to/server2 random_server_name\n";
            }
            else if(command == "list"){
                std::cout << "Usage: mcsm group list (--running)\n";
                std::cout << "Lists all groups, optionally filtered by '--running'.\n";
                std::cout << "Example: mcsm group list --running\n";
            }
            else if(command == "start"){
                std::cout << "Usage: mcsm group start [server1] [server2] ... [server_name]\n";
                std::cout << "Starts specified servers in a group.\n";
                std::cout << "Example: mcsm group start /path/to/server1 /path/to/server2 random_server_name\n";
            }
            else if(command == "stop"){
                std::cout << "Usage: mcsm group stop [server1] [server2] ... [server_name]\n";
                std::cout << "Stops specified servers in a group.\n";
                std::cout << "Example: mcsm group stop /path/to/server1 /path/to/server2 random_server_name\n";
            }
            else if(command == "init"){
                std::cout << "Usage: mcsm group init [server_name] [screen/default]\n";
                std::cout << "Initializes a new group. You must specify the mode (screen/default) and a name.\n";
                std::cout << "Example: mcsm group init screen my_server_group\n";
            }
            else{
                std::cout << "Unknown command: " << command << "\n";
                std::exit(1);
            }
        }
        std::exit(0);
    }
    mcsm::warning("Invalid command.");
    mcsm::warning("Type mcsm group help for more information.");
    std::exit(1);
}
