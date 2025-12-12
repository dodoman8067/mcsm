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

mcsm::Result<mcsm::PaperMetadata> mcsm::PaperServer::getVersionData(const std::string& ver){
    if(!mcsm::isSafeString(ver)){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::UNSAFE_STRING, {ver});
        return tl::unexpected(err);
    }
    auto res1 = mcsm::get("https://fill.papermc.io/v3/projects/paper/versions/" + ver, "mcsm/0.6 (https://github.com/dodoman8067/mcsm)");
    if(!res1) return tl::unexpected(res1.error());
    nlohmann::json json1 = nlohmann::json::parse(res1.value(), nullptr, false);
    if(json1.is_discarded()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::GET_REQUEST_FAILED, {"https://fill.papermc.io/v3/projects/paper/versions/" + ver, "Invalid API json responce"});
        return tl::unexpected(err);
    }
    nlohmann::json jvonVersion = json1["version"];
    if(jvonVersion == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::SERVER_UNSUPPORTED_VERSION_CUSTOM, {ver});
        return tl::unexpected(err);
    }
    if(!jvonVersion.is_object()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE, {"\"version\" in https://fill.papermc.io/v3/projects/paper/versions/" + ver, "json object"});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }

    if(jvonVersion["support"] == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_NOT_FOUND, {"\"support\"", "https://fill.papermc.io/v3/projects/paper/versions/" + ver});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    if(!jvonVersion["support"].is_object()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE, {"\"support\" in https://fill.papermc.io/v3/projects/paper/versions/" + ver, "json object"});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    nlohmann::json supportStatus = jvonVersion["support"];
    if(supportStatus["status"] == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_NOT_FOUND, {"\"status\" key in \"support\"", "https://fill.papermc.io/v3/projects/paper/versions/" + ver});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    if(!supportStatus["status"].is_string()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE, {"\"status\" key in \"support\" from data https://fill.papermc.io/v3/projects/paper/versions/" + ver, "string"});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    std::string metaSupportStatus = supportStatus["status"].get<std::string>();

    if(jvonVersion["java"] == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_NOT_FOUND, {"\"java\"", "https://fill.papermc.io/v3/projects/paper/versions/" + ver});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    if(!jvonVersion["java"].is_object()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE, {"\"java\" in https://fill.papermc.io/v3/projects/paper/versions/" + ver, "json object"});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }

    nlohmann::json javaJson = jvonVersion["java"];
    nlohmann::json javaFlags = javaJson["flags"];
    nlohmann::json javaVersion = javaJson["version"];

    if(javaFlags == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_NOT_FOUND, {"\"flags\" in \"java\"", "https://fill.papermc.io/v3/projects/paper/versions/" + ver});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    if(!javaFlags.is_object()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE, {"\"flags\" key in \"java\" from data https://fill.papermc.io/v3/projects/paper/versions/" + ver, "json object"});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }

    if(javaFlags["recommended"] == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_NOT_FOUND, {"\"recommended\" in \"flags\"", "https://fill.papermc.io/v3/projects/paper/versions/" + ver});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    if(!javaFlags["recommended"].is_array()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE, {"\"recommended\" in \"flags\" from data https://fill.papermc.io/v3/projects/paper/versions/" + ver, "array of string"});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    std::vector<std::string> metaJVMFlags;
    for(auto& s : javaFlags["recommended"]){
        if(s.is_string()){
            metaJVMFlags.push_back(s.get<std::string>());
        }
    }
    metaJVMFlags.push_back("-jar");
    
    if(javaVersion == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_NOT_FOUND, {"\"version\" in \"java\"", "https://fill.papermc.io/v3/projects/paper/versions/" + ver});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    if(!javaVersion.is_object()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE, {"\"version\" key in \"java\" from data https://fill.papermc.io/v3/projects/paper/versions/" + ver, "json object"});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    if(javaVersion["minimum"] == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_NOT_FOUND, {"\"minimum\" in \"version\"", "https://fill.papermc.io/v3/projects/paper/versions/" + ver});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    if(!javaVersion["minimum"].is_number()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE, {"\"minimum\" in \"version\" from data https://fill.papermc.io/v3/projects/paper/versions/" + ver, "int"});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    std::string metaMinJava = std::to_string(javaVersion["minimum"].get<int>());

    auto res = mcsm::get("https://fill.papermc.io/v3/projects/paper/versions/" + ver + "/builds/latest", "mcsm/0.6 (https://github.com/dodoman8067/mcsm)");
    if(!res) return tl::unexpected(res.error());
    nlohmann::json json = nlohmann::json::parse(res.value(), nullptr, false);
    if(json.is_discarded()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::GET_REQUEST_FAILED, {"https://fill.papermc.io/v3/projects/paper/versions/" + ver + "/builds/latest", "Invalid API json responce"});
        return tl::unexpected(err);
    }
    if(json["id"] == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::SERVER_UNSUPPORTED_VERSION_CUSTOM, {ver + " with build latest"});
        return tl::unexpected(err);
    }
    if(!json["id"].is_number_integer()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE, {"\"id\" in https://fill.papermc.io/v3/projects/paper/versions/" + ver + "/builds/latest", "int"});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    std::string metaId = std::to_string(json["id"].get<int>());

    if(json["channel"] == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_NOT_FOUND, {"\"channel\"", "https://fill.papermc.io/v3/projects/paper/versions/" + ver + "/builds/latest"});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    if(!json["channel"].is_string()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE, {"\"channel\" in https://fill.papermc.io/v3/projects/paper/versions/" + ver + "/builds/latest", "json object"});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    std::string metaChannel = json["channel"].get<std::string>();

    if(json["downloads"] == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_NOT_FOUND, {"\"downloads\"", "https://fill.papermc.io/v3/projects/paper/versions/" + ver + "/builds/latest"});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    if(!json["downloads"].is_object()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE, {"\"downloads\" in https://fill.papermc.io/v3/projects/paper/versions/" + ver + "/builds/latest", "json object"});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    nlohmann::json jvonDownload = json["downloads"];

    if(jvonDownload["server:default"] == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_NOT_FOUND, {"\"server:default\"", "https://fill.papermc.io/v3/projects/paper/versions/" + ver + "/builds/latest"});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    if(!jvonDownload["server:default"].is_object()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE, {"\"server:default\" in https://fill.papermc.io/v3/projects/paper/versions/" + ver + "/builds/latest", "json object"});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }

    nlohmann::json jvonURL = jvonDownload["server:default"];
    if(jvonURL["url"] == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_NOT_FOUND, {"\"url\"", "https://fill.papermc.io/v3/projects/paper/versions/" + ver + "/builds/latest"});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    if(!jvonURL["url"].is_string()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE, {"\"url\" in https://fill.papermc.io/v3/projects/paper/versions/" + ver + "/builds/latest", "url"});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    std::string metaURL = jvonURL["url"].get<std::string>();

    mcsm::PaperMetadata meta;
    meta.recommendedJavaFlags = metaJVMFlags;
    meta.channel = metaChannel;
    meta.build = metaId;
    meta.downloadUrl = metaURL;
    meta.supportStatus = metaSupportStatus;
    meta.minJava = metaMinJava;
    return meta;
}

