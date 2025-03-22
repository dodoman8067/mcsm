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
        if(subArgs.empty()) mcsm::info("a");
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
}
