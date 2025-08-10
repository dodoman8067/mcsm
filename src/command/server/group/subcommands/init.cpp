#include <mcsm/command/server/group/subcommands/init.h>

mcsm::GroupInitSubCommand::GroupInitSubCommand(mcsm::ServerGroupGenerator* generator){
    if(generator){
        this->generator = generator;
    }
}

mcsm::GroupInitSubCommand::~GroupInitSubCommand(){

}

void mcsm::GroupInitSubCommand::execute(const std::vector<std::string>& args){
    if(!mcsm::isSafeString(args[0])){
        mcsm::warning("The following server name contains blacklisted characters: \"" + args[0] + "\"");
        mcsm::warning("Please try again with a valid name.");
        std::exit(1);
    }

    if(args[1] != "screen" && args[1] != "default"){
        mcsm::warning("Invalid group launch mode: \"" + args[1] + "\"");
        mcsm::warning("Valid group launch modes are \"screen\" and \"default\"");
        std::exit(1);
    }

    mcsm::unwrapOrExit(this->generator->generate(args[1]));
    mcsm::success("Group \"" + args[0] + "\" configured with mode \"" + args[1] + "\".");
    std::exit(0);
}