mcsm::Result<mcsm::PaperMetadata> mcsm::PaperServer::getVersionData(const std::string& ver, const std::string& build){
    if(!mcsm::isSafeString(ver)){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::UNSAFE_STRING, {ver});
        return tl::unexpected(err);
    }
    if(!mcsm::isSafeString(build) || !mcsm::is_number(build)){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::UNSAFE_STRING, {build});
        return tl::unexpected(err);
    }
    auto res1 = mcsm::get("https://fill.papermc.io/v3/projects/paper/versions/" + ver, "mcsm/0.6 (https://github.com/dodoman8067/mcsm)");
    if(!res1) return tl::unexpected(res1.error());
    nlohmann::json json1 = nlohmann::json::parse(res1.value(), nullptr, false);
    if(json1.is_discarded()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::GET_REQUEST_FAILED, {"https://fill.papermc.io/v3/projects/paper/versions/" + ver, "Invalid API json responce"});
        return tl::unexpected(err);
    }
    nlohmann::json jvonVersion = json1["version"];
    if(jvonVersion == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::SERVER_UNSUPPORTED_VERSION_CUSTOM, {ver});
        return tl::unexpected(err);
    }
    if(!jvonVersion.is_object()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE, {"\"version\" in https://fill.papermc.io/v3/projects/paper/versions/" + ver, "json object"});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }

    if(jvonVersion["support"] == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_NOT_FOUND, {"\"support\"", "https://fill.papermc.io/v3/projects/paper/versions/" + ver});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    if(!jvonVersion["support"].is_object()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE, {"\"support\" in https://fill.papermc.io/v3/projects/paper/versions/" + ver, "json object"});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    nlohmann::json supportStatus = jvonVersion["support"];
    if(supportStatus["status"] == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_NOT_FOUND, {"\"status\" key in \"support\"", "https://fill.papermc.io/v3/projects/paper/versions/" + ver});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    if(!supportStatus["status"].is_string()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE, {"\"status\" key in \"support\" from data https://fill.papermc.io/v3/projects/paper/versions/" + ver, "string"});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    std::string metaSupportStatus = supportStatus["status"].get<std::string>();

    if(jvonVersion["java"] == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_NOT_FOUND, {"\"java\"", "https://fill.papermc.io/v3/projects/paper/versions/" + ver});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    if(!jvonVersion["java"].is_object()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE, {"\"java\" in https://fill.papermc.io/v3/projects/paper/versions/" + ver, "json object"});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }

    nlohmann::json javaJson = jvonVersion["java"];
    nlohmann::json javaFlags = javaJson["flags"];
    nlohmann::json javaVersion = javaJson["version"];

    if(javaFlags == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_NOT_FOUND, {"\"flags\" in \"java\"", "https://fill.papermc.io/v3/projects/paper/versions/" + ver});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    if(!javaFlags.is_object()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE, {"\"flags\" key in \"java\" from data https://fill.papermc.io/v3/projects/paper/versions/" + ver, "json object"});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }

    if(javaFlags["recommended"] == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_NOT_FOUND, {"\"recommended\" in \"flags\"", "https://fill.papermc.io/v3/projects/paper/versions/" + ver});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    if(!javaFlags["recommended"].is_array()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE, {"\"recommended\" in \"flags\" from data https://fill.papermc.io/v3/projects/paper/versions/" + ver, "array of string"});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    std::vector<std::string> metaJVMFlags;
    for(auto& s : javaFlags["recommended"]){
        if(s.is_string()){
            metaJVMFlags.push_back(s.get<std::string>());
        }
    }
    metaJVMFlags.push_back("-jar");
    
    if(javaVersion == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_NOT_FOUND, {"\"version\" in \"java\"", "https://fill.papermc.io/v3/projects/paper/versions/" + ver});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    if(!javaVersion.is_object()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE, {"\"version\" key in \"java\" from data https://fill.papermc.io/v3/projects/paper/versions/" + ver, "json object"});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    if(javaVersion["minimum"] == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_NOT_FOUND, {"\"minimum\" in \"version\"", "https://fill.papermc.io/v3/projects/paper/versions/" + ver});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    if(!javaVersion["minimum"].is_number()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE, {"\"minimum\" in \"version\" from data https://fill.papermc.io/v3/projects/paper/versions/" + ver, "int"});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    std::string metaMinJava = std::to_string(javaVersion["minimum"].get<int>());

    auto res = mcsm::get("https://fill.papermc.io/v3/projects/paper/versions/" + ver + "/builds/" + build, "mcsm/0.6 (https://github.com/dodoman8067/mcsm)");
    if(!res) return tl::unexpected(res.error());
    nlohmann::json json = nlohmann::json::parse(res.value(), nullptr, false);
    if(json.is_discarded()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::GET_REQUEST_FAILED, {"https://fill.papermc.io/v3/projects/paper/versions/" + ver + "/builds/" + build, "Invalid API json responce"});
        return tl::unexpected(err);
    }
    if(json["id"] == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::SERVER_UNSUPPORTED_VERSION_CUSTOM, {ver + " with build " + build});
        return tl::unexpected(err);
    }

    if(json["channel"] == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_NOT_FOUND, {"\"channel\"", "https://fill.papermc.io/v3/projects/paper/versions/" + ver + "/builds/" + build});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    if(!json["channel"].is_string()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE, {"\"channel\" in https://fill.papermc.io/v3/projects/paper/versions/" + ver + "/builds/" + build, "json object"});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    std::string metaChannel = json["channel"].get<std::string>();

    if(json["downloads"] == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_NOT_FOUND, {"\"downloads\"", "https://fill.papermc.io/v3/projects/paper/versions/" + ver + "/builds/" + build});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    if(!json["downloads"].is_object()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE, {"\"downloads\" in https://fill.papermc.io/v3/projects/paper/versions/" + ver + "/builds/" + build, "json object"});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    nlohmann::json jvonDownload = json["downloads"];

    if(jvonDownload["server:default"] == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_NOT_FOUND, {"\"server:default\"", "https://fill.papermc.io/v3/projects/paper/versions/" + ver + "/builds/" + build});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    if(!jvonDownload["server:default"].is_object()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE, {"\"server:default\" in https://fill.papermc.io/v3/projects/paper/versions/" + ver + "/builds/" + build, "json object"});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }

    nlohmann::json jvonURL = jvonDownload["server:default"];
    if(jvonURL["url"] == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_NOT_FOUND, {"\"url\"", "https://fill.papermc.io/v3/projects/paper/versions/" + ver + "/builds/" + build});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    if(!jvonURL["url"].is_string()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE, {"\"url\" in https://fill.papermc.io/v3/projects/paper/versions/" + ver + "/builds/" + build, "url"});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    std::string metaURL = jvonURL["url"].get<std::string>();

    mcsm::PaperMetadata meta;
    meta.recommendedJavaFlags = metaJVMFlags;
    meta.channel = metaChannel;
    meta.build = build;
    meta.downloadUrl = metaURL;
    meta.supportStatus = metaSupportStatus;
    meta.minJava = metaMinJava;
    return meta;
}

