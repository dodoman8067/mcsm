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

#include <mcsm/server/type/proxy/velocity_server.h>

mcsm::VelocityServer::VelocityServer(){}

mcsm::VelocityServer::~VelocityServer(){}

// VelocityServer provides a method that returns the 'ver' required in getVersion method as users don't usually type versions like 3.3.0-SNAPSHOT
// it is something like a method that returns latest mc version

mcsm::Result<mcsm::VelocityMetaData> mcsm::VelocityServer::getVersionData(){
    auto res1 = mcsm::get("https://fill.papermc.io/v3/projects/velocity/versions", "mcsm/0.6 (https://github.com/dodoman8067/mcsm)");
    if(!res1) return tl::unexpected(res1.error());
    nlohmann::json json1 = nlohmann::json::parse(res1.value(), nullptr, false);
    if(json1.is_discarded()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::GET_REQUEST_FAILED, {"https://fill.papermc.io/v3/projects/velocity/versions", "Invalid API json responce"});
        return tl::unexpected(err);
    }
    auto verArray = json1["versions"];
    if(verArray == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_NOT_FOUND, {"\"versions\"", "https://fill.papermc.io/v3/projects/velocity/versions"});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    if(!verArray.is_array()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE, {"\"versions\" in https://fill.papermc.io/v3/projects/velocity/versions", "array"});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
}

