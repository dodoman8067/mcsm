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
#include <mcsm/data/options/general/skip_version_check_property.h>
#include <mcsm/data/options/general/advanced_json_errors_property.h>

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
    generateServerCommand->addAlias("initserver");
    mcsm::CommandManager::addCommand(std::move(generateServerCommand));

    std::unique_ptr<mcsm::JvmOptionGeneratorCommand> jvmOptionGeneratorCommand = std::make_unique<mcsm::JvmOptionGeneratorCommand>("genJvmProfile", "Generates a Java Virtual Machine launch profile.");
    jvmOptionGeneratorCommand->addAlias("generatejvmprofile");
    jvmOptionGeneratorCommand->addAlias("genjvmprofile");
    mcsm::CommandManager::addCommand(std::move(jvmOptionGeneratorCommand));

    std::unique_ptr<mcsm::JvmOptionSearchCommand> jvmProfileSearchCommand = std::make_unique<mcsm::JvmOptionSearchCommand>("searchJvmProfile", "Looks up Java Virtual Machine profiles.");
    jvmProfileSearchCommand->addAlias("searchprofile");
    jvmProfileSearchCommand->addAlias("searchjvmprofile");
    mcsm::CommandManager::addCommand(std::move(jvmProfileSearchCommand));

    std::unique_ptr<mcsm::JvmOptionEditCommand> jvmOptionEditCommand = std::make_unique<mcsm::JvmOptionEditCommand>("editJvmProfile", "Edits the specified Java Virtual Machine launch profile.");
    jvmOptionEditCommand->addAlias("editjvmprofile");
    jvmOptionEditCommand->addAlias("editprofile");
    mcsm::CommandManager::addCommand(std::move(jvmOptionEditCommand));

    std::unique_ptr<mcsm::StartServerCommand> startServerCommand = std::make_unique<mcsm::StartServerCommand>("start", "Starts the configured server.");
    startServerCommand->addAlias("startserver");
    mcsm::CommandManager::addCommand(std::move(startServerCommand));

    std::unique_ptr<mcsm::ViewServerCommand> viewServerCommand = std::make_unique<mcsm::ViewServerCommand>("view", "Provides the configured server's basic information.");
    viewServerCommand->addAlias("status");
    mcsm::CommandManager::addCommand(std::move(viewServerCommand));

    std::unique_ptr<mcsm::ViewServerTypeCommand> viewServerTypeCommand = std::make_unique<mcsm::ViewServerTypeCommand>("info", "Provides a basic information about the specified server implementation.");
    viewServerTypeCommand->addAlias("information");
    mcsm::CommandManager::addCommand(std::move(viewServerTypeCommand));

    std::unique_ptr<mcsm::ClearServerCommand> clearServerCommand = std::make_unique<mcsm::ClearServerCommand>("clear", "Clears the configured server's jarfile and its update history.");
    clearServerCommand->addAlias("clean");
    mcsm::CommandManager::addCommand(std::move(clearServerCommand));
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

    std::unique_ptr<mcsm::SkipVersionCheckProperty> p1 = std::make_unique<mcsm::SkipVersionCheckProperty>("skip_version_check_while_configuring");
    sr.registerGeneralProperty("skip_version_check_while_configuring", std::move(p1));

    std::unique_ptr<mcsm::AdvancedJsonErrorsProperty> p2 = std::make_unique<mcsm::AdvancedJsonErrorsProperty>("advanced_json_parse_fail_errors");
    sr.registerGeneralProperty("advanced_json_parse_fail_errors", std::move(p2));
}

bool mcsm::init::isInitialized() const {
    return *this->initialized;
}