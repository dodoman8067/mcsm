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
#include <string>
#include <tl/expected.hpp>

namespace mcsm {
    void info(const std::string &message);
    void success(const std::string &message);
    void warning(const std::string &message);
    void error(const std::string &message); 

    enum class ResultType{
        MCSM_SUCCESS,
        MCSM_OK,
        MCSM_WARN,
        MCSM_WARN_NOEXIT,
        MCSM_FAIL,
        MCSM_UNKNOWN
    };

    enum class ErrorStatus {
        OK,
        WARNING,
        WARNING_NOEXIT,
        ERROR
    };

    struct ErrorTemplate {
        int code;
        std::string message;   // e.g., "File %s not found"
        std::string solution;  // e.g., "Check if the file exists"
    };

    // The full error structure
    struct Error {
        mcsm::ErrorStatus status;
        int code;
        std::string message;              // Template string
        std::vector<std::string> params;  // Params to replace in %s
        std::string solution;             // Optional help text
    };

    inline mcsm::Error makeError(mcsm::ErrorStatus status, const mcsm::ErrorTemplate& tmpl, std::vector<std::string> params = {}) {
        return mcsm::Error{status, tmpl.code, tmpl.message, std::move(params), tmpl.solution};
    }

    inline std::string formatError(const mcsm::Error& err){
        std::string formatted;
        size_t pos = 0;
        size_t paramIndex = 0;
        formatted = err.message;

        while((pos = formatted.find("%s")) != std::string::npos && paramIndex < err.params.size()){
            formatted.replace(pos, 2, err.params[paramIndex++]);
        }

        if(paramIndex < err.params.size()){
            formatted += " [extra: ";
            for(size_t i = paramIndex; i < err.params.size(); i++){
                formatted += err.params[i];
                if(i < err.params.size() - 1) formatted += ", ";
            }
            formatted += "]";
        }

        return formatted;
    }

    inline void printError(const mcsm::Error& err) {
        if(err.status == mcsm::ErrorStatus::ERROR){
            mcsm::error(formatError(err));
        }
        if(err.status == mcsm::ErrorStatus::WARNING || err.status == mcsm::ErrorStatus::WARNING_NOEXIT){
            mcsm::warning(formatError(err));
        }
        if(err.status == mcsm::ErrorStatus::OK){
            mcsm::info(formatError(err));
        }

        if(!err.solution.empty()){
            if(err.status == mcsm::ErrorStatus::ERROR){
                mcsm::error(err.solution);
            }
            if(err.status == mcsm::ErrorStatus::WARNING || err.status == mcsm::ErrorStatus::WARNING_NOEXIT){
                mcsm::warning(err.solution);
            }
            if(err.status == mcsm::ErrorStatus::OK){
                mcsm::info(err.solution);
            }
        }
    }

    inline void exitIfFail(const mcsm::Error& err) {
        if(err.status != mcsm::ErrorStatus::OK && err.status != mcsm::ErrorStatus::WARNING_NOEXIT){
            std::exit(1);
        }
    }

    // === RESULT ALIASES ===
    //template<typename T>
    //using Result = std::expected<T, Error>;