mcsm::Result<mcsm::VelocityMetaData> mcsm::VelocityServer::getVersionData(const std::string& ver){
    if(!mcsm::isSafeString(ver)){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::UNSAFE_STRING, {ver});
        return tl::unexpected(err);
    }
    auto res1 = mcsm::get("https://fill.papermc.io/v3/projects/velocity/versions/" + ver, "mcsm/0.6 (https://github.com/dodoman8067/mcsm)");
    if(!res1) return tl::unexpected(res1.error());
    nlohmann::json json1 = nlohmann::json::parse(res1.value(), nullptr, false);
    if(json1.is_discarded()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::GET_REQUEST_FAILED, {"https://fill.papermc.io/v3/projects/velocity/versions/" + ver, "Invalid API json responce"});
        return tl::unexpected(err);
    }
    nlohmann::json jvonVersion = json1["version"];
    if(jvonVersion == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::SERVER_UNSUPPORTED_VERSION_CUSTOM, {ver});
        return tl::unexpected(err);
    }
    if(!jvonVersion.is_object()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE, {"\"version\" in https://fill.papermc.io/v3/projects/velocity/versions/" + ver, "json object"});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }

    if(jvonVersion["support"] == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_NOT_FOUND, {"\"support\"", "https://fill.papermc.io/v3/projects/velocity/versions/" + ver});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    if(!jvonVersion["support"].is_object()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE, {"\"support\" in https://fill.papermc.io/v3/projects/velocity/versions/" + ver, "json object"});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    nlohmann::json supportStatus = jvonVersion["support"];
    if(supportStatus["status"] == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_NOT_FOUND, {"\"status\" key in \"support\"", "https://fill.papermc.io/v3/projects/velocity/versions/" + ver});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    if(!supportStatus["status"].is_string()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE, {"\"status\" key in \"support\" from data https://fill.papermc.io/v3/projects/velocity/versions/" + ver, "string"});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    std::string metaSupportStatus = supportStatus["status"].get<std::string>();

    if(jvonVersion["java"] == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_NOT_FOUND, {"\"java\"", "https://fill.papermc.io/v3/projects/velocity/versions/" + ver});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    if(!jvonVersion["java"].is_object()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE, {"\"java\" in https://fill.papermc.io/v3/projects/velocity/versions/" + ver, "json object"});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }

    nlohmann::json javaJson = jvonVersion["java"];
    nlohmann::json javaFlags = javaJson["flags"];
    nlohmann::json javaVersion = javaJson["version"];

    if(javaFlags == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_NOT_FOUND, {"\"flags\" in \"java\"", "https://fill.papermc.io/v3/projects/velocity/versions/" + ver});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    if(!javaFlags.is_object()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE, {"\"flags\" key in \"java\" from data https://fill.papermc.io/v3/projects/velocity/versions/" + ver, "json object"});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }

    if(javaFlags["recommended"] == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_NOT_FOUND, {"\"recommended\" in \"flags\"", "https://fill.papermc.io/v3/projects/velocity/versions/" + ver});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    if(!javaFlags["recommended"].is_array()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE, {"\"recommended\" in \"flags\" from data https://fill.papermc.io/v3/projects/velocity/versions/" + ver, "array of string"});
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
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_NOT_FOUND, {"\"version\" in \"java\"", "https://fill.papermc.io/v3/projects/velocity/versions/" + ver});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    if(!javaVersion.is_object()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE, {"\"version\" key in \"java\" from data https://fill.papermc.io/v3/projects/velocity/versions/" + ver, "json object"});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    if(javaVersion["minimum"] == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_NOT_FOUND, {"\"minimum\" in \"version\"", "https://fill.papermc.io/v3/projects/velocity/versions/" + ver});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    if(!javaVersion["minimum"].is_number()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE, {"\"minimum\" in \"version\" from data https://fill.papermc.io/v3/projects/velocity/versions/" + ver, "int"});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    std::string metaMinJava = std::to_string(javaVersion["minimum"].get<int>());

    auto res = mcsm::get("https://fill.papermc.io/v3/projects/velocity/versions/" + ver + "/builds/latest", "mcsm/0.6 (https://github.com/dodoman8067/mcsm)");
    if(!res) return tl::unexpected(res.error());
    nlohmann::json json = nlohmann::json::parse(res.value(), nullptr, false);
    if(json.is_discarded()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::GET_REQUEST_FAILED, {"https://fill.papermc.io/v3/projects/velocity/versions/" + ver + "/builds/latest", "Invalid API json responce"});
        return tl::unexpected(err);
    }
    if(json["id"] == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::SERVER_UNSUPPORTED_VERSION_CUSTOM, {ver + " with build latest"});
        return tl::unexpected(err);
    }
    if(!json["id"].is_number_integer()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE, {"\"id\" in https://fill.papermc.io/v3/projects/velocity/versions/" + ver + "/builds/latest", "int"});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    std::string metaId = std::to_string(json["id"].get<int>());

    if(json["channel"] == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_NOT_FOUND, {"\"channel\"", "https://fill.papermc.io/v3/projects/velocity/versions/" + ver + "/builds/latest"});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    if(!json["channel"].is_string()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE, {"\"channel\" in https://fill.papermc.io/v3/projects/velocity/versions/" + ver + "/builds/latest", "json object"});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    std::string metaChannel = json["channel"].get<std::string>();

    if(json["downloads"] == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_NOT_FOUND, {"\"downloads\"", "https://fill.papermc.io/v3/projects/velocity/versions/" + ver + "/builds/latest"});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    if(!json["downloads"].is_object()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE, {"\"downloads\" in https://fill.papermc.io/v3/projects/velocity/versions/" + ver + "/builds/latest", "json object"});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    nlohmann::json jvonDownload = json["downloads"];

    if(jvonDownload["server:default"] == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_NOT_FOUND, {"\"server:default\"", "https://fill.papermc.io/v3/projects/velocity/versions/" + ver + "/builds/latest"});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    if(!jvonDownload["server:default"].is_object()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE, {"\"server:default\" in https://fill.papermc.io/v3/projects/velocity/versions/" + ver + "/builds/latest", "json object"});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }

    nlohmann::json jvonURL = jvonDownload["server:default"];
    if(jvonURL["url"] == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_NOT_FOUND, {"\"url\"", "https://fill.papermc.io/v3/projects/velocity/versions/" + ver + "/builds/latest"});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    if(!jvonURL["url"].is_string()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE, {"\"url\" in https://fill.papermc.io/v3/projects/velocity/versions/" + ver + "/builds/latest", "url"});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    std::string metaURL = jvonURL["url"].get<std::string>();

    mcsm::VelocityMetaData meta;
    meta.recommendedJavaFlags = metaJVMFlags;
    meta.channel = metaChannel;
    meta.build = metaId;
    meta.downloadUrl = metaURL;
    meta.supportStatus = metaSupportStatus;
    meta.minJava = metaMinJava;
    return meta;
}

