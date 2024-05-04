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

#ifndef __MCSM_RESULT_H__
#define __MCSM_RESULT_H__

#include <vector>
#include <mutex>
#include <mcsm/util/cli/logging.h>

namespace mcsm {
    enum class ResultType {
        MCSM_SUCCESS,
        MCSM_OK,
        MCSM_WARN,
        MCSM_WARN_NOEXIT,
        MCSM_FAIL,
        MCSM_UNKNOWN
    };

    enum class ResultCode {
        JSON_WRONG_TYPE,
        JSON_WRONG_TYPE_CAN_BE_FIXED,
        JSON_NOT_FOUND,
        JSON_NOT_FOUND_CAN_BE_FIXED,
        JSON_PARSE_FAILED,
        JSON_PARSE_FAILED_CANNOT_BE_FIXED,
        SERVER_ALREADY_CONFIGURED,
        SERVER_NOT_CONFIGURED,
        SERVER_DATA_NOT_CONFIGURED,
        SERVER_DATA_ALREADY_CONFIGURED,
        SERVER_WRONG_INSTANCE_GENERATED,
        SERVER_UNSUPPORTED_VERSION,
        SERVER_DEFAULT_JVM_PROFILE_NOT_FOUND,
        SERVER_DEFAULT_JVM_PROFILE_NAME_NOT_FOUND,
        JVM_PROFILE_NOT_FOUND,
        JAVA_NOT_FOUND,
        FILE_CREATION_FAILED,
        FILE_SAVING_FAILED,
        FILE_READING_FAILED,
        FILE_NOT_FOUND,
        CURL_INIT_FAILED,
        HTTP_GET_REQUEST_FAILED,
        HTTP_DOWNLOAD_REQUEST_FAILED,
        HTTP_DOWNLOAD_TARGET_NOT_TEXT,
        PLATFORM_NOT_SUPPORTED,
        SUCCESS,
        OTHER
    };

    class Result {
    private:
        mcsm::ResultType result;
        std::vector<std::string> message;
        std::pair<mcsm::ResultType, std::vector<std::string>> resPair;
    public:
        Result(const mcsm::ResultType& type, const std::vector<std::string>& message);
        ~Result();
        mcsm::ResultType getResult() const;
        std::vector<std::string> getMessage() const;
        std::pair<mcsm::ResultType, std::vector<std::string>> getResultPair() const;
        bool isSuccess() const;
        void printMessage();
        void printMessage(const mcsm::ResultType& type);
    };

    namespace message_utils {
        inline std::vector<std::string> jsonWrongType(const std::string& key, const std::string& type){
            return {
                "Value " + key + " has to be a(n) " + type + ", but it's not.",
                "Manually editing the launch profile might have caused this issue.",
                "If you know what you're doing, I believe you that you know how to handle this issue.",
                "Please report this to GitHub (https://github.com/dodoman8067/mcsm) if you think this is a software issue."
            };
        }

        inline std::vector<std::string> jsonWrongTypePlusFix(const std::string& key, const std::string& name, const std::string& type, const std::string& changeInto){
            return {
                "Value " + key + " option in " + name + " must be a(n) " + type + ".",
                "To fix, " + changeInto + "."
            };
        }

        inline std::vector<std::string> jsonNotFound(const std::string& key, const std::string& name){
            return {
                "No "+ key +" value specified in file " + name,
                "Manually editing the launch profile might have caused this issue.",
                "If you know what you're doing, I believe you that you know how to handle this issue.",
                "Please report this to GitHub (https://github.com/dodoman8067/mcsm) if you think this is a software issue."
            };
        }

        inline std::vector<std::string> jsonNotFoundPlusFix(const std::string& key, const std::string& name, const std::string& changeInto){
            return {
                "Missing " + key + " option in " + name,
                "To fix, " + changeInto + "."
            };
        }

        inline std::vector<std::string> jsonParseFailed(const std::string& name){
            return {
                "Failed to parse json " + name,
                "There are many reasons why you see this error.",
                "1. Comments: Comments are not allowed in JSON. If you used the comment syntax like other programming languages, you must remove them and try again.",
                "2. Unclosed Brackets: Check if all curly braces '{}' and square brackets '[]' are properly closed.",
                "3. Trailing Commas: Trailing Commas are not allowed in JSON. Make sure that end of the element doesn't end with comma.",
                "4. Incorrect Escape Sequences: Check if all escaped are used properly.",
                "5. Data types: Double check each element is storing the correct data types.",
                "6. File Access Permissions: Check if your OS or security software is blocking the access to the file. Creating the file in system directories may cause this problem."
            };
        }

        inline std::vector<std::string> jsonParseFailedCannotBeModified(){
            return {
                "Parse of json failed.",
                "If you believe that this is a software issue, please report this to GitHub. (https://github.com/dodoman8067/mcsm)"
            };
        }

        inline std::vector<std::string> serverAlreadyConfigured(){
            return {
                "Server is already configured in this directory.",
                "Please try again in other directories."
            };
        }

        inline std::vector<std::string> serverAlreadyConfigured(const std::string& dir){
            return {
                "Server already configured in " + dir + ".",
                "Please try again in other directories."
            };
        }