mcsm::IntResult mcsm::PaperServer::getVersion(const std::string& ver) const {
    if(!mcsm::isSafeString(ver)){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::UNSAFE_STRING, {ver});
        return tl::unexpected(err);
    }
    auto res = mcsm::get("https://api.papermc.io/v2/projects/paper/versions/" + ver, "mcsm/0.6 (https://github.com/dodoman8067/mcsm)");
    if(!res) return tl::unexpected(res.error());
    nlohmann::json json = nlohmann::json::parse(res.value(), nullptr, false);
    if(json.is_discarded()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::GET_REQUEST_FAILED, {"https://api.papermc.io/v2/projects/paper/versions/" + ver, "Invalid API json responce"});
        return tl::unexpected(err);
    }
    if(json["builds"] == nullptr){
        return -1; // keep it this way; otherwise it returns invalid get error instead of unsupported version error
    }
    if(json["builds"].is_array()){
        nlohmann::json builds = json["builds"];
        if(builds[json["builds"].size() - 1] == nullptr || !builds[json["builds"].size() - 1].is_number_integer()) return -1;
        return builds[json["builds"].size() - 1];
    }else{
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::GET_REQUEST_FAILED, {"https://api.papermc.io/v2/projects/paper/versions/" + ver, "Invalid API json responce on property \"builds\""});
        return tl::unexpected(err);
    }
}