mcsm::Result<mcsm::VelocityMetaData> mcsm::VelocityServer::getVersionData(const std::string& ver, const std::string& build){
    if(!mcsm::isSafeString(ver)){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::UNSAFE_STRING, {ver});
        return tl::unexpected(err);
    }
    if(!mcsm::isSafeString(build) || !mcsm::is_number(build)){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::UNSAFE_STRING, {build});
        return tl::unexpected(err);
    }
    auto res1 = mcsm::get("https://fill.papermc.io/v3/projects/velocity/versions/" + ver, "mcsm/0.6 (https://github.com/dodoman8067/mcsm)");
    if(!res1) return tl::unexpected(res1.error());
    nlohmann::json json1 = nlohmann::json::parse(res1.value(), nullptr, false);
    if(json1.is_discarded()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::GET_REQUEST_FAILED, {"https://fill.papermc.io/v3/projects/velocity/versions/" + ver, "Invalid API json responce"});
        return tl::unexpected(err);
    }
    nlohmann::json jvonVersion = json1["version"];
    if(jvonVersion == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::SERVER_UNSUPPORTED_VERSION_CUSTOM, {ver});
        return tl::unexpected(err);
    }
    if(!jvonVersion.is_object()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE, {"\"version\" in https://fill.papermc.io/v3/projects/velocity/versions/" + ver, "json object"});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }

    if(jvonVersion["support"] == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_NOT_FOUND, {"\"support\"", "https://fill.papermc.io/v3/projects/velocity/versions/" + ver});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    if(!jvonVersion["support"].is_object()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE, {"\"support\" in https://fill.papermc.io/v3/projects/velocity/versions/" + ver, "json object"});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    nlohmann::json supportStatus = jvonVersion["support"];
    if(supportStatus["status"] == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_NOT_FOUND, {"\"status\" key in \"support\"", "https://fill.papermc.io/v3/projects/velocity/versions/" + ver});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    if(!supportStatus["status"].is_string()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE, {"\"status\" key in \"support\" from data https://fill.papermc.io/v3/projects/velocity/versions/" + ver, "string"});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    std::string metaSupportStatus = supportStatus["status"].get<std::string>();

    if(jvonVersion["java"] == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_NOT_FOUND, {"\"java\"", "https://fill.papermc.io/v3/projects/velocity/versions/" + ver});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    if(!jvonVersion["java"].is_object()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE, {"\"java\" in https://fill.papermc.io/v3/projects/velocity/versions/" + ver, "json object"});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }

    nlohmann::json javaJson = jvonVersion["java"];
    nlohmann::json javaFlags = javaJson["flags"];
    nlohmann::json javaVersion = javaJson["version"];

    if(javaFlags == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_NOT_FOUND, {"\"flags\" in \"java\"", "https://fill.papermc.io/v3/projects/velocity/versions/" + ver});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    if(!javaFlags.is_object()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE, {"\"flags\" key in \"java\" from data https://fill.papermc.io/v3/projects/velocity/versions/" + ver, "json object"});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }

    if(javaFlags["recommended"] == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_NOT_FOUND, {"\"recommended\" in \"flags\"", "https://fill.papermc.io/v3/projects/velocity/versions/" + ver});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    if(!javaFlags["recommended"].is_array()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE, {"\"recommended\" in \"flags\" from data https://fill.papermc.io/v3/projects/velocity/versions/" + ver, "array of string"});
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
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_NOT_FOUND, {"\"version\" in \"java\"", "https://fill.papermc.io/v3/projects/velocity/versions/" + ver});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    if(!javaVersion.is_object()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE, {"\"version\" key in \"java\" from data https://fill.papermc.io/v3/projects/velocity/versions/" + ver, "json object"});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    if(javaVersion["minimum"] == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_NOT_FOUND, {"\"minimum\" in \"version\"", "https://fill.papermc.io/v3/projects/velocity/versions/" + ver});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    if(!javaVersion["minimum"].is_number()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE, {"\"minimum\" in \"version\" from data https://fill.papermc.io/v3/projects/velocity/versions/" + ver, "int"});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    std::string metaMinJava = std::to_string(javaVersion["minimum"].get<int>());

    auto res = mcsm::get("https://fill.papermc.io/v3/projects/velocity/versions/" + ver + "/builds/" + build, "mcsm/0.6 (https://github.com/dodoman8067/mcsm)");
    if(!res) return tl::unexpected(res.error());
    nlohmann::json json = nlohmann::json::parse(res.value(), nullptr, false);
    if(json.is_discarded()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::GET_REQUEST_FAILED, {"https://fill.papermc.io/v3/projects/velocity/versions/" + ver + "/builds/" + build, "Invalid API json responce"});
        return tl::unexpected(err);
    }
    if(json["id"] == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::SERVER_UNSUPPORTED_VERSION_CUSTOM, {ver + " with build " + build});
        return tl::unexpected(err);
    }

    if(json["channel"] == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_NOT_FOUND, {"\"channel\"", "https://fill.papermc.io/v3/projects/velocity/versions/" + ver + "/builds/" + build});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    if(!json["channel"].is_string()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE, {"\"channel\" in https://fill.papermc.io/v3/projects/velocity/versions/" + ver + "/builds/" + build, "json object"});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    std::string metaChannel = json["channel"].get<std::string>();

    if(json["downloads"] == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_NOT_FOUND, {"\"downloads\"", "https://fill.papermc.io/v3/projects/velocity/versions/" + ver + "/builds/" + build});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    if(!json["downloads"].is_object()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE, {"\"downloads\" in https://fill.papermc.io/v3/projects/velocity/versions/" + ver + "/builds/" + build, "json object"});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    nlohmann::json jvonDownload = json["downloads"];

    if(jvonDownload["server:default"] == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_NOT_FOUND, {"\"server:default\"", "https://fill.papermc.io/v3/projects/velocity/versions/" + ver + "/builds/" + build});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    if(!jvonDownload["server:default"].is_object()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE, {"\"server:default\" in https://fill.papermc.io/v3/projects/velocity/versions/" + ver + "/builds/" + build, "json object"});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }

    nlohmann::json jvonURL = jvonDownload["server:default"];
    if(jvonURL["url"] == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_NOT_FOUND, {"\"url\"", "https://fill.papermc.io/v3/projects/velocity/versions/" + ver + "/builds/" + build});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    if(!jvonURL["url"].is_string()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE, {"\"url\" in https://fill.papermc.io/v3/projects/velocity/versions/" + ver + "/builds/" + build, "url"});
        err.solution = "This is likely caused by PaperMC API breakage.";
        return tl::unexpected(err);
    }
    std::string metaURL = jvonURL["url"].get<std::string>();

    mcsm::VelocityMetaData meta;
    meta.recommendedJavaFlags = metaJVMFlags;
    meta.channel = metaChannel;
    meta.build = build;
    meta.downloadUrl = metaURL;
    meta.supportStatus = metaSupportStatus;
    meta.minJava = metaMinJava;
    return meta;
}

