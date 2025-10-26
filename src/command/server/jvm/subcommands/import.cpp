#include <mcsm/command/server/jvm/subcommands/import.h>
#include <mcsm/data/options/general_option.h>

void mcsm::JvmImportSubCommand::copyToRepo(const mcsm::JvmOption& opt, const mcsm::SearchTarget& target){
    std::string path = mcsm::unwrapOrExit(opt.getProfilePath());
    mcsm::Option a(path, opt.getProfileName());

    if(!mcsm::unwrapOrExit(a.exists())){
        mcsm::warning("Cannot import JVM profile " + path + "/" + opt.getProfileName() + " that does not exist.");
        std::exit(1);
    }
    mcsm::unwrapOrExit(a.load(mcsm::GeneralOption::getGeneralOption().advancedParseEnabled()));

    std::string bPath;
    if(target == mcsm::SearchTarget::CURRENT){
        bPath = mcsm::unwrapOrExit(mcsm::getCurrentPath()) + "/.mcsm/jvm/profiles";
    }
    if(target == mcsm::SearchTarget::GLOBAL){
        bPath = mcsm::asGlobalDataPath("/jvm/profiles");
    }

    mcsm::Option b(bPath, opt.getProfileName());
    if(mcsm::unwrapOrExit(b.exists())){
        mcsm::warning("Cannot copy JVM profile to the following location \"" + bPath + "\" that already contains the file.");
        std::exit(1);
    }

    mcsm::unwrapOrExit(b.load());

    mcsm::info("Copying the following datas in " + mcsm::joinPath(path, opt.getProfileName()) + " to " + mcsm::joinPath(bPath, opt.getProfileName()));

    auto jArgs = mcsm::unwrapOrExit(opt.getJvmArguments());

    auto serverArgs = mcsm::unwrapOrExit(opt.getServerArguments());

    std::string jvm = mcsm::unwrapOrExit(opt.getJvmPath());

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
    std::cout << a.getData() << "\n";
    mcsm::unwrapOrExit(b.save(a.getData()));
    mcsm::success("Copy successful.");
}

std::unique_ptr<mcsm::JvmOption> mcsm::JvmImportSubCommand::getJvmOption(const mcsm::ArgumentParser& parser){
    if(!parser.flagExists("path")){
        mcsm::warning("JVM Profile to import must be given in --path option.");
        std::exit(1);
    }
    std::string path1 = parser.getValue("path");
    std::string normalized = mcsm::normalizePath(path1);
    std::string parentPath;

    std::filesystem::path path(normalized);
    if(!path.has_filename()){
        mcsm::warning("JVM Profile to import must have a filename.json given in --path option.");
        std::exit(1);
    }
    if(!path.has_parent_path()){
        mcsm::warning("No parent path detected in the given --path value; defualting to current directory.");
        parentPath = mcsm::unwrapOrExit(mcsm::getCurrentPath());
    }else{
        parentPath = path.parent_path();
    }

    auto optP = std::make_unique<mcsm::Option>(parentPath, path.filename());
    std::unique_ptr<mcsm::JvmOption> opt = std::make_unique<mcsm::JvmOption>(std::move(optP));
    
    mcsm::VoidResult res = opt->init();
    if(!res){
        mcsm::warning("Parsing JVM profile " + normalized + " failed with the following internal logs:");
        mcsm::warning("");
        mcsm::printError(res.error());
        std::exit(1);
    }
    return opt;
}

mcsm::SearchTarget mcsm::JvmImportSubCommand::getSaveTarget(const mcsm::ArgumentParser& args) const {
    return args.flagExists("global") ? mcsm::SearchTarget::GLOBAL : mcsm::SearchTarget::CURRENT;
}

void mcsm::JvmImportSubCommand::execute(const std::vector<std::string>& args){
    const std::vector<std::pair<std::string, std::vector<std::string>>> availableOptions = {
        {"path", {"p"}},
        {"global", {"g"}}
    };
    mcsm::ArgumentParser parser(availableOptions, mcsm::vecToString(args));
    mcsm::SearchTarget target = getSaveTarget(parser);
    
    auto jvmOption = getJvmOption(parser);
    copyToRepo(*jvmOption.get(), target);

    std::exit(0);
}