// used for checking if versions with specific build exists
mcsm::IntResult mcsm::PaperServer::getVersion(const std::string& ver, const std::string& build) const {
    if(!mcsm::isSafeString(build)){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::UNSAFE_STRING, {build});
        return tl::unexpected(err);
    }
    if(!mcsm::isSafeString(ver)){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::UNSAFE_STRING, {ver});
        return tl::unexpected(err);
    }
    auto res = mcsm::get("https://api.papermc.io/v2/projects/paper/versions/" + ver + "/builds/" + build);
    if(!res) return tl::unexpected(res.error());
    nlohmann::json json = nlohmann::json::parse(res.value(), nullptr, false);
    if(json.is_discarded()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::GET_REQUEST_FAILED, {"https://api.papermc.io/v2/projects/paper/versions/" + ver + "/builds/" + build, "Invalid API json responce"});
        return tl::unexpected(err);
    }

    if(json["build"] == nullptr) return -1;  // keep it this way; otherwise it returns invalid get error instead of unsupported version error
    if(!json["build"].is_number_integer()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::GET_REQUEST_FAILED, {"https://api.papermc.io/v2/projects/paper/versions/" + ver + "/builds/" + build, "Invalid API json responce on property \"build\""});
        return tl::unexpected(err);
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

const tl::expected<std::map<std::string, std::string>, mcsm::Error> mcsm::PaperServer::getRequiredValues() const {
    return tl::expected<std::map<std::string, std::string>, mcsm::Error>{
        std::map<std::string, std::string>{
                {"name", "" },
                {"minecraft_version", ""},
                {"if_paper_should_generate_recommended_profile", "false"},
                {"default_jvm_launch_profile_search_path", "current"},
                {"default_jvm_launch_profile_name", ""},
                {"server_jarfile", getTypeAsString() + ".jar"},
                {"server_build_version", "latest"},
                {"auto_server_jar_update", "true"}
        }
    };
}

