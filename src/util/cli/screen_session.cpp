#include <mcsm/util/cli/screen_session.h>
#include <mcsm/data/options/general_option.h>

mcsm::ScreenSession::ScreenSession(const std::string& name, const std::string& command) : name(name), command(command){
    this->screenPath = mcsm::GeneralOption::getGeneralOption().screenBinPathProperty();
}

mcsm::Result mcsm::ScreenSession::start(){
    if(isRunning()){
        return {mcsm::ResultType::MCSM_FAIL, {"A screen session with the same name already exists."}};
    }
    std::string startCommand = this->screenPath + " -dmS " + getFullSessionName() + " sh -c '" + this->command + "'";
    #ifdef MCSM_DEBUG
        std::cout << "DEBUG: " << startCommand << "\n";
    #endif
    if(mcsm::runCommandQuietly(startCommand) != 0){
        return {mcsm::ResultType::MCSM_FAIL, {"Failed to start screen session " + getFullSessionName()}};
    }
    return {mcsm::ResultType::MCSM_SUCCESS, {"Success"}};
}

mcsm::Result mcsm::ScreenSession::stop(){
    if(!isRunning()){
        return {mcsm::ResultType::MCSM_FAIL, {"No running session with name " + this->name + ".mcsm to stop."}};
    }
    std::string stopCommand = this->screenPath + " -S " + getFullSessionName() + " -X quit";
    #ifdef MCSM_DEBUG
        std::cout << "DEBUG: " << stopCommand << "\n";
    #endif
    if(mcsm::runCommandQuietly(stopCommand) != 0){
        return {mcsm::ResultType::MCSM_FAIL, {"Failed to stop screen session " + getFullSessionName()}};
    }
    return {mcsm::ResultType::MCSM_SUCCESS, {"Success"}};
}

mcsm::Result mcsm::ScreenSession::attach(){
    if(!isRunning()){
        return {mcsm::ResultType::MCSM_FAIL, {"Cannot send command to a non-existent session: " + this->name + ".mcsm"}};
    }
    std::string attachCommand = this->screenPath + " -r " + getFullSessionName();
    if(mcsm::runCommandQuietly(attachCommand) != 0){
        return {mcsm::ResultType::MCSM_FAIL, {"Failed to attach to session: " + getFullSessionName()}};
    }
    return {mcsm::ResultType::MCSM_SUCCESS, {"Success"}};
}

mcsm::Result mcsm::ScreenSession::sendCommand(const std::string& str){
    if(!isRunning()){
        return {mcsm::ResultType::MCSM_FAIL, {"Cannot send command to a non-existent session: " + this->name + ".mcsm"}};
    }
    std::string command = this->screenPath + " -S " + getFullSessionName() + " -p 0 -X stuff \"" + str + "\\n\"";
    #ifdef MCSM_DEBUG
        std::cout << "DEBUG: " << command << "\n";
    #endif
    if(mcsm::runCommandQuietly(command) != 0){
        return {mcsm::ResultType::MCSM_FAIL, {"Failed to send command to session: " + getFullSessionName()}};
    }
    return {mcsm::ResultType::MCSM_SUCCESS, {"Success"}};
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
    std::string checkCommand = this->screenPath + " -ls";
    std::FILE* pipe = popen(checkCommand.c_str(), "r");
    if (!pipe) return false;

    char buffer[128];
    while(std::fgets(buffer, sizeof(buffer), pipe)){
        std::string line = trim(buffer);
        if(line.find(getFullSessionName()) != std::string::npos){
            pclose(pipe);
            return true;
        }
    }

    pclose(pipe);
    return false;
}

bool mcsm::ScreenSession::tryReconnect(){
    if(isRunning()){
        return true;
    }
    return false;
}