mcsm::IntResult mcsm::VelocityServer::getVersion(const std::string& ver) const {
    if(!mcsm::isSafeString(ver)){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::UNSAFE_STRING, {ver});
        return tl::unexpected(err);
    }
    auto res = mcsm::get("https://api.papermc.io/v2/projects/velocity/versions/" + ver);
    if(!res) return tl::unexpected(res.error());
    nlohmann::json json = nlohmann::json::parse(res.value(), nullptr, false);
    if(json.is_discarded()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_PARSE_FAILED_CANNOT_BE_MODIFIED, {});
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
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::GET_REQUEST_FAILED, {"https://api.papermc.io/v2/projects/velocity/versions/" + ver, "Invalid API json responce type on property \"builds\""});
        return tl::unexpected(err);
    }
}

// used for checking if versions with specific build exists
mcsm::IntResult mcsm::VelocityServer::getVersion(const std::string& ver, const std::string& build) const {
    if(!mcsm::isSafeString(build)){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::UNSAFE_STRING, {build});
        return tl::unexpected(err);
    }
    if(!mcsm::isSafeString(ver)){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::UNSAFE_STRING, {ver});
        return tl::unexpected(err);
    }
    auto res = mcsm::get("https://api.papermc.io/v2/projects/velocity/versions/" + ver + "/builds/" + build);
    if(!res) return tl::unexpected(res.error());
    nlohmann::json json = nlohmann::json::parse(res.value(), nullptr, false);
    if(json.is_discarded()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_PARSE_FAILED_CANNOT_BE_MODIFIED, {});
        return tl::unexpected(err);
    }

    if(json["build"] == nullptr) return -1; // keep it this way; otherwise it returns invalid get error instead of unsupported version error

    if(!json["build"].is_number_integer()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::GET_REQUEST_FAILED, {"https://api.papermc.io/v2/projects/velocity/versions/" + ver, "Invalid API json responce on property \"build\""});
        return tl::unexpected(err);
    }else{
        return json["build"];
    }
}