mcsm::VoidResult mcsm::PaperServer::download(const std::string& version){
    auto path1 = mcsm::getCurrentPath();
    if(!path1) return tl::unexpected(path1.error());
    
    auto jar = getJarFile();
    if(!jar) return tl::unexpected(jar.error());

    return download(version, path1.value(), jar.value(), path1.value());
}

mcsm::VoidResult mcsm::PaperServer::download(const std::string& version, const std::string& path){
    auto path1 = mcsm::getCurrentPath();
    if(!path1) return tl::unexpected(path1.error());

    auto jar = getJarFile();
    if(!jar) return tl::unexpected(jar.error());

    return download(version, path, jar.value(), path1.value());
}

mcsm::VoidResult mcsm::PaperServer::download(const std::string& version, const std::string& path, const std::string& name){
    auto path1 = mcsm::getCurrentPath();
    if(!path1) return tl::unexpected(path1.error());

    return download(version, path, name, path1.value());
}

mcsm::VoidResult mcsm::PaperServer::download(const std::string& version, const std::string& path, const std::string& name, const std::string& optionPath){
    mcsm::Option opt(optionPath, "server");
    auto optExists = opt.exists();
    if(!optExists) return tl::unexpected(optExists.error());
    if(!optExists.value()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::SERVER_NOT_CONFIGURED, {});
        return tl::unexpected(err);
    }

    auto optLRes1 = opt.load(mcsm::GeneralOption::getGeneralOption().advancedParseEnabled());
    if(!optLRes1) return optLRes1;

    mcsm::ServerDataOption sDataOpt(optionPath);

    auto sLoadRes = sDataOpt.load();
    if(!sLoadRes) return sLoadRes;

    auto tVRes = opt.getValue("type");
    if(!tVRes) return tl::unexpected(tVRes.error());

    nlohmann::json typeValue = tVRes.value();

    if(typeValue == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_NOT_FOUND_PLUS_FIX, {"\"type\"", opt.getName(), "change it into \"type\": \"[yourtype]\""});
        return tl::unexpected(err);
    }
    if(!typeValue.is_string()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE_PLUS_FIX, {"\"type\"", opt.getName(), "string", "change it into \"type\": \"[yourtype]\""});
        return tl::unexpected(err);
    }
    if(typeValue != "paper"){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::SERVER_WRONG_INSTANCE_GENERATED, {"Paper"});
        return tl::unexpected(err);
    }

    auto sBVRes = opt.getValue("server_build");
    if(!sBVRes) return tl::unexpected(sBVRes.error());

    nlohmann::json serverBuildValue = sBVRes.value();
    
    if(serverBuildValue == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_NOT_FOUND_PLUS_FIX, {"\"server_build\"", opt.getName(), "add \"server_build\": \"latest\" to server.json for automatic download"});
        return tl::unexpected(err);
    }
    if(!serverBuildValue.is_string()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE_PLUS_FIX, {"\"server_build\"", opt.getName(), "string", "change it into \"server_build\": \"latest\""});
        return tl::unexpected(err);
    }
    if(serverBuildValue != "latest"){
        std::string build = serverBuildValue.get<std::string>();
        auto ver = getVersionData(version, build);
        if(!ver) return tl::unexpected(ver.error());

        std::string strVer = ver.value().build;
        mcsm::info("URL : " + ver.value().downloadUrl);
        auto res = mcsm::download(name, ver.value().downloadUrl, path, true);
        if(!res) return res;
        return sDataOpt.updateLastDownloadedBuild(strVer);
    }else{
        auto ver = getVersionData(version);
        if(!ver) return tl::unexpected(ver.error());

        std::string strVer = ver.value().build;
        mcsm::info("URL : " + ver.value().downloadUrl);
        auto res = mcsm::download(name, ver.value().downloadUrl, path, true);
        if(!res) return res;
        return sDataOpt.updateLastDownloadedBuild(strVer);
    }
}

