#include <mcsm/command/server/jvm/jvm_command.h>
#include <mcsm/command/server/jvm/subcommands/create.h>
#include <mcsm/command/server/jvm/subcommands/edit.h>

mcsm::JvmCommand::JvmCommand(const std::string& name, const std::string& description) : mcsm::Command(name, description){}

mcsm::JvmCommand::~JvmCommand(){

}

void mcsm::JvmCommand::execute(const std::vector<std::string>& args){
    if(args.empty()){
        mcsm::warning("Invalid command.");
        mcsm::warning("Type mcsm jvm help for more information.");
        std::exit(1);
    }
    std::string subc = args[0];
    std::vector<std::string> subArgs;
    for(size_t i = 1; i < args.size(); i++){
        subArgs.push_back(args[i]);
    }

    // will handle string valid checks on command classes.
    // only handle them here if you must
    if(subc == "create"){
        mcsm::JvmCreateSubCommand cmd;
        cmd.execute(subArgs);
    }
    if(subc == "edit"){
        mcsm::JvmEditSubCommand cmd;
        cmd.execute(subArgs);
    }
    if(subc == "detect-install"){
    }
    if(subc == "search"){
    }
    if(subc == "import"){
    }
    if(subc == "test"){
    }
    if(subc == "help"){

    }
    mcsm::warning("Invalid command.");
    mcsm::warning("Type mcsm jvm help for more information.");
    std::exit(1);
}
