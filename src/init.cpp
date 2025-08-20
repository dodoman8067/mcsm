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
#include <mcsm/data/options/general/sponge_api_search_recommended_versions_property.h>
#include <mcsm/data/options/general/screen_bin_path_property.h>

mcsm::init::init(){
    this->initialized = false;
}

mcsm::init::~init(){
    mcsm::CommandManager::cleanup();
    mcsm::curl_holder::cleanup();
    curl_global_cleanup();
}

mcsm::VoidResult mcsm::init::initMCSM(const std::string& version){
    curl_global_init(CURL_GLOBAL_DEFAULT);
    auto curlInitRes = mcsm::curl_holder::init();
    if(!curlInitRes) return curlInitRes;
    initCommands(version);
    initServers(); // hanles server registry for singleton server instances

    mcsm::VoidResult res = mcsm::GeneralOption::getGeneralOption().initialize(); // initializes global configurations
    if(!res){
        return res;
    }

    this->initialized = true;
    return {};
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

    std::unique_ptr<mcsm::GroupCommand> groupCommand = std::make_unique<mcsm::GroupCommand>("group", "Group command");
    mcsm::CommandManager::addCommand(std::move(groupCommand));
}

void mcsm::init::initServers(){
    auto& sr = mcsm::ServerRegistry::getServerRegistry();

    auto vanilla = std::make_unique<mcsm::VanillaServer>();
    sr.registerServer("vanilla", std::move(vanilla));
    
    auto paper = std::make_unique<mcsm::PaperServer>();
    sr.registerServer("paper", std::move(paper));

    auto purpur = std::make_unique<mcsm::PurpurServer>();
    sr.registerServer("purpur", std::move(purpur));

    auto fabric = std::make_unique<mcsm::FabricServer>();
    sr.registerServer("fabric", std::move(fabric));

    auto velocity = std::make_unique<mcsm::VelocityServer>();
    sr.registerServer("velocity", std::move(velocity));

    auto sponge = std::make_unique<mcsm::SpongeServer>();
    sr.registerServer("sponge", std::move(sponge));

    auto custom = std::make_unique<mcsm::CustomServer>();
    sr.registerServer("custom", std::move(custom));

    auto folia = std::make_unique<mcsm::FoliaServer>();
    sr.registerServer("folia", std::move(folia));

    std::unique_ptr<mcsm::SkipVersionCheckProperty> p1 = std::make_unique<mcsm::SkipVersionCheckProperty>("skip_version_check_while_configuring");
    sr.registerGeneralProperty("skip_version_check_while_configuring", std::move(p1));

    std::unique_ptr<mcsm::AdvancedJsonErrorsProperty> p2 = std::make_unique<mcsm::AdvancedJsonErrorsProperty>("advanced_json_parse_fail_errors");
    sr.registerGeneralProperty("advanced_json_parse_fail_errors", std::move(p2));

    std::unique_ptr<mcsm::SpongeAPISearchRecommendedVersionsProperty> p3 = std::make_unique<mcsm::SpongeAPISearchRecommendedVersionsProperty>("sponge_api_search_recommended_versions");
    sr.registerGeneralProperty("sponge_api_search_recommended_versions", std::move(p3));

    std::unique_ptr<mcsm::ScreenBinPathProperty> p4 = std::make_unique<mcsm::ScreenBinPathProperty>("screen_binary_path");
    sr.registerGeneralProperty("screen_binary_path", std::move(p4));
}

bool mcsm::init::isInitialized() const {
    return this->initialized;
}