mcsm::VoidResult mcsm::PaperServer::obtainJarFile(const std::string& version, const std::string& path, const std::string& name, const std::string& optionPath){
    return download(version, path, name, optionPath);
}

mcsm::StringResult mcsm::PaperServer::start(mcsm::ServerConfigLoader* loader, mcsm::JvmOption& option){
    // ServerOption class handles the data file stuff

    mcsm::StringResult cJarPath = loader->getServerJarPath();
    if(!cJarPath) return cJarPath;
    return start(loader, option, cJarPath.value(), loader->getHandle()->getPath());
}

mcsm::StringResult mcsm::PaperServer::start(mcsm::ServerConfigLoader* loader, mcsm::JvmOption& option, const std::string& path, const std::string& optionPath){
    // ServerOption class handles the data file stuff
    
    mcsm::StringResult jar = loader->getServerJarFile();
    if(!jar) return jar;

    mcsm::BoolResult fileExists = mcsm::fileExists(path + "/" + jar.value());
    if(!fileExists) return tl::unexpected(fileExists.error());

    if(!fileExists.value()){
        mcsm::info("Downloading " + mcsm::joinPath(path, jar.value()) + "...");
        mcsm::StringResult sVer = loader->getServerVersion();
        if(!sVer) return sVer;

        mcsm::VoidResult res = download(sVer.value(), path, jar.value(), optionPath);
        if(!res) return tl::unexpected(res.error());
    }else{
        mcsm::BoolResult doesUpdate = loader->doesAutoUpdate();
        if(!doesUpdate) return tl::unexpected(doesUpdate.error());

        if(doesUpdate.value()){
            mcsm::VoidResult res = update(path, optionPath);
            if(!res) return tl::unexpected(res.error());
        }
    }
    return Server::start(loader, option, path, optionPath);
}

mcsm::VoidResult mcsm::PaperServer::update(){
    auto path = mcsm::getCurrentPath();
    if(!path) return tl::unexpected(path.error());

    return update(path.value(), path.value());
}

mcsm::VoidResult mcsm::PaperServer::update(const std::string& optionPath){
    auto path = mcsm::getCurrentPath();
    if(!path) return tl::unexpected(path.error());

    return update(path.value(), optionPath);
}

mcsm::VoidResult mcsm::PaperServer::update(const std::string& path, const std::string& optionPath){
    // Program won't downgrade server jarfiles automatically. This is an intented feature.
    mcsm::info("Checking updates...");
    mcsm::ServerDataOption sDataOpt(optionPath);

    mcsm::VoidResult sLoadRes = sDataOpt.load();
    if(!sLoadRes) return sLoadRes;

    mcsm::ServerConfigLoader loader(optionPath);
    mcsm::VoidResult loadRes = loader.loadConfig();
    if(!loadRes) return loadRes;

    auto build = loader.getServerJarBuild();
    if(!build) return tl::unexpected(build.error());
    if(build.value() != "latest"){
        mcsm::warning("This server won't update to the latest build.");
        mcsm::warning("Change server.json into \"server_build\": \"latest\" for automatic download.");
        return {}; // todo: return errorstatus::ok with reason update passed
    }

    auto version = loader.getServerVersion();
    if(!version) return tl::unexpected(version.error());
    
    auto ver = getVersion(version.value());
    if(!ver) return tl::unexpected(ver.error());

    if(ver.value() == -1){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::SERVER_UNSUPPORTED_VERSION, {});
        return tl::unexpected(err);
    }
    auto lastBuild = sDataOpt.getLastDownloadedBuild();
    if(!lastBuild) return tl::unexpected(lastBuild.error());

    if(lastBuild.value() == std::to_string(ver.value())){
        mcsm::success("Server is up to date.");
        return {};
    }
    mcsm::success("Update found : "  + std::to_string(ver.value()) + ". Current build : " + lastBuild.value());

    auto jar = loader.getServerJarFile();
    if(!jar) return tl::unexpected(jar.error());

    auto fileExists = mcsm::fileExists(path + "/" + jar.value());
    if(!fileExists) return tl::unexpected(fileExists.error());

    if(fileExists.value()){
        auto rmfRes = mcsm::removeFile(path + "/" + jar.value());
        if(!rmfRes) return tl::unexpected(rmfRes.error());
    }
    return download(version.value(), path, jar.value(), optionPath);
}

