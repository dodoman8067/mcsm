#include <mcsm/server/type/custom_server.h>

mcsm::CustomServer::CustomServer(){

}

mcsm::CustomServer::~CustomServer(){

}

mcsm::ServerType mcsm::CustomServer::getType() const {
    return mcsm::ServerType::CUSTOM;
}

std::string mcsm::CustomServer::getTypeAsString() const {
    return "custom";
}

std::string mcsm::CustomServer::getSupportedVersions() const {
    return "unknown";
}

std::string mcsm::CustomServer::getFileLocation() const {
    mcsm::Option option(".", "server");
    if(!option.exists()){
        mcsm::error("Option does not exist; Task aborted.");
        std::exit(1);
    }
    if(option.getValue("jar_location") == nullptr){
        mcsm::error("No \"jar_location\" value specified in file " + option.getName());
        mcsm::error("Manually editing the launch profile might have caused this issue.");
        mcsm::error("If you know what you're doing, I believe you that you know how to handle this issue.");
        mcsm::error("If you believe that this is a software issue, please report it to GitHub (https://github.com/dodoman8067/mcsm).");
        std::exit(1);
    }
    if(!option.getValue("jar_location").is_string()){
        mcsm::error("Value \"jar_location\" has to be a string, but it's not.");
        mcsm::error("Manually editing the launch profile might have caused this issue.");
        mcsm::error("If you know what you're doing, I believe you that you know how to handle this issue.");
        mcsm::error("If you believe that this is a software issue, please report it to GitHub (https://github.com/dodoman8067/mcsm).");
        std::exit(1);
    }
    std::string value = option.getValue("jar_location");
    if(mcsm::startsWith(value, "current") && mcsm::endsWith(value, "current")){
        return mcsm::getCurrentPath();
    }
    return option.getValue("jar_location");
}

void mcsm::CustomServer::setFileLocation(const std::string& location) {
    mcsm::Option option(".", "server");
    option.setValue("jar_location", location);
}

void mcsm::CustomServer::getFileFromLocation() {
    std::string location = getFileLocation();
    if(isFile(location) || isURL(location)){
        if(isURL(location)){
            mcsm::download(getJarFile(), location, getFileLocation(), true);
        }
    }else{
        mcsm::error("The following server jarfile location wasn't a vaild location : " + location);
        mcsm::error("If you believe that this is a software issue, please report it to GitHub (https://github.com/dodoman8067/mcsm).");
    }
}

bool mcsm::CustomServer::isFile(const std::string& location) const {
    return std::filesystem::exists(location);
}

bool mcsm::CustomServer::isURL(const std::string& location) const {
    std::regex urlPattern(
        R"(https?:\/\/(www\.)?[-a-zA-Z0-9@:%._\+~#=]{1,256}\.[a-zA-Z0-9()]{1,6}\b"
        R"([-a-zA-Z0-9()@:%_\+.~#?&//=]*))"
    );
    return std::regex_match(location, urlPattern);
}

std::string mcsm::CustomServer::getBasedServer() const {
    return "unknown";
}

bool mcsm::CustomServer::isBasedAs(const std::string& input) const {
    return false;
}

std::string mcsm::CustomServer::getWebSite() const {
    return "unknown";
}

std::string mcsm::CustomServer::getGitHub() const {
    return "unknown";
}