mcsm::StringResult mcsm::VelocityServer::getLatestVersion() const {
    auto res = mcsm::get("https://api.papermc.io/v2/projects/velocity");
    if(!res) return tl::unexpected(res.error());
    nlohmann::json json = nlohmann::json::parse(res.value(), nullptr, false);
    if(json.is_discarded()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_PARSE_FAILED_CANNOT_BE_MODIFIED, {});
        return tl::unexpected(err);
    }

    if(json["versions"] == nullptr || !json["versions"].is_array()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::GET_REQUEST_FAILED, {"https://api.papermc.io/v2/projects/velocity", "Invalid API json responce on property \"versions\""});
        return tl::unexpected(err);
    }else{
        nlohmann::json builds = json["versions"];
        if(builds[json["versions"].size() - 1] == nullptr || !builds[json["versions"].size() - 1].is_string()) return "";
        return builds[json["versions"].size() - 1];
    }
}

std::vector<std::string> mcsm::VelocityServer::getAvailableVersions(){
    std::vector<std::string> versions;
    for(const std::string& s : mcsm::getMinecraftVersions()){
        versions.push_back(s);
    }
    return versions;
}

std::string mcsm::VelocityServer::getSupportedVersions() const {
    return "1.7.2~";
}

std::string mcsm::VelocityServer::getBasedServer() const {
    return "none";
}

std::string mcsm::VelocityServer::getWebSite() const {
    return "https://papermc.io";
}

std::string mcsm::VelocityServer::getGitHub() const {
    return "https://github.com/PaperMC/Velocity";
}

mcsm::VoidResult mcsm::VelocityServer::download(const std::string& version){
    auto path1 = mcsm::getCurrentPath();
    if(!path1) return tl::unexpected(path1.error());

    auto jar = getJarFile();
    if(!jar) return tl::unexpected(jar.error());

    return download(version, path1.value(), jar.value(), path1.value());
}

