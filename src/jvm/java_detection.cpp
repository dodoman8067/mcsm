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

#include "java_detection.h"


#ifdef _WIN32
    const std::string javaExecutable = "java.exe";
#else
    const std::string javaExecutable = "java";
#endif

std::string mcsm::getJavaFromHome(){
    const char* path = std::getenv("JAVA_HOME");
    if(path == nullptr || path == NULL) return std::move("");
    if(mcsm::isWhitespaceOrEmpty(path)){
        return std::move("");
    }
    std::string strPath = path;
    if(!std::filesystem::exists(strPath + "/bin")) return std::move("");
    if(std::filesystem::exists(strPath + "/bin/java")) return std::move(strPath + "/bin/java");
    if(std::filesystem::exists(strPath + "/bin/java.exe")) return std::move(strPath + "/bin/java.exe");
    return std::move("");
}

std::string mcsm::getJavaFromPath(){
    char* path = std::getenv("PATH");
    if (path != nullptr) {
        std::string pathStr(path);

        size_t found = pathStr.find_first_of(';');
        while (found != std::string::npos) {
            std::string directory = pathStr.substr(0, found);
            std::filesystem::path javaPath = std::filesystem::path(directory) / javaExecutable;
            if (std::filesystem::exists(javaPath)) {
                return std::move(javaPath.string());
            }

            pathStr = pathStr.substr(found + 1);
            found = pathStr.find_first_of(';');
        }
    }
    return std::move("");
}

std::string mcsm::detectJava(){
    std::cout << "[mcsm] Detecting java..\n";
    std::string str = getJavaFromHome();
    if(!mcsm::isWhitespaceOrEmpty(str)){
        mcsm::replaceAll(str, "\\", "/");
        std::cout << "[mcsm] Detected java from JAVA_HOME : " << str << ".\n";
        return str;
    }
    std::cout << "[mcsm] Failed to detect java from JAVA_HOME; Looking from PATH.\n";
    std::string pathStr = getJavaFromPath();
    if(!mcsm::isWhitespaceOrEmpty(pathStr)){
        mcsm::replaceAll(pathStr, "\\", "/");
        std::cout << "[mcsm] Detected java from PATH : " << pathStr << ".\n";
        return pathStr;
    }
    std::cerr << "[mcsm] Failed to detect java from PATH.\n";
    std::cerr << "[mcsm] But you may specify jvm path manually.\n";
    std::exit(1);
}