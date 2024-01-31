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

}

void mcsm::CustomServer::setFileLocation(const std::string& location) {

}

void mcsm::CustomServer::getFileFromLocation() {

}

bool mcsm::CustomServer::isFile(const std::string& location) const {
    return std::filesystem::exists(location);
}

bool mcsm::CustomServer::isURL(const std::string& location) const {

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