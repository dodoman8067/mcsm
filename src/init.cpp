#include <mcsm/init.h>

mcsm::init::init(){}

mcsm::init::~init(){}

void mcsm::init::initMCSM(const std::string& version){
    // Some other init tasks will be added
    initCommands(version);
}

void mcsm::init::initCommands(const std::string& version){
    // Initializes CommandManager class
    mcsm::CommandManager::init();

    // Adds command to CommandManager
    std::unique_ptr<mcsm::VersionCommand> versionCommand = std::make_unique<mcsm::VersionCommand>("version", "Returns version information about this program.", version);
    versionCommand->addAliases("ver");
    mcsm::CommandManager::addCommand(std::move(versionCommand));

    std::unique_ptr<mcsm::HelpCommand> helpCommand = std::make_unique<mcsm::HelpCommand>("help", "Shows full list of commands.");
    mcsm::CommandManager::addCommand(std::move(helpCommand));

    std::unique_ptr<mcsm::GenerateServerCommand> generateServerCommand = std::make_unique<mcsm::GenerateServerCommand>("init", "Configures a server.");
    mcsm::CommandManager::addCommand(std::move(generateServerCommand));

    std::unique_ptr<mcsm::JvmOptionGeneratorCommand> jvmOptionGeneratorCommand = std::make_unique<mcsm::JvmOptionGeneratorCommand>("genJvmProfile", "Generates Java Virtual Machine launch profile.");
    jvmOptionGeneratorCommand->addAliases("generateJvmProfile");
    jvmOptionGeneratorCommand->addAliases("generateJavaVirtualMachineProfile");
    jvmOptionGeneratorCommand->addAliases("genjvmprofile");
    mcsm::CommandManager::addCommand(std::move(jvmOptionGeneratorCommand));

    std::unique_ptr<mcsm::JvmOptionSearchCommand> jvmProfileSearchCommand = std::make_unique<mcsm::JvmOptionSearchCommand>("searchJvmProfile", "Looks up for Java Virtual Machine profiles.");
    jvmProfileSearchCommand->addAliases("searchProfile");
    jvmProfileSearchCommand->addAliases("searchJavaVirtualMachineProfile");
    jvmProfileSearchCommand->addAliases("searchprofile");
    mcsm::CommandManager::addCommand(std::move(jvmProfileSearchCommand));

    std::unique_ptr<mcsm::JvmOptionEditCommand> jvmOptionEditCommand = std::make_unique<mcsm::JvmOptionEditCommand>("editJvmProfile", "Edits the specified Java Virtual Machine launch profile.");
    jvmOptionEditCommand->addAliases("editJavaVirtualMachineProfile");
    jvmOptionEditCommand->addAliases("editprofile");
    jvmOptionEditCommand->addAliases("editProfile");
    jvmOptionEditCommand->addAliases("editjvmprofile");
    mcsm::CommandManager::addCommand(std::move(jvmOptionEditCommand));

    std::unique_ptr<mcsm::StartServerCommand> startServerCommand = std::make_unique<mcsm::StartServerCommand>("start", "Starts a server.");
    startServerCommand->addAliases("startServer");
    startServerCommand->addAliases("startserver");
    mcsm::CommandManager::addCommand(std::move(startServerCommand));

    // TODO: Consider a better name for these two commands.
    std::unique_ptr<mcsm::ViewServerCommand> viewServerCommand = std::make_unique<mcsm::ViewServerCommand>("view", "Prints configured server's basic information.");
    viewServerCommand->addAliases("viewServer");
    viewServerCommand->addAliases("viewserver");
    mcsm::CommandManager::addCommand(std::move(viewServerCommand));

    std::unique_ptr<mcsm::ViewServerTypeCommand> viewServerTypeCommmand = std::make_unique<mcsm::ViewServerTypeCommand>("info", "Prints specified server implementation's basic information.");
    viewServerTypeCommmand->addAliases("infoserver");
    viewServerTypeCommmand->addAliases("infoServer");
    mcsm::CommandManager::addCommand(std::move(viewServerTypeCommmand));
}