mcsm::VoidResult mcsm::VelocityServer::download(const std::string& version, const std::string& path){
    auto path1 = mcsm::getCurrentPath();
    if(!path1) return tl::unexpected(path1.error());

    auto jar = getJarFile();
    if(!jar) return tl::unexpected(jar.error());

    return download(version, path, jar.value(), path1.value());
}

mcsm::VoidResult mcsm::VelocityServer::download(const std::string& version, const std::string& path, const std::string& name){
    auto path1 = mcsm::getCurrentPath();
    if(!path1) return tl::unexpected(path1.error());

    return download(version, path, name, path1.value());
}

mcsm::VoidResult mcsm::VelocityServer::download(const std::string& version, const std::string& path, const std::string& name, const std::string& optionPath){
    mcsm::Option opt(optionPath, "server");
    auto optExistsResult = opt.exists();
    if(!optExistsResult) return tl::unexpected(optExistsResult.error());

    bool optExists = optExistsResult.value();
    if(!optExists){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::SERVER_NOT_CONFIGURED, {});
        return tl::unexpected(err);
    }

    auto optLRes = opt.load(mcsm::GeneralOption::getGeneralOption().advancedParseEnabled());
    if(!optLRes) return optLRes;

    mcsm::ServerDataOption sDataOpt(optionPath);

    auto sdoLRes = sDataOpt.load();
    if(!sdoLRes) return sdoLRes;

    auto typeValueRes = opt.getValue("type");
    if(!typeValueRes) return tl::unexpected(typeValueRes.error());

    nlohmann::json typeValue = typeValueRes.value();

    if(typeValue == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE_PLUS_FIX, {"\"type\"", opt.getName(), "change it into \"type\": \"[yourtype]\""});
        return tl::unexpected(err);
    }
    if(!typeValue.is_string()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE_PLUS_FIX, {"\"type\"", opt.getName(), "string", "change it into \"type\": \"[yourtype]\""});
        return tl::unexpected(err);
    }
    if(typeValue != "velocity"){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::SERVER_WRONG_INSTANCE_GENERATED, {"Velocity"});
        return tl::unexpected(err);
    }

    auto serverBuildValueRes = opt.getValue("server_build");
    if(!serverBuildValueRes) return tl::unexpected(serverBuildValueRes.error());
    nlohmann::json serverBuildValue = serverBuildValueRes.value();
    
    if(serverBuildValue == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE_PLUS_FIX, {"\"server_build\"", opt.getName(), "add \"server_build\": \"latest\" to server.json for automatic download"});
        return tl::unexpected(err);
    }
    if(!serverBuildValue.is_string()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::JSON_WRONG_TYPE_PLUS_FIX, {"\"server_build\"", opt.getName(), "string", "change it into \"server_build\": \"latest\""});
        return tl::unexpected(err);
    }
    if(serverBuildValue != "latest"){
        std::string build = serverBuildValue.get<std::string>();
        auto ver = getVersion(version, build);
        if(!ver) return tl::unexpected(ver.error());

        if(ver.value() == -1){
            mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::SERVER_UNSUPPORTED_VERSION, {build});
            return tl::unexpected(err);
        }
        std::string strVer = std::to_string(ver.value());
        std::string url = "https://api.papermc.io/v2/projects/velocity/versions/" + version + "/builds/" + strVer + "/downloads/velocity-" + version + "-" + strVer + ".jar";
        mcsm::info("URL : " + url);
        auto res = mcsm::download(name, url, path, true);
        if(!res) return res;
        auto uptLDBRes = sDataOpt.updateLastDownloadedBuild(strVer);
        return uptLDBRes;
    }else{
        auto ver = getVersion(version);
        if(!ver) return tl::unexpected(ver.error());
        
        if(ver.value() == -1){
            mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::SERVER_UNSUPPORTED_VERSION, {});
            return tl::unexpected(err);
        }
        std::string strVer = std::to_string(ver.value());
        std::string url = "https://api.papermc.io/v2/projects/velocity/versions/" + version + "/builds/" + strVer + "/downloads/velocity-" + version + "-" + strVer + ".jar";
        mcsm::info("URL : " + url);
        auto res = mcsm::download(name, url, path, true);
        if(!res) return res;
        auto uptLDBRes = sDataOpt.updateLastDownloadedBuild(strVer);
        return uptLDBRes;
    }
}

