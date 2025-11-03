#include <mcsm/command/server/jvm/subcommands/edit.h>

void mcsm::JvmEditSubCommand::execute(const std::vector<std::string>& args){
    const std::vector<std::pair<std::string, std::vector<std::string>>> availableOptions = {
        {"jvmpath", {"jp", "jpath"}},
        {"global", {"g"}},
        {"serverarguments", {"serverargs", "sargs", "sa"}},
        {"name", {"n"}},
        {"jvmarguments", {"jvmargs", "jargs", "ja"}}
    };
    mcsm::ArgumentParser parser(availableOptions, mcsm::vecToString(args));
    mcsm::SearchTarget target = getSaveTarget(parser);
    editProfile(parser, target);
}

mcsm::SearchTarget mcsm::JvmEditSubCommand::getSaveTarget(const mcsm::ArgumentParser& args) {
    return args.flagExists("global") ? mcsm::SearchTarget::GLOBAL : mcsm::SearchTarget::CURRENT;
}

std::string mcsm::JvmEditSubCommand::getJvmPath(const mcsm::ArgumentParser& args) const {
    if(!args.flagExists("jvmpath")){
        return "";
    }
    std::string jvmPath = args.getValue("jvmpath");
    if(!mcsm::startsWith(jvmPath, "\"") && !mcsm::startsWith(jvmPath, "\'")){
        jvmPath = "\"" + jvmPath;
    }
    if(!mcsm::endsWith(jvmPath, "\"") && !mcsm::endsWith(jvmPath, "\'")){
        jvmPath += "\"";
    }
    return jvmPath;
}

std::vector<std::string> mcsm::JvmEditSubCommand::getJvmArguments(const mcsm::ArgumentParser& args) const {
    std::vector<std::string> jvmArgs;
    if(!args.flagExists("jvmarguments")){
        return {};
    }
    std::string jvmArgsRaw = args.getValue("jvmarguments");
    std::istringstream iss(jvmArgsRaw);
    std::string part;
    while(std::getline(iss, part, ' ')) {
        if(!part.empty()) {
            jvmArgs.push_back(part);
        }
    }
    return jvmArgs;
}

std::vector<std::string> mcsm::JvmEditSubCommand::getServerArguments(const mcsm::ArgumentParser& args) const {
    std::vector<std::string> serverArgs;
    if(!args.flagExists("serverarguments")) {
        return {};
    }
    std::string serverArgsRaw = args.getValue("serverarguments");
    std::istringstream iss(serverArgsRaw);
    std::string part;
    while(std::getline(iss, part, ' ')) {
        if(!part.empty()) {
            serverArgs.push_back(part);
        }
    }
    return serverArgs;
}

std::string mcsm::JvmEditSubCommand::getOptionName(const mcsm::ArgumentParser& args) const {
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

std::string mcsm::JvmEditSubCommand::getProfileName(const mcsm::ArgumentParser& args, const mcsm::SearchTarget& target) const {
    std::string name = getOptionName(args);
    mcsm::JvmOption jvm(mcsm::unwrapOrExit(mcsm::jvmProfileFromSearchTarget(name, target, mcsm::unwrapOrExit(mcsm::getCurrentPath()))));
    mcsm::unwrapOrExit(jvm.init());

    if(!mcsm::unwrapOrExit(jvm.exists())){
        mcsm::warning("JVM launch profile " + name + " doesn't exist. Command failed.");
        std::exit(1);
    }
    return name;
}

void mcsm::JvmEditSubCommand::editProfile(const mcsm::ArgumentParser& args, const mcsm::SearchTarget& target) {
    mcsm::JvmOption option(mcsm::unwrapOrExit(mcsm::jvmProfileFromSearchTarget(getProfileName(args, target), target, mcsm::unwrapOrExit(mcsm::getCurrentPath()))));
    mcsm::unwrapOrExit(option.init());

    std::vector<std::string> jvmArgs = getJvmArguments(args);
    std::vector<std::string> sArgs = getServerArguments(args);
    std::string jvm = getJvmPath(args);

    std::vector<std::string> optJvmArgs = mcsm::unwrapOrExit(option.getJvmArguments());
    std::vector<std::string> optServerArgs = mcsm::unwrapOrExit(option.getServerArguments());
    std::string oldJvm = mcsm::unwrapOrExit(option.getJvmPath());

    mcsm::info("Java Virtual Machine launch profile edited : ");
    mcsm::info("Profile name : " + option.getProfileName());

    if(args.flagExists("jvmpath") && oldJvm != jvm){
        mcsm::info("JVM path : " + oldJvm + " -> " + jvm);
        mcsm::unwrapOrExit(option.setJvmPath(jvm));
    }
    if(args.flagExists("jvmarguments")){
        mcsm::unwrapOrExit(option.setJvmArguments(jvmArgs));
        printDifference("JVM arguments", optJvmArgs, jvmArgs);
    }
    if(args.flagExists("serverarguments")){
        mcsm::unwrapOrExit(option.setServerArguments(sArgs));
        printDifference("Server arguments", optServerArgs, sArgs);
    }
    std::exit(0);
}

inline void mcsm::JvmEditSubCommand::printDifference(const std::string& category, const std::vector<std::string>& from, const std::vector<std::string>& to) const {
    if(from != to){
        std::cout << "[mcsm/INFO] " << category << " : \n";
        std::cout << "[mcsm/INFO] From : ";
        for (std::string_view arg : from) {
            std::cout << arg << " ";
        }
    
        std::cout << "\n[mcsm/INFO] To : ";
        for(std::string_view arg : to){
            std::cout << arg << " ";
        }
        std::cout << "\n";
    }
}