mcsm::VoidResult mcsm::PaperServer::generate(const std::string& name, mcsm::JvmOption& option, const std::string& path, const std::string& version, const bool& autoUpdate, const std::map<std::string, std::string>& extraValues){
    mcsm::GeneralProperty* property = mcsm::GeneralOption::getGeneralOption().getProperty("skip_version_check_while_configuring");

    if(property == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_NOT_FOUND, {"skip_version_check_while_configuring", "general option", "\"skip_version_check_while_configuring\": false"});
        return tl::unexpected(err);
    }

    const nlohmann::json& propertyValue = property->getCurrentValue();
    if(!propertyValue.is_boolean()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE_PLUS_FIX, {"skip_version_check_while_configuring", "general option", "boolean", "false or true"});
        return tl::unexpected(err);
    }

    bool skipCheck = propertyValue;

    if(!skipCheck){
        auto vExists = this->getVersionData(version);
        if(!vExists){
            if(vExists.error().code != mcsm::errors::SERVER_UNSUPPORTED_VERSION_CUSTOM.code){
                return tl::unexpected(vExists.error());
            }
        }
    }
    if(extraValues.find("if_paper_should_generate_recommended_profile")->second == "true"){
        auto vData = this->getVersionData(version);
        if(!vData){
            if(vData.error().code != mcsm::errors::SERVER_UNSUPPORTED_VERSION_CUSTOM.code){
                return tl::unexpected(vData.error());
            }
        }
        mcsm::JvmOption option(mcsm::unwrapOrExit(mcsm::jvmProfileFromSearchTarget("_paper_autogenerated", mcsm::SearchTarget::CURRENT, path)));
        auto initRes = option.init();
        if(!initRes){
            mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_WARNING_NOEXIT, mcsm::errors::FILE_CREATE_FAILED, {"for internal reason: " + initRes.error().message});
            return tl::unexpected(err);
        }

        auto jvm = mcsm::detectJava();
        if(!jvm){
            mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_WARNING_NOEXIT, mcsm::errors::FILE_CREATE_FAILED, {"for internal reason: " + jvm.error().message});
            return tl::unexpected(err);
        }

        auto createRes = option.create(jvm.value(), vData.value().recommendedJavaFlags);
        if(!createRes){
            mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_WARNING_NOEXIT, mcsm::errors::FILE_CREATE_FAILED, {"for internal reason: " + createRes.error().message});
            return tl::unexpected(err);
        }
        mcsm::info("Java Virtual Machine launch profile generated : ");
        mcsm::info("Profile name : _paper_autogenerated");
        mcsm::info("Profile location : " + path);
        mcsm::info("JVM path : " + jvm.value());

        if(!vData.value().recommendedJavaFlags.empty()) {
            std::cout << "[mcsm/INFO] JVM arguments : ";
            for(std::string_view args : vData.value().recommendedJavaFlags) {
                std::cout << args << " ";
            }
            std::cout << "\n";
        }
        mcsm::info("Server arguments : nogui");
    }

    mcsm::ServerDataOption opt(path);

    // No need to call opt.load() here. create() in ServerDataOption will call it eventually
    return configure(version, this, &opt, path, name, option, autoUpdate, extraValues.find("server_build_version")->second, extraValues.find("server_jarfile")->second);
}

mcsm::BoolResult mcsm::PaperServer::hasVersion(const std::string& version) const {
    return getVersion(version) != -1;
}

std::string mcsm::PaperServer::getTypeAsString() const {
    return "paper";
}