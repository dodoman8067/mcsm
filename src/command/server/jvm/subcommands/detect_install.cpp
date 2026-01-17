#include <mcsm/command/server/jvm/subcommands/detect_install.h>
#include <mcsm/jvm/java_detection.h>

void mcsm::JvmDetectInstallSubCommand::execute(const std::vector<std::string>& /* args */){
    mcsm::info("This is a simple utility that tries to detect installed Java virtual machine binaries.");
    mcsm::info("Note that it may not catch manual portable installs and such. You may define MCSM_JAVA_PATHS environment variable with semicolon(;) separators to let MCSM detect custom locations.");

    auto javas = mcsm::findJavaPaths();
    std::set<std::string> validJavas = {};
    for(const std::string& java : javas){
        auto isValid = mcsm::unwrapOrExit(mcsm::isValidJava(java));
        if(isValid){
            validJavas.insert({java});
        }
    }

    if(!validJavas.empty()){ mcsm::success("Detected Java virtual machine binaries: "); }
    else{ mcsm::warning("No java seems to be detected for now!"); }

    for(const std::string& java2 : validJavas){
        std::cout << " * " << java2 << "\n"; 
    }

    std::exit(0);
}