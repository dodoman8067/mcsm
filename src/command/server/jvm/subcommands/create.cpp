#include <mcsm/command/server/jvm/subcommands/create.h>
#include <mcsm/jvm/java_detector.h>

void mcsm::JvmCreateSubCommand::execute(const std::vector<std::string>& args){
    const std::vector<std::pair<std::string, std::vector<std::string>>> availableOptions = {
        {"jvmpath", {"jp", "jpath"}},
        {"global", {"g"}},
        {"serverarguments", {"serverargs", "sargs", "sa"}},
        {"name", {"n"}},
        {"jvmarguments", {"jvmargs", "jargs", "ja"}},
        {"defaults", {"d", "use-defaults"}}
    };
    mcsm::ArgumentParser parser(availableOptions, mcsm::vecToString(args));
    mcsm::SearchTarget target = getSaveTarget(parser);
    createProfile(parser, target);
}

mcsm::SearchTarget mcsm::JvmCreateSubCommand::getSaveTarget(const mcsm::ArgumentParser& args) {
    return args.flagExists("global") ? mcsm::SearchTarget::GLOBAL : mcsm::SearchTarget::CURRENT;
}

std::string mcsm::JvmCreateSubCommand::getJvmPath(const mcsm::ArgumentParser& args) const {
    if(!args.flagExists("jvmpath")) return "";
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

std::vector<std::string> mcsm::JvmCreateSubCommand::getJvmArguments(const mcsm::ArgumentParser& args) const {
    std::vector<std::string> jvmArgs;
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

std::vector<std::string> mcsm::JvmCreateSubCommand::getServerArguments(const mcsm::ArgumentParser& args) const {
    std::vector<std::string> serverArgs;
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

std::string mcsm::JvmCreateSubCommand::getOptionName(const mcsm::ArgumentParser& args) const {
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

std::string mcsm::JvmCreateSubCommand::getProfileName(const mcsm::ArgumentParser& args, const mcsm::SearchTarget& target) const {
    std::string name = getOptionName(args);
    mcsm::JvmOption jvm(mcsm::unwrapOrExit(mcsm::jvmProfileFromSearchTarget(name, target, mcsm::unwrapOrExit(mcsm::getCurrentPath()))));
    mcsm::unwrapOrExit(jvm.init());

    if(mcsm::unwrapOrExit(jvm.exists())){
        mcsm::warning("JVM launch profile " + name + " already exists. Command failed.");
        std::exit(1);
    }
    return name;
}

std::string mcsm::JvmCreateSubCommand::prompt(const std::string& key) const {
    std::cout << "Enter "
              << key
              << " : ";

    std::string input;
    std::getline(std::cin, input);
    return input;
}

void mcsm::JvmCreateSubCommand::createProfile(const mcsm::ArgumentParser& args, const mcsm::SearchTarget& target) {
    mcsm::JvmOption option(mcsm::unwrapOrExit(mcsm::jvmProfileFromSearchTarget(getProfileName(args, target), target, mcsm::unwrapOrExit(mcsm::getCurrentPath()))));
    mcsm::unwrapOrExit(option.init());

    bool useDefaults = args.flagExists("defaults");

    std::vector<std::string> jvmArgs = getJvmArguments(args);
    if(!args.flagExists("jvmarguments") && useDefaults){
        jvmArgs.push_back("-Xms2G");
        jvmArgs.push_back("-Xmx2G");
        jvmArgs.push_back("-jar");
        mcsm::info("No JVM arguments specified; Defaulting to -Xms2G, -Xmx2G and -jar.");
    }else if(!args.flagExists("jvmarguments") && !useDefaults){
        jvmArgs = mcsm::stringToVec(prompt("Java virtual machine flags to use"));
    }
    
    std::vector<std::string> sargs = getServerArguments(args);
    if(!args.flagExists("serverarguments") && useDefaults) {
        sargs.push_back("nogui");
        mcsm::info("No server arguments specified; Defaulting to nogui.");
    }else if(!args.flagExists("serverarguments") && !useDefaults){
        sargs = mcsm::stringToVec(prompt("server flags to use"));
    }

    std::string jvm = getJvmPath(args);
    if(!args.flagExists("jvmpath")){
        if(useDefaults){
            //TODO: add "default_jvm_path" on general option
            mcsm::warning("defaults on jvm path not implemented");
        }
        std::map<int, std::string> jvmMap;
        int i = 0;
        for(const std::string& java : mcsm::findJavaPaths()){
            auto isValid = mcsm::unwrapOrExit(mcsm::isValidJava(java));
            if(isValid){
                i++;
                jvmMap.insert({i, java});
                std::cout << " * " << i << ". " << java << "\n";
            }
        }

        std::cout << "\nEnter one of the given numbers, or a custom path for Java virtual machine binary that this profile will use.\n";
        bool valid = false;
        while (!valid){
            std::string res = prompt("JVM path");
            if(mcsm::is_number(res)){
                int index = std::stoi(res);
                if(index < 1 || index > i){
                    std::cout << "Invalid input; please try again.\n";
                    continue;
                }else{
                    jvm = jvmMap[index];
                    std::cout << "Choosing " << jvm << "..\n";
                    valid = true;
                    break;
                }
            }else{
                if(mcsm::isWhitespaceOrEmpty(jvm)){
                    std::cout << "Invalid input; please try again.\n";
                    continue;
                }
                jvm = res;
                std::cout << "Choosing " << jvm << "..\n";
                valid = true;
                break;
            }
        }
    }

    mcsm::unwrapOrExit(option.create(jvm, jvmArgs, sargs));

    auto name = option.getProfileName();

    auto path = mcsm::unwrapOrExit(option.getProfilePath());

    auto jArgs = mcsm::unwrapOrExit(option.getJvmArguments());

    auto serverArgs = mcsm::unwrapOrExit(option.getServerArguments());

    mcsm::info("Java Virtual Machine launch profile generated : ");
    mcsm::info("Profile name : " + name);
    mcsm::info("Profile location : " + path);
    mcsm::info("JVM path : " + jvm);

    if(!jArgs.empty()) {
        std::cout << "[mcsm/INFO] JVM arguments : ";
        for(std::string_view args : jArgs) {
            std::cout << args << " ";
        }
        std::cout << "\n";
    }
    if(!serverArgs.empty()) {
        std::cout << "[mcsm/INFO] Server arguments : ";
        for(const std::string &sArgs : serverArgs) {
            std::cout << sArgs << " ";
        }
        std::cout << "\n";
    }
    std::exit(0);
}