    using VoidResult = tl::expected<void, mcsm::Error>;
    using StringResult = tl::expected<std::string, mcsm::Error>;
    using BoolResult = tl::expected<bool, mcsm::Error>;
    using IntResult = tl::expected<int, mcsm::Error>;

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
        void printMessage() const;
        void printMessage(const mcsm::ResultType& type) const;
    };

    namespace message_utils {
        inline std::vector<std::string> jsonWrongType(const std::string& key, const std::string& type){
            return {
                "Value " + key + " has to be a(n) " + type + ", but it's not.",
                "Manually editing a file generated by the program might have caused this issue.",
                "If you know what you're doing, I believe that you know how to handle this issue.",
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
                "Specified server path's server option wasn't for " + serverName + " servers.",
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

        inline std::vector<std::string> downloadTargetIsNotText(const std::string& url, const std::string& reason){
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

        inline std::vector<std::string> unsafeString(const std::string& str){
            return {
                str + " contains characters that is not allowed. (&, |, \\, .., / \", \')",
                "Please resolve this issue before trying again."
            };
        }
    }

    std::pair<mcsm::ResultType, std::vector<std::string>> getLastResult();
    void updateLastResult(const std::pair<mcsm::ResultType, std::vector<std::string>> newRes);
    void printResultMessage();
    void printResultMessage(const std::pair<mcsm::ResultType, std::vector<std::string>> res);
}

namespace mcsm::errors {

    // ================================
    // 10X: File and OS Errors
    // ================================
    inline const ErrorTemplate FILE_NOT_FOUND = {
        100,
        "File %s cannot be found.",
        "Make sure the file exists at the specified path."
    };

    inline const ErrorTemplate FILE_OPEN_FAILED = {
        101,
        "Failed to open file %s.",
        "Check if the file exists and has proper permissions."
    };

    inline const ErrorTemplate FILE_EXIST_CHECK_FAILED = {
        102,
        "Checking if file/directory %s operation failed with reason: %s",
        "Check if the file exists and has proper permissions."
    };

    inline const ErrorTemplate FILE_CREATE_FAILED = {
        103,
        "Cannot create file %s.",
        "This may be due to permissions or invalid file name."
    };

    inline const ErrorTemplate FILE_SAVE_FAILED = {
        104,
        "Failed to save file %s.",
        "Check file permissions or if the file path is valid."
    };

    inline const ErrorTemplate FILE_REMOVE_FAILED = {
        105,
        "Failed to remove file %s for reason: %s",
        "Check file permissions or if the file path is valid."
    };

    inline const ErrorTemplate UNSUPPORTED_OS = {
        106,
        "The current OS is not supported.",
        "We recommend to use Windows or Linux."
    };

    // ================================
    // 20X: JSON Errors
    // ================================
    inline const ErrorTemplate JSON_WRONG_TYPE = {
        200,
        "Value %s has to be a(n) %s, but it's not.",
        "Manually editing the file might have caused this issue."
    };

    inline const ErrorTemplate JSON_WRONG_TYPE_PLUS_FIX = {
        201,
        "Value %s option in %s must be a(n) %s.\n%s",
        ""
    };

    inline const ErrorTemplate JSON_NOT_FOUND = {
        202,
        "No %s value found in %s.",
        "Make sure the field is defined in the config file."
    };

    inline const ErrorTemplate JSON_NOT_FOUND_PLUS_FIX = {
        203,
        "No %s value found in %s.\n%s",
        ""
    };

    inline const ErrorTemplate JSON_PARSE_FAILED = {
        204,
        "Failed to parse json %s.",
        "Check for comments, unclosed brackets, trailing commas, or incorrect escape sequences."
    };

    inline const ErrorTemplate JSON_PARSE_FAILED_CANNOT_BE_MODIFIED = {
        205,
        "Failed to parse json. Likely an invalid JSON responce was recieved from curl requests.",
        "This error cannot be resolved by editing the file."
    };

    // ================================
    // 30X: HTTP Errors
    // ================================
    inline const ErrorTemplate CURL_INIT_FAILED = {
        300,
        "Unable to initialize curl.",
        "Try re-running the program, reboot your machine, or reinstall."
    };

    inline const ErrorTemplate GET_REQUEST_FAILED = {
        301,
        "Failed to perform GET request for %s with reason: %s.",
        "Check your internet connection or try again later."
    };

    inline const ErrorTemplate DOWNLOAD_REQUEST_FAILED = {
        302,
        "Failed to download the file from %s with reason: %s.",
        "Check your internet connection or verify the URL."
    };

    inline const ErrorTemplate DOWNLOAD_TARGET_NOT_TEXT = {
        303,
        "Failed to verify if URL %s returns a text, reason: %s.",
        "Ensure the target is a valid text-based resource."
    };

    // ================================
    // 40X: Server Config Errors
    // ================================
    inline const ErrorTemplate SERVER_ALREADY_CONFIGURED = {
        400,
        "Server is already configured in %s.",
        "Please try again in another directory."
    };

    inline const ErrorTemplate SERVER_NOT_CONFIGURED = {
        401,
        "Server isn't configured in %s.",
        "Run \"mcsm init\" to configure a server."
    };

    inline const ErrorTemplate SERVER_DATA_ALREADY_CONFIGURED = {
        402,
        "Server data config already exists in %s.",
        "Try again in a different directory."
    };

    inline const ErrorTemplate SERVER_DATA_NOT_CONFIGURED = {
        403,
        "Server data not configured in %s.",
        "Make sure .mcsm folder is intact."
    };

    inline const ErrorTemplate SERVER_DATA_ACCESSED_WITHOUT_LOAD = {
        404,
        "Server data loader class has accessed its data without calling the load function.",
        "Please report this error to GitHub. (https://github.com/dodoman8067/mcsm)"
    };

    inline const ErrorTemplate SERVER_WRONG_INSTANCE_GENERATED = {
        405,
        "Generated server path is not a valid instance for %s servers.",
        "Check your server setup and try again."
    };

    inline const ErrorTemplate SERVER_UNSUPPORTED_VERSION = {
        406,
        "Unsupported version detected: %s.",
        "Please try again with a valid version."
    };

    inline const ErrorTemplate SERVER_DEFAULT_PROFILE_NOT_FOUND = {
        407,
        "No default launch profile found in file %s.",
        "Ensure the profile is properly defined."
    };

    inline const ErrorTemplate SERVER_DEFAULT_PROFILE_NAME_NOT_FOUND = {
        408,
        "No default profile name specified in %s.",
        "Please verify the profile name in the file."
    };

    inline const ErrorTemplate SERVER_UNSUPPORTED_VERSION_CUSTOM = {
        409,
        "Unsupported version: %s.",
        "Try a different version or check the build."
    };

    inline const ErrorTemplate JVM_PROFILE_NOT_FOUND = {
        410,
        "JVM profile %s does not exist.",
        "Check the profile definition or create a new one."
    };

    inline const ErrorTemplate JVM_DETECTION_FAILED = {
        411,
        "JVM detection failed.",
        "Set the JAVA_HOME environment variable and try again."
    };

    inline const ErrorTemplate SERVER_GROUP_MODE_INVALID = {
        412,
        "Invalid value of server group mode \"%s\"",
        ""
    };

    // ================================
    // Misc Errors
    // ================================
    inline const ErrorTemplate PLATFORM_NOT_SUPPORTED = {
        500,
        "Operating system is not supported.",
        "Please report this issue on GitHub."
    };

    inline const ErrorTemplate UNSAFE_STRING = {
        501,
        "String contains illegal() characters: %s.",
        "Please remove them and try again."
    };

    inline const ErrorTemplate GIT_CLONE_FAILED = {
        502,
        "Failed to clone a git repository %s with the following reason: %s",
        ""
    };

    inline const ErrorTemplate CURRENT_PATH_UNCATCHABLE = {
        503,
        "Getting current path operation failed with the following reason: %s",
        ""
    };

    inline const ErrorTemplate INTERNAL_FUNC_EXECUTION_FAILED = {
        504,
        "Executing function %s failed with the following reason: %s",
        ""
    };

    inline const ErrorTemplate ILLEGAL_PARAMETER = {
        505,
        "An illegal parameter value has been passed (likely nullptr). This message will likely be replaced by a more detailed description depending on the cause.",
        "Please report this to GitHub. (https://github.com/dodoman8067/mcsm)"
    };

    // ================================
    // Screen Session Errors
    // ================================
    inline const ErrorTemplate SCREEN_SESSION = {
        600,
        "Screen task performation failed with the following reason: %s",
        ""
    };
}

#endif // __MCSM_RESULT_H__