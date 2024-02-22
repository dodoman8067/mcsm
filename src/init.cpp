#include <mcsm/init.h>

mcsm::init::init(){
    this->initialized = new bool(false);
}

mcsm::init::~init(){
    delete this->initialized;
    this->initialized = nullptr;
}

void mcsm::init::initMCSM(const std::string& version){
    // Some other init tasks will be added
    initCommands(version);

    *this->initialized = true;
}

void mcsm::init::initCommands(const std::string& version){
    // Initializes CommandManager class
    mcsm::CommandManager::init();

    // Register commands to CommandManager
    std::unique_ptr<mcsm::VersionCommand> versionCommand = std::make_unique<mcsm::VersionCommand>("version", "Returns version information about this program.", version);
    versionCommand->addAlias("ver");
    mcsm::CommandManager::addCommand(std::move(versionCommand));

    std::unique_ptr<mcsm::HelpCommand> helpCommand = std::make_unique<mcsm::HelpCommand>("help", "Shows the full list of commands.");
    helpCommand->addAlias("?");
    mcsm::CommandManager::addCommand(std::move(helpCommand));

    std::unique_ptr<mcsm::GenerateServerCommand> generateServerCommand = std::make_unique<mcsm::GenerateServerCommand>("init", "Configures a server.");
    generateServerCommand->addAlias("initServer");
    generateServerCommand->addAlias("initserver");
    generateServerCommand->addAlias("initialize");
    generateServerCommand->addAlias("initializeServer");
    generateServerCommand->addAlias("initializeserver");
    mcsm::CommandManager::addCommand(std::move(generateServerCommand));

    std::unique_ptr<mcsm::JvmOptionGeneratorCommand> jvmOptionGeneratorCommand = std::make_unique<mcsm::JvmOptionGeneratorCommand>("genJvmProfile", "Generates a Java Virtual Machine launch profile.");
    jvmOptionGeneratorCommand->addAlias("generateJvmProfile");
    jvmOptionGeneratorCommand->addAlias("generatejvmprofile");
    jvmOptionGeneratorCommand->addAlias("generateJavaVirtualMachineProfile");
    jvmOptionGeneratorCommand->addAlias("genjvmprofile");
    mcsm::CommandManager::addCommand(std::move(jvmOptionGeneratorCommand));

    std::unique_ptr<mcsm::JvmOptionSearchCommand> jvmProfileSearchCommand = std::make_unique<mcsm::JvmOptionSearchCommand>("searchJvmProfile", "Looks up Java Virtual Machine profiles.");
    jvmProfileSearchCommand->addAlias("searchProfile");
    jvmProfileSearchCommand->addAlias("searchJavaVirtualMachineProfile");
    jvmProfileSearchCommand->addAlias("searchprofile");
    mcsm::CommandManager::addCommand(std::move(jvmProfileSearchCommand));

    std::unique_ptr<mcsm::JvmOptionEditCommand> jvmOptionEditCommand = std::make_unique<mcsm::JvmOptionEditCommand>("editJvmProfile", "Edits the specified Java Virtual Machine launch profile.");
    jvmOptionEditCommand->addAlias("editJavaVirtualMachineProfile");
    jvmOptionEditCommand->addAlias("editprofile");
    jvmOptionEditCommand->addAlias("editProfile");
    jvmOptionEditCommand->addAlias("editjvmprofile");
    mcsm::CommandManager::addCommand(std::move(jvmOptionEditCommand));

    std::unique_ptr<mcsm::StartServerCommand> startServerCommand = std::make_unique<mcsm::StartServerCommand>("start", "Starts a configured server.");
    startServerCommand->addAlias("startServer");
    startServerCommand->addAlias("startserver");
    mcsm::CommandManager::addCommand(std::move(startServerCommand));

    std::unique_ptr<mcsm::ViewServerCommand> viewServerCommand = std::make_unique<mcsm::ViewServerCommand>("view", "Provides the configured server's basic information.");
    viewServerCommand->addAlias("viewServer");
    viewServerCommand->addAlias("viewserver");
    mcsm::CommandManager::addCommand(std::move(viewServerCommand));

    std::unique_ptr<mcsm::ViewServerTypeCommand> viewServerTypeCommand = std::make_unique<mcsm::ViewServerTypeCommand>("info", "Provides basic information about the specified server implementation.");
    viewServerTypeCommand->addAlias("information");
    viewServerTypeCommand->addAlias("informationServer");
    viewServerTypeCommand->addAlias("informationserver");
    viewServerTypeCommand->addAlias("infoServer");
    viewServerTypeCommand->addAlias("infoserver");
    mcsm::CommandManager::addCommand(std::move(viewServerTypeCommand));
}

bool mcsm::init::isInitialized() const {
    return *this->initialized;
}