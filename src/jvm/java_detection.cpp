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

#include <mcsm/jvm/java_detection.h>

#ifdef _WIN32
    const std::string javaExecutable = "java.exe";
#else
    const std::string javaExecutable = "java";
#endif

std::string mcsm::getJavaFromHome(){
    const char* path = std::getenv("JAVA_HOME");
    if(path == nullptr || path == NULL) return "";
    if(mcsm::isWhitespaceOrEmpty(path)){
        return "";
    }
    std::string strPath = path;
    if(!std::filesystem::exists(strPath + "/bin")) return "";
    if(std::filesystem::exists(strPath + "/bin/" + javaExecutable)) return strPath + "/bin/" + javaExecutable;
    return "";
}

std::string mcsm::getJavaFromPath(){
    char* path = std::getenv("PATH");
    if (path != nullptr) {
        if(mcsm::getCurrentOS() == mcsm::OS::WINDOWS){
            std::string pathStr(path);

            size_t found = pathStr.find_first_of(';');
            while(found != std::string::npos){
                std::string directory = pathStr.substr(0, found);
                std::filesystem::path javaPath = std::filesystem::path(directory) / javaExecutable;
                
                std::error_code ec;
                bool exists = std::filesystem::exists(javaPath, ec);
                if(ec){
                    mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
                        "Checking if directory " + javaPath.string() + "operation failed : " + ec.message(), 
                        "If you believe that this is a software issue, please report this to GitHub (https://github.com/dodoman8067/mcsm)."
                    }});
                    return "";
                }

                if(exists){
                    mcsm::Result res({mcsm::ResultType::MCSM_OK, {"Success"}});
                    return javaPath.string();
                }

                pathStr = pathStr.substr(found + 1);
                found = pathStr.find_first_of(';');
            }
        }
        if(mcsm::getCurrentOS() == mcsm::OS::LINUX){
            std::string pathStr(path);

            size_t found = pathStr.find_first_of(':');
            while(found != std::string::npos){
                std::string directory = pathStr.substr(0, found);
                std::filesystem::path javaPath = std::filesystem::path(directory) / javaExecutable;

                std::error_code ec;
                bool exists = std::filesystem::exists(javaPath, ec);
                if(ec){
                    mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
                        "Checking if directory " + javaPath.string() + "operation failed : " + ec.message(), 
                        "If you believe that this is a software issue, please report this to GitHub (https://github.com/dodoman8067/mcsm)."
                    }});
                    return "";
                }

                if(exists){
                    mcsm::Result res({mcsm::ResultType::MCSM_OK, {"Success"}});
                    return javaPath.string();
                }

                pathStr = pathStr.substr(found + 1);
                found = pathStr.find_first_of(':');
            }
        }
    }
    mcsm::Result res({mcsm::ResultType::MCSM_OK, {"Success"}});
    return "";
}

std::string mcsm::detectJava(){
    mcsm::info("Detecting java..");
    std::string str = getJavaFromHome();
    if(!mcsm::isWhitespaceOrEmpty(str)){
        mcsm::replaceAll(str, "\\", "/");
        if(!isValidJava(str)){
            mcsm::Result res({mcsm::ResultType::MCSM_WARN, {"Detected java was not a valid java."}});
            return "";
        }
        mcsm::info("Detected java from JAVA_HOME : " + str);
        if(!mcsm::startsWith(str, "\"")){
            str = "\"" + str;
        }
        if(!mcsm::endsWith(str, "\"")){
            str = str + "\"";
        }
        mcsm::Result res({mcsm::ResultType::MCSM_OK, {"Success"}});
        return str;
    }
    mcsm::info("Failed to detect java from JAVA_HOME; Looking from PATH.");
    std::string pathStr = getJavaFromPath();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return "";
    
    if(!mcsm::isWhitespaceOrEmpty(pathStr)){
        mcsm::replaceAll(pathStr, "\\", "/");
        if(!isValidJava(pathStr)){
            mcsm::Result res({mcsm::ResultType::MCSM_WARN, {"Detected java was not a valid java."}});
            return "";
        }
        mcsm::info("Detected java from PATH : " + pathStr + ".");
        if(!mcsm::startsWith(pathStr, "\"")){
            pathStr = "\"" + pathStr;
        }
        if(!mcsm::endsWith(pathStr, "\"")){
            pathStr = pathStr + "\"";
        }
        mcsm::Result res({mcsm::ResultType::MCSM_OK, {"Success"}});
        return pathStr;
    }

    mcsm::Result res({mcsm::ResultType::MCSM_WARN, {
        "Failed to detect java from PATH.",
        "But you may specify the jvm path manually."
    }});
    return "";
}

bool mcsm::isValidJava(const std::string& path){
    std::string command;
    if (!mcsm::startsWith(command, "\"")) {
        command = "\"" + command;
    }
    if (!mcsm::endsWith(command, "\"")) {
        command += "\"";
    }
    if(mcsm::getCurrentOS() == mcsm::OS::WINDOWS){
        command = "\""+ path + "\"" + " -version > NUL 2>&1";
    }else if(mcsm::getCurrentOS() == mcsm::OS::LINUX){
        command = path + " -version > /dev/null 2>&1";
    }
    
    int code = mcsm::runCommand(command.c_str());
    mcsm::Result res({mcsm::ResultType::MCSM_OK, {"Success"}});
    return code == 0;
}