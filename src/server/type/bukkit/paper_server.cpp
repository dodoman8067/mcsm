/*
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

#include <mcsm/server/type/bukkit/paper_server.h>

mcsm::PaperServer::PaperServer() {}

mcsm::PaperServer::~PaperServer() {}

int mcsm::PaperServer::getVersion(const std::string& ver) const {
    std::string res = mcsm::get("https://api.papermc.io/v2/projects/paper/versions/" + ver);
    nlohmann::json json = nlohmann::json::parse(res, nullptr, false);
    if(json.is_discarded()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonParseFailedCannotBeModified()});
        return -1;
    }
    if(json["builds"].is_array()){
        nlohmann::json builds = json["builds"];
        if(builds[json["builds"].size() - 1] == nullptr || !builds[json["builds"].size() - 1].is_number_integer()) return -1;
        return builds[json["builds"].size() - 1];
    }else{
        return -1;
    }
}

int mcsm::PaperServer::getVersion(const std::string& ver, const std::string& build) const {
    std::string res = mcsm::get("https://api.papermc.io/v2/projects/paper/versions/" + ver + "/builds/" + build);
    nlohmann::json json = nlohmann::json::parse(res, nullptr, false);
    if(json.is_discarded()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonParseFailedCannotBeModified()});
        return -1;
    }

    if(json["build"] == nullptr || !json["build"].is_number_integer()){
        return -1;
    }else{
        return json["build"];
    }
}

std::vector<std::string> mcsm::PaperServer::getAvailableVersions(){
    std::vector<std::string> versions;
    for(const std::string& s : mcsm::getMinecraftVersions()){
        versions.push_back(s);
    }
    return versions;
}

std::string mcsm::PaperServer::getSupportedVersions() const {
    return "1.8~";
}

std::string mcsm::PaperServer::getBasedServer() const {
    return "spigot";
}

std::string mcsm::PaperServer::getWebSite() const {
    return "https://papermc.io";
}

std::string mcsm::PaperServer::getGitHub() const {
    return "https://github.com/PaperMC/Paper";
}

mcsm::Result mcsm::PaperServer::download(const std::string& version){
    std::string path = mcsm::getCurrentPath();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }
    
    std::string jar = getJarFile();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    return download(version, path, jar, path);
}

mcsm::Result mcsm::PaperServer::download(const std::string& version, const std::string& path){
    std::string path1 = mcsm::getCurrentPath();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    std::string jar = getJarFile();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    return download(version, path, jar, path1);
}

mcsm::Result mcsm::PaperServer::download(const std::string& version, const std::string& path, const std::string& name){
    std::string path1 = mcsm::getCurrentPath();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    return download(version, path, name, path1);
}

mcsm::Result mcsm::PaperServer::update(const std::string& optionPath){
    // If you change the default build to specific build from latest build, it won't downgrade automatically. (You'll have to manually delete the server jarfile) This is an intented feature.
    mcsm::info("Checking updates...");
    mcsm::ServerDataOption sDataOpt(optionPath);
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    mcsm::Option opt(optionPath, "server");
    nlohmann::json sBuild = opt.getValue("server_build");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    if(sBuild == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFound("\"server_build\"", opt.getName())});
        return res;
    }
    if(!sBuild.is_string()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongTypePlusFix("\"server_build\"", opt.getName(), "string", "change it into \"server_build\": \"latest\"")});
        return res;
    }
    std::string build = sBuild.get<std::string>();
    if(build != "latest"){
        mcsm::Result res({mcsm::ResultType::MCSM_WARN_NOEXIT, {
            "This server won't update to the latest build.",
            "Change server.json into \"server_build\": \"latest\" for automatic download."
        }});
        return res;
    }

    nlohmann::json sVer = opt.getValue("version");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    if(sVer == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFound("\"version\"", opt.getName())});
        return res;
    }
    if(!sVer.is_string()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"version\"", "string")});
        return res;
    }
    
    std::string version = sVer.get<std::string>();
    int ver = getVersion(version);
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    if(ver == -1){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverUnsupportedVersion()});
        return res;
    }
    std::string lastBuild = sDataOpt.getLastDownloadedBuild();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    if(lastBuild == version){
        mcsm::success("Server is up to date.");
        mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
        return res;
    }
    mcsm::success("Update found : "  + version + ". Current build : " + lastBuild);

    std::string jar = getJarFile(optionPath);
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    bool fileExists = mcsm::fileExists(jar);
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    if(fileExists){
        mcsm::removeFile(jar);
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
            std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
            mcsm::Result res(resp.first, resp.second);
            return res;
        }
    }
    return download(version, optionPath, jar, optionPath);
}

mcsm::Result mcsm::PaperServer::start(mcsm::JvmOption& option){
    mcsm::ServerOption sOpt;
    if(!std::filesystem::exists(getJarFile())){
        mcsm::info("Downloading " + getJarFile() + "...");
        download(sOpt.getServerVersion());
    }else{
        update();
    }
    Server::start(option);
}

mcsm::Result mcsm::PaperServer::update(){
    std::string path = mcsm::getCurrentPath();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    update(path);
}

mcsm::Result mcsm::PaperServer::update(const std::string& optionPath){
    // If you change the default build to specific build from latest build, it won't downgrade automatically. (You'll have to manually delete the server jarfile) This is an intented feature.
    mcsm::info("Checking updates...");
    mcsm::ServerDataOption sDataOpt(optionPath);
    mcsm::Option opt(optionPath, "server");
    if(!opt.getValue("server_build").is_string()){
        mcsm::error("Value \"server_build\" option in server.json must be a string type.");
        mcsm::error("To fix, change it into \"server_build\": \"latest\" .");
        std::exit(1);
    }
    std::string build = opt.getValue("server_build").get<std::string>();
    if(build != "latest"){
        mcsm::warning("This server won't update to the latest build.");
        mcsm::warning("Change server.json into \"server_build\": \"latest\" for automatic download.");
        return;
    }
    if(opt.getValue("version") == nullptr){
        mcsm::error("No \"version\" value specified in file " + opt.getName());
        mcsm::error("Manually editing the launch profile might have caused this issue.");
        mcsm::error("If you know what you're doing, I believe you that you know how to handle this issue.");
        mcsm::error("If you believe that this is a software issue, please report this to GitHub (https://github.com/dodoman8067/mcsm).");
        std::exit(1);
    }
    if(!opt.getValue("version").is_string()){
        mcsm::error("Value \"version\" has to be a string, but it's not.");
        mcsm::error("Manually editing the launch profile might have caused this issue.");
        mcsm::error("If you know what you're doing, I believe you that you know how to handle this issue.");
        mcsm::error("If you believe that this is a software issue, please report this to GitHub (https://github.com/dodoman8067/mcsm).");
        std::exit(1);            
    }
    std::string version = opt.getValue("version").get<std::string>();
    int ver = getVersion(version);
    if(ver == -1){
        mcsm::error("Unsupported version.");
        mcsm::error("Please try again with a different version.");
        std::exit(1);
    }
    std::string strVer = std::to_string(ver);
    if(sDataOpt.getLastDownloadedBuild() == strVer){
        mcsm::success("Server is up to date.");
        return;
    }
    mcsm::success("Update found : "  + strVer + ". Current build : " + sDataOpt.getLastDownloadedBuild());
    if(std::filesystem::exists(getJarFile(optionPath))){
        std::filesystem::remove(getJarFile(optionPath));
    }
    download(version, optionPath, getJarFile(), optionPath);
}

bool mcsm::PaperServer::hasVersion(const std::string& version){
    return getVersion(version) != -1;
}

std::string mcsm::PaperServer::getTypeAsString() const {
    return "paper";
}