mcsm::VoidResult mcsm::VelocityServer::obtainJarFile(const std::string& version, const std::string& path, const std::string& name, const std::string& optionPath){
    return download(version, path, name, optionPath);
}

mcsm::StringResult mcsm::VelocityServer::start(mcsm::ServerConfigLoader* loader, mcsm::JvmOption& option){
    // ServerOption class handles the data file stuff

    mcsm::StringResult cJarPath = loader->getServerJarPath();
    if(!cJarPath) return cJarPath;
    return start(loader, option, cJarPath.value(), loader->getHandle()->getPath());
}

mcsm::StringResult mcsm::VelocityServer::start(mcsm::ServerConfigLoader* loader, mcsm::JvmOption& option, const std::string& path, const std::string& optionPath){
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

mcsm::VoidResult mcsm::VelocityServer::update(){
    auto path = mcsm::getCurrentPath();
    if(!path) return tl::unexpected(path.error());

    return update(path.value(), path.value());
}

mcsm::VoidResult mcsm::VelocityServer::update(const std::string& optionPath){
    auto path = mcsm::getCurrentPath();
    if(!path) return tl::unexpected(path.error());

    return update(path.value(), optionPath);
}

mcsm::VoidResult mcsm::VelocityServer::update(const std::string& path, const std::string& optionPath){
    // Program won't downgrade server jarfiles automatically. This is an intented feature.
    mcsm::info("Checking updates...");
    mcsm::ServerDataOption sDataOpt(optionPath);

    auto sdoLRes = sDataOpt.load();
    if(!sdoLRes) return sdoLRes;

    mcsm::ServerConfigLoader loader(optionPath);
    mcsm::VoidResult loadRes = loader.loadConfig();
    if(!loadRes) return loadRes;

    auto build = loader.getServerJarBuild();
    if(!build) return tl::unexpected(build.error());

    if(build.value() != "latest"){
        mcsm::warning("This server won't update to the latest build.");
        mcsm::warning("Change server.json into \"server_build\": \"latest\" for automatic download.");
        return {};
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
        auto rmRes = mcsm::removeFile(path + "/" + jar.value());
        if(!rmRes) return tl::unexpected(rmRes.error());
    }
    return download(version.value(), path, jar.value(), optionPath);
}

mcsm::VoidResult mcsm::VelocityServer::generate(const std::string& name, mcsm::JvmOption& option, const std::string& path, const std::string& version, const bool& autoUpdate, const std::map<std::string, std::string>& extraValues){
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
        auto vExists = this->hasVersion(version);
        if(!vExists){
            return tl::unexpected(vExists.error());
        }
        if(!vExists.value()){
            mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::MCSM_FAIL, mcsm::errors::SERVER_UNSUPPORTED_VERSION, {});
            return tl::unexpected(err);
        }
    }
    mcsm::ServerDataOption opt(path);
    
    // No need to call opt.load() here. create() in ServerDataOption will call it eventually
    return configure(version, this, &opt, path, name, option, autoUpdate, extraValues.find("server_build_version")->second, extraValues.find("server_jarfile")->second);
}

mcsm::BoolResult mcsm::VelocityServer::hasVersion(const std::string& version) const {
    auto vRes = getVersion(version);
    if(!vRes) return tl::unexpected(vRes.error());
    return vRes.value() != -1;
}

mcsm::ServerType mcsm::VelocityServer::getType() const {
    return mcsm::ServerType::VELOCITY;
}

std::string mcsm::VelocityServer::getTypeAsString() const {
    return "velocity";
}