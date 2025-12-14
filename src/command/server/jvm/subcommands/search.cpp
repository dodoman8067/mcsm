#include <mcsm/command/server/jvm/subcommands/search.h>


mcsm::SearchTarget mcsm::JvmSearchSubCommand::getSearchTarget(const mcsm::ArgumentParser& args) const {
    if(args.flagExists("global")) return mcsm::SearchTarget::GLOBAL;
    if(args.flagExists("current")) return mcsm::SearchTarget::CURRENT;
    return mcsm::SearchTarget::ALL;
}


std::string mcsm::JvmSearchSubCommand::getOptionName(const mcsm::ArgumentParser& args) const {
    if(!args.flagExists("name")){
        mcsm::warning("Name not provided; specify a name with --name option to continue.");
        std::exit(1);
    }
    std::string name = args.getValue("name");
    if (!mcsm::isSafeString(name)){
        mcsm::warning("The given runtime profile name \"" + name + "\" contains blacklisted characters.");
        std::exit(1);
    }
    return name;
}

std::vector<std::unique_ptr<mcsm::JvmOption>> mcsm::JvmSearchSubCommand::search(const mcsm::SearchTarget& target, const std::string& name){
    std::vector<std::unique_ptr<mcsm::JvmOption>> result;
    if(target == mcsm::SearchTarget::GLOBAL || target == mcsm::SearchTarget::ALL){
        std::unique_ptr<mcsm::JvmOption> opt = std::make_unique<mcsm::JvmOption>(mcsm::unwrapOrExit(mcsm::jvmProfileFromSearchTarget(name, mcsm::SearchTarget::GLOBAL, mcsm::unwrapOrExit(mcsm::getCurrentPath()))));
        mcsm::unwrapOrExit(opt->init());
        bool profileExists = mcsm::unwrapOrExit(opt->exists());
        if(profileExists) result.push_back(std::move(opt));
    }
    if(target == mcsm::SearchTarget::CURRENT || target == mcsm::SearchTarget::ALL){
        std::unique_ptr<mcsm::JvmOption> opt = std::make_unique<mcsm::JvmOption>(mcsm::unwrapOrExit(mcsm::jvmProfileFromSearchTarget(name, mcsm::SearchTarget::CURRENT, mcsm::unwrapOrExit(mcsm::getCurrentPath()))));
        mcsm::unwrapOrExit(opt->init());
        bool profileExists = mcsm::unwrapOrExit(opt->exists());
        if(profileExists) result.push_back(std::move(opt));
    }
    return result;
}


void mcsm::JvmSearchSubCommand::execute(const std::vector<std::string>& args){
    const std::vector<std::pair<std::string, std::vector<std::string>>> availableOptions = {
        {"global", {"g"}},
        {"current", {"c"}},
        {"name", {"n"}}
    };
    mcsm::ArgumentParser parser(availableOptions, mcsm::vecToString(args));
    mcsm::SearchTarget target = getSearchTarget(parser);

    std::string name = getOptionName(parser);
    std::vector<std::unique_ptr<mcsm::JvmOption>> options = search(target, name); // no need to call init; already called by search()
    if(options.empty()){
        mcsm::info("No JVM launch profile found in the given search target.");
        std::exit(1);
    }
    mcsm::info("Search result : ");
    std::cout << "\n";
    int i = 1;
    for(std::unique_ptr<mcsm::JvmOption>& option : options){
        auto name = option->getProfileName();
        auto path = mcsm::unwrapOrExit(option->getProfilePath());
        auto jvmPath = mcsm::unwrapOrExit(option->getJvmPath());
        auto jvmArgs = mcsm::unwrapOrExit(option->getJvmArguments());
        auto serverArgs = mcsm::unwrapOrExit(option->getServerArguments());

        std::cout << "Result " << i << " : "<< "\n";
        std::cout << "Name : " << name << "\n";
        std::cout << "Path : " << path << "\n";
        std::cout << "JVM : " << jvmPath << "\n";
        if(!jvmArgs.empty()){
            std::cout << "JVM arguments : ";
            for(std::string_view args : jvmArgs){
                std::cout << args << " ";
            }
            std::cout << "\n";
        }
        if(!serverArgs.empty()){
            std::cout << "Server arguments : ";
            for(const std::string& sArgs : serverArgs){
                std::cout << sArgs << " ";
            }
            std::cout << "\n";
        }
        std::cout << "\n";
        i++;
    }
    std::exit(0);
}