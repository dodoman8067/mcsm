/*
MIT License

Copyright (c) 2023 dodoman8067

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <mcsm/init.h>
#include <mcsm/data/options/general/test1.h>
#include <mcsm/data/options/general/test2.h>

mcsm::init::init(){
    this->initialized = new bool(false);
}

mcsm::init::~init(){
    curl_global_cleanup();
    mcsm::curl_holder::cleanup();
    delete this->initialized;
    this->initialized = nullptr;
}

void mcsm::init::initMCSM(const std::string& version){
    curl_global_init(CURL_GLOBAL_DEFAULT);
    if(!mcsm::curl_holder::init().isSuccess()) return;
    initCommands(version);
    initServers();

    mcsm::Result res = mcsm::GeneralOption::getGeneralOption().initialize();
    if(!res.isSuccess()){
        res.printMessage();
        return;
    }

    *this->initialized = true;
}

void mcsm::init::initCommands(const std::string& version){
    // Initializes CommandManager class
    mcsm::CommandManager::init();

    // Register commands to CommandManager
    std::unique_ptr<mcsm::VersionCommand> versionCommand = std::make_unique<mcsm::VersionCommand>("version", "Returns the version information about this program.", version);
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

    std::unique_ptr<mcsm::StartServerCommand> startServerCommand = std::make_unique<mcsm::StartServerCommand>("start", "Starts the configured server.");
    startServerCommand->addAlias("startServer");
    startServerCommand->addAlias("startserver");
    mcsm::CommandManager::addCommand(std::move(startServerCommand));

    std::unique_ptr<mcsm::ViewServerCommand> viewServerCommand = std::make_unique<mcsm::ViewServerCommand>("view", "Provides the configured server's basic information.");
    viewServerCommand->addAlias("viewServer");
    viewServerCommand->addAlias("viewserver");
    mcsm::CommandManager::addCommand(std::move(viewServerCommand));

    std::unique_ptr<mcsm::ViewServerTypeCommand> viewServerTypeCommand = std::make_unique<mcsm::ViewServerTypeCommand>("info", "Provides a basic information about the specified server implementation.");
    viewServerTypeCommand->addAlias("information");
    viewServerTypeCommand->addAlias("informationServer");
    viewServerTypeCommand->addAlias("informationserver");
    viewServerTypeCommand->addAlias("infoServer");
    viewServerTypeCommand->addAlias("infoserver");
    mcsm::CommandManager::addCommand(std::move(viewServerTypeCommand));
}

void mcsm::init::initServers(){
    auto& sr = mcsm::ServerRegistry::getServerRegistry();

    sr.registerServer("vanilla", []() { return std::make_shared<mcsm::VanillaServer>(); }, mcsm::ServerType::VANILLA);
    sr.registerServer("paper", []() { return std::make_shared<mcsm::PaperServer>(); }, mcsm::ServerType::BUKKIT);
    sr.registerServer("purpur", []() { return std::make_shared<mcsm::PurpurServer>(); }, mcsm::ServerType::BUKKIT);
    sr.registerServer("fabric", []() { return std::make_shared<mcsm::FabricServer>(); }, mcsm::ServerType::FABRIC);
    sr.registerServer("velocity", []() { return std::make_shared<mcsm::VelocityServer>(); }, mcsm::ServerType::VELOCITY);
    sr.registerServer("sponge", []() { return std::make_shared<mcsm::SpongeServer>(); }, mcsm::ServerType::SPONGE_VANILLA);
    sr.registerServer("custom", []() { return std::make_shared<mcsm::CustomServer>(); }, mcsm::ServerType::CUSTOM);

    std::unique_ptr<mcsm::Test1> t = std::make_unique<mcsm::Test1>("test1");
    sr.registerGeneralProperty("test1", std::move(t));

    std::unique_ptr<mcsm::Test2> t2 = std::make_unique<mcsm::Test2>("test2");
    sr.registerGeneralProperty("test2", std::move(t2));
}

bool mcsm::init::isInitialized() const {
    return *this->initialized;
}