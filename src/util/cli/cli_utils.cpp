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

#include <mcsm/util/cli/cli_utils.h>

mcsm::IntResult mcsm::runCommandQuietly(const std::string& command){
    if(mcsm::getCurrentOS() == mcsm::OS::WINDOWS){
        std::string cmd = command + " > NUL 2>&1";
        return std::system(cmd.c_str());
    }else if(mcsm::getCurrentOS() == mcsm::OS::LINUX){
        std::string cmd = command + " > /dev/null 2>&1";
        #ifdef __linux__
            int code = std::system(cmd.c_str());
            if(WIFEXITED(code)){
                int finalCode = WEXITSTATUS(code);
                return finalCode;
            }else{
                return 1;
            }
        #else
            return 1;
        #endif
    }else{
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::UNSUPPORTED_OS, {});
        return tl::unexpected(err);
    }
}

mcsm::IntResult mcsm::runCommand(const std::string& command){
    if(mcsm::getCurrentOS() == mcsm::OS::WINDOWS){
        return std::system(command.c_str());
    }else if(mcsm::getCurrentOS() == mcsm::OS::LINUX){
        #ifdef __linux__
            int code = std::system(command.c_str());
            if(WIFEXITED(code)){
                int finalCode = WEXITSTATUS(code);
                return finalCode;
            }else{
                return 1;
            }
        #else
            return 1;
        #endif
    }else{
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::UNSUPPORTED_OS, {});
        return tl::unexpected(err);
    }
}

mcsm::StringResult mcsm::getCurrentPath(){
    std::error_code ec;
    std::string path = std::filesystem::current_path(ec).string();
    if(ec){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::CURRENT_PATH_UNCATCHABLE, {ec.message()});
        return tl::unexpected(err);
    }
    return path;
}

std::string mcsm::getDataPathPerOS(){
    std::string returnPath;
    switch (mcsm::getCurrentOS()){
        case mcsm::OS::WINDOWS: {
            std::string windowsPath = getenv("USERPROFILE");
            windowsPath.append("/AppData/Local/mcsm");
            mcsm::replaceAll(windowsPath, "\\", "/");
            returnPath = std::move(windowsPath);
            break;
        }

        case mcsm::OS::LINUX: {
            std::string linuxPath = getenv("HOME");
            linuxPath.append("/.local/share/mcsm");
            returnPath = std::move(linuxPath);
            break;
        }
    }
    return returnPath;
}

std::string mcsm::asGlobalConfigPath(const std::string& value){
    return mcsm::normalizePath(getDataPathPerOS() + value);
}

mcsm::BoolResult mcsm::fileExists(const std::string& path){
    std::error_code ec;
    bool exists = std::filesystem::exists(path, ec);
    if(ec){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::FILE_EXIST_CHECK_FAILED, {path, ec.message()});
        return tl::unexpected(err);
    }
    return exists;
}

mcsm::BoolResult mcsm::removeFile(const std::string& path){
    if(!fileExists(path)) return false;
    std::error_code ec;
    bool success = std::filesystem::remove(path, ec);
    if(ec){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::FILE_EXIST_CHECK_FAILED, {path, ec.message()});
        return tl::unexpected(err);
    }
    return success; 
}

bool mcsm::mkdir(const std::string& dirName){
    std::error_code ec;
    ec.clear();
    if(!std::filesystem::create_directories(dirName, ec)){
        if(std::filesystem::exists(dirName)){
            ec.clear();
            return true;
        }
        return false;
    }
    return true;
}

/*
Example usage:

    auto beforeInput = []() {
        std::cout << "Enter a valid input: ";
    };

    auto invalidInput = [](const std::string& input) {
        std::cout << "Invalid input " << input << ". Please try again." << std::endl;
    };

    auto isValid = [](const std::string& input) -> bool {
        // Example validation: input must not be empty and must have more than 3 characters
        return !input.empty() && input.length() > 3;
    };

    std::string userInput;
    mcsm::getInput(userInput, beforeInput, invalidInput, isValid);

    std::cout << "You entered a valid input: " << userInput << std::endl;
*/

void mcsm::getInput(std::string& input, std::function<void(std::vector<std::string>)> beforeInputFunction, std::function<void(std::vector<std::string>)> invalidInputFunction, std::function<bool(std::vector<std::string>)> checkValid) {
    std::string typedInput;
    while(true){
        beforeInputFunction({});
        std::getline(std::cin, typedInput);
            
        if(checkValid({typedInput})){
            input = typedInput;
            break;
        }else{
            invalidInputFunction({typedInput});
        }
    }
}

bool mcsm::isDebug(){
    #ifdef MCSM_DEBUG
        return true;
    #else
        return false;
    #endif
}

mcsm::StringResult mcsm::getExecutablePath(){
#ifdef __linux__
    char buf[PATH_MAX];
    ssize_t len = readlink("/proc/self/exe", buf, sizeof(buf) - 1);

    if(len == -1){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::INTERNAL_FUNC_EXECUTION_FAILED, {"readlink", "unknown"});
        return tl::unexpected(err);
    }

    buf[len] = '\0';
    return "\"" + std::string(buf) + "\"";
#elif defined(_WIN32)
    char buf[MAX_PATH];
    DWORD len = GetModuleFileNameA(NULL, buf, MAX_PATH);

    if(len == 0 || len == MAX_PATH){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::INTERNAL_FUNC_EXECUTION_FAILED, {"GetModuleFileName", "unknown"});
        return tl::unexpected(err);
    }

    return "\"" + std::string(buf) + "\"";
#else
    mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::UNSUPPORTED_OS, {});
    return tl::unexpected(err);
#endif
}