#include <mcsm/command/server/group/group_command.h>
#include <mcsm/command/server/group/subcommands/add.h>
#include <mcsm/command/server/group/subcommands/attach.h>
#include <mcsm/command/server/group/subcommands/init.h>
#include <mcsm/command/server/group/subcommands/list.h>
#include <mcsm/command/server/group/subcommands/list_running.h>
#include <mcsm/command/server/group/subcommands/remove.h>

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
    for(int i = 2; i < subArgs.size(); i++){
        subArgs.push_back(subArgs[i]);
    }

    // will handle string valid checks on command classes.
    // only handle it here if not possible
    
    if(subc == "add"){

    }
    if(subc == "attach"){

    }
    if(subc == "init"){
        if(subArgs.empty() || subArgs.size() > 1){
            mcsm::warning("Invalid command.");
            mcsm::warning("Type mcsm group help for more information.");
            std::exit(1);            
        }
        std::string name = subArgs[0];

        if(!mcsm::isSafeString(name)){
            mcsm::warning("The following server name contains blacklisted characters: \"" + name + "\"");
            mcsm::warning("Please try again with a valid name.");
            std::exit(1);
        }
        std::string path = mcsm::getCurrentPath(); // going to make my program able to specify where to configure the file. use subArgs[1] if implemented

        mcsm::ServerGroupGenerator gen(name, path);
        mcsm::GroupInitSubCommand cmd(&gen);
        cmd.execute(subArgs);
    }
}
