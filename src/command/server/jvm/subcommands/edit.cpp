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
        mcsm::info("Java detection from -jp command arguments failed; Automatically detecting java..");
        std::string java = mcsm::unwrapOrExit(mcsm::detectJava());
        return java;
    }
    std::string jvmPath = args.getValue("jvmpath");
    if(!mcsm::startsWith(jvmPath, "\"") && !mcsm::startsWith(jvmPath, "\'")){
        jvmPath = "\"" + jvmPath;
    }
    if(!mcsm::endsWith(jvmPath, "\"") && !mcsm::endsWith(jvmPath, "\'")){
        jvmPath += "\"";
    }
    if (!mcsm::unwrapOrExit(mcsm::isValidJava(jvmPath))){
        mcsm::warning("Detected java in " + jvmPath + " was not a valid java.");
        std::exit(1);
    }
    mcsm::success("Detected java from specified path : " + jvmPath);
    return jvmPath;
}

std::vector<std::string> mcsm::JvmEditSubCommand::getJvmArguments(const mcsm::ArgumentParser& args) const {
    std::vector<std::string> jvmArgs;
    if(!args.flagExists("jvmarguments")){
        jvmArgs.push_back("-Xms2G");
        jvmArgs.push_back("-Xmx2G");
        jvmArgs.push_back("-jar");
        mcsm::info("No JVM arguments specified; Defaulting to -Xms2G, -Xmx2G and -jar.");
        return jvmArgs;
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
    if(!args.flagExists("jvmarguments")) {
        serverArgs.push_back("nogui");
        mcsm::info("No server arguments specified; Defaulting to nogui.");
        return serverArgs;
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
    mcsm::JvmOption jvm(name, target);
    mcsm::unwrapOrExit(jvm.init());

    if(!mcsm::unwrapOrExit(jvm.exists())){
        mcsm::warning("JVM launch profile " + name + " doesn't exist. Command failed.");
        std::exit(1);
    }
    return name;
}

void mcsm::JvmEditSubCommand::editProfile(const mcsm::ArgumentParser& args, const mcsm::SearchTarget& target) {
    mcsm::JvmOption option(getProfileName(args, target), target);
    mcsm::unwrapOrExit(option.init());

    std::vector<std::string> jvmArgs = getJvmArguments(args);
    std::vector<std::string> sArgs = getServerArguments(args);
    std::string jvm = getJvmPath(args);

    std::vector<std::string> optJvmArgs = mcsm::unwrapOrExit(option.getJvmArguments());
    std::vector<std::string> optServerArgs = mcsm::unwrapOrExit(option.getServerArguments());
    std::string oldJvm = mcsm::unwrapOrExit(option.getJvmPath());

    mcsm::info("Java Virtual Machine launch profile edited : ");
    mcsm::info("Profile name : " + option.getProfileName());

    if(oldJvm != jvm && !mcsm::isWhitespaceOrEmpty(jvm)){
        mcsm::info("JVM path : " + oldJvm + " -> " + jvm);
        mcsm::unwrapOrExit(option.setJvmPath(jvm));
    }

    if(!jvmArgs.empty() && optJvmArgs != jvmArgs){
        mcsm::unwrapOrExit(option.setJvmArguments(jvmArgs));
        printDifference("JVM arguments", optJvmArgs, jvmArgs);
    }
    if(!sArgs.empty()){
        mcsm::unwrapOrExit(option.setServerArguments(sArgs));
        printDifference("Server arguments", optServerArgs, sArgs);
    }
    std::exit(0);
}

inline void mcsm::JvmEditSubCommand::printDifference(const std::string& category, const std::vector<std::string>& from, const std::vector<std::string>& to) const {
    if(from != to){
        std::cout << "[mcsm/INFO] " << category << " : \n";
        if(!from.empty()){
            std::cout << "[mcsm/INFO] From : ";
            for (std::string_view arg : from) {
                std::cout << arg << " ";
            }
        }

        if(!to.empty()){
            std::cout << "\n[mcsm/INFO] To : ";
            for(std::string_view arg : to){
                std::cout << arg << " ";
            }
        }
        std::cout << "\n";
    }
}