        inline std::vector<std::string> serverNotConfigured(){
            return {
                "Server isn't configured in this directory",
                "Run \"mcsm init\" to configure a server."
            };
        }

        inline std::vector<std::string> serverNotConfigured(const std::string& dir){
            return {
                "Server not configured in " + dir + ".",
                "Run \"mcsm init\" to configure a server."
            };
        }

        inline std::vector<std::string> serverDataAlreadyConfigured(){
            return {
                "Server data config is already in its place.",
                "Please try again in other directories."
            };
        }

        inline std::vector<std::string> serverDataNotConfigured(){
            return {
                "Server data file isn't configured in this directory",
                "Modifying .mcsm folder might have caused this issue.",
                "If you believe that this is a software issue and not your fault, please report this to GitHub. (https://github.com/dodoman8067/mcsm)"
            };
        }

        inline std::vector<std::string> serverWrongInstanceGenerated(const std::string& serverName){
            return { 
                "Specified server path's server option wasn't for" + serverName + "servers.",
                "Please try again in other directories." 
            };
        }

        inline std::vector<std::string> serverUnsupportedVersion(){
            return {
                "Unsupported version.",
                "Please try again with a different version."
            };
        }

        inline std::vector<std::string> serverDefaultProfileNotFound(const std::string& name){
            return {
                "No default launch profile specified in file " + name,
                "Manually editing the launch profile might have caused this issue.",
                "If you know what you're doing, I believe you that you know how to handle this issue.",
                "Please report this to GitHub (https://github.com/dodoman8067/mcsm) if you think this is a software issue."
            };
        }

        inline std::vector<std::string> serverDefaultProfileNameNotFound(const std::string& name){
            return {
                "No default launch profile name specified in file " + name,
                "Manually editing the launch profile might have caused this issue.",
                "If you know what you're doing, I believe you that you know how to handle this issue.",
                "Please report this to GitHub (https://github.com/dodoman8067/mcsm) if you think this is a software issue."
            };
        }

        inline std::vector<std::string> serverUnsupportedVersion(const std::string& build){
            return {
                "Unsupported version : " + build,
                "Please try again with a different version."
            };
        }

        inline std::vector<std::string> jvmProfileNotFound(){
            return {
                "This jvm option does not exist.",
                "Please edit the launch profile after creating."
            };
        }

        inline std::vector<std::string> jvmDetectionFailed(){
            return {
                "JVM detection failed.",
                "This error can be fixed by setting JAVA_HOME enviroment variable.",
            };
        }

        inline std::vector<std::string> fileCreateFailed(const std::string& name){
            return {
                "Cannot create directory/file " + name,
                "This might have caused because of permissions or file name problems.",
                "Please report this to GitHub (https://github.com/dodoman8067/mcsm) if you think this is a software issue."
            };
        }

        inline std::vector<std::string> fileSaveFailed(const std::string& name){
            return {
                "Cannot save file " + name,
                "This might have caused because of permissions or file name problems.",
                "Please report this to GitHub (https://github.com/dodoman8067/mcsm) if you think this is a software issue."
            };
        }

        inline std::vector<std::string> fileOpenFailed(const std::string& name){
            return {
                "Cannot open file " + name,
                "This might have caused because of permissions or file name problems.",
                "Please report this to GitHub (https://github.com/dodoman8067/mcsm) if you think this is a software issue."
            };
        }

        inline std::vector<std::string> fileNotFound(const std::string& name){
            return {
                "File " + name + " cannot be found.",
                "Please report this to GitHub (https://github.com/dodoman8067/mcsm) if you think this is a software issue."
            };
        }

        inline std::vector<std::string> curlInitFailed(){
            return {
                "Unable to initialize curl",
                "Please try re-running the program, reboot the PC or reinstall the program.",
                "If none of it isn't working for you, please open a new issue in GitHub (https://github.com/dodoman8067/mcsm)."
            };
        }

        inline std::vector<std::string> getRequestFailed(const std::string& url, const std::string& reason){
            return {
                "Failed to perform GET request in the following url : " + url + " with the following reason : " + reason
            };
        }

        inline std::vector<std::string> downloadRequestFailed(const std::string& url, const std::string& reason){
            return {
                "Failed to download the file in the following url : " + url + " with the following reason : " + reason
            };
        }

        inline std::vector<std::string> downloadTargetIsNotText(const std::string& url, const std::string reason){
            return {
                "Failed to check if the following url : " + url + " returns a text with the following reason : " + reason
            };
        }

        inline std::vector<std::string> platformNotSupported(){
            return {
                "Current operating system is not supported.",
                "Please report this to GitHub (https://github.com/dodoman8067/mcsm) if you think this is a software issue."
            };
        }
    }

    std::pair<mcsm::ResultType, std::vector<std::string>> getLastResult();
    void updateLastResult(const std::pair<mcsm::ResultType, std::vector<std::string>> newRes);
    void printResultMessage();
    void printResultMessage(const std::pair<mcsm::ResultType, std::vector<std::string>> res);
}


#endif // __MCSM_RESULT_H__