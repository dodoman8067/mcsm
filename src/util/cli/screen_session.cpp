#include <mcsm/util/cli/screen_session.h>
#include <mcsm/data/options/general_option.h>

mcsm::ScreenSession::ScreenSession(const std::string& name) : mcsm::ScreenSession(name, ""){}

mcsm::ScreenSession::ScreenSession(const std::string& name, const std::string& command) : name(name), command(command){
    this->screenPath = mcsm::GeneralOption::getGeneralOption().screenBinPathProperty();
}

mcsm::VoidResult mcsm::ScreenSession::start(){
    if(isRunning()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::SCREEN_SESSION, {"A screen session with the same name already exists."});
        return tl::unexpected(err);
    }
    std::string startCommand = this->screenPath + " -dmS " + getFullSessionName() + " sh -c '" + this->command + "'";
    if(mcsm::runCommandQuietly(startCommand) != 0){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::SCREEN_SESSION, {"Failed to start screen session " + getFullSessionName()});
        return tl::unexpected(err);
    }
    return {};
}

mcsm::VoidResult mcsm::ScreenSession::stop(){
    if(!isRunning()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::SCREEN_SESSION, {"No running session with name " + this->name + ".mcsm to stop."});
        return tl::unexpected(err);
    }
    std::string stopCommand = this->screenPath + " -S " + getFullSessionName() + " -X quit";
    if(mcsm::runCommandQuietly(stopCommand) != 0){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::SCREEN_SESSION, {"Failed to stop screen session " + getFullSessionName()});
        return tl::unexpected(err);
    }
    return {};
}

mcsm::VoidResult mcsm::ScreenSession::attach(){
    if(!isRunning()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::SCREEN_SESSION, {"Cannot send command to a session that is not running: " + this->name + ".mcsm"});
        return tl::unexpected(err);
    }
    std::string attachCommand = this->screenPath + " -r " + getFullSessionName();
    if(mcsm::runCommandQuietly(attachCommand) != 0){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::SCREEN_SESSION, {"Failed to attach to session: " + getFullSessionName()});
        return tl::unexpected(err);
    }
    return {};
}

mcsm::VoidResult mcsm::ScreenSession::sendCommand(const std::string& str){
    if(!isRunning()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::SCREEN_SESSION, {"Cannot send command to a non-existent session: " + this->name + ".mcsm"});
        return tl::unexpected(err);
    }
    std::string command = this->screenPath + " -S " + getFullSessionName() + " -p 0 -X stuff \"" + str + "\\n\"";
    if(mcsm::runCommandQuietly(command) != 0){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::SCREEN_SESSION, {"Failed to send command to session: " + getFullSessionName()});
        return tl::unexpected(err);
    }
    return {};
}

bool mcsm::ScreenSession::validateScreen() const {
    std::string checkCommand = "test -x " + this->screenPath + " && echo OK";
    std::FILE* pipe = popen(checkCommand.c_str(), "r");
    if (!pipe) return false;
    char buffer[128];
    bool valid = std::fgets(buffer, sizeof(buffer), pipe) != nullptr && std::string(buffer).find("OK") != std::string::npos;
    pclose(pipe);
    return valid;
}

std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    size_t last = str.find_last_not_of(" \t\n\r");
    return (first == std::string::npos) ? "" : str.substr(first, last - first + 1);
}

bool mcsm::ScreenSession::isRunning() const {
    std::string sessionName = getFullSessionName();
    std::string checkCommand = this->screenPath + " -ls | grep -w \"" + sessionName + "\"";

    auto exitCode = mcsm::runCommandQuietly(checkCommand.c_str());
    if(!exitCode) return false;
    return exitCode.value() == 0;
}

bool mcsm::ScreenSession::tryReconnect(){
    if(isRunning()){
        return true;
    }
    return false;
}