#include <mcsm/command/server/jvm/subcommands/detect_install.h>
#include <mcsm/jvm/java_detection.h>

void mcsm::JvmDetectInstallSubCommand::execute(const std::vector<std::string>& /* args */){
    mcsm::info("This is a simple utility that tries to detect installed Java virtual machine binaries.");
    mcsm::info("Note that it may not catch manual portable installs and such. You may define MCSM_JAVA_PATHS environment variable with semicolon(;) separators to let MCSM detect custom locations.");
    mcsm::success("Detected Java virtual machine binaries: ");
    for(const std::string& java : mcsm::findJavaPaths()){
        auto isValid = mcsm::unwrapOrExit(mcsm::isValidJava(java));
        if(isValid){
            std::cout << " * " << java << "\n";
        }
    }
    std::exit(0);
}