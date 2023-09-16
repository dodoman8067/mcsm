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

bool mcsm::getJava(){
    int code = std::system("java -version");
    return code == 0;
}

std::string mcsm::getJavaFromPath(){
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


std::string mcsm::detectJava(){
    std::cout << "[mcsm] Detecting java..\n";
    if(getJava()){
        std::cout << "[mcsm] Detected java from command.\n";
        return std::move("java");
    }else{
        std::cout << "[mcsm] Couldn't detect java from command; Looking from JAVA_HOME.\n";
        std::string str = getJavaFromPath();
        if(mcsm::isWhitespaceOrEmpty(str)){
            std::cerr << "[mcsm] JAVA_HOME is not set; Please set an enviroment variable for JAVA_HOME.\n";
            std::cerr << "[mcsm] Or you may set the JVM path manually.\n";
            std::exit(1);
        }else{
            mcsm::replaceAll(str, "\\", "/");
            std::cout << "[mcsm] Detected java from JAVA_HOME : " << str << ".\n";
            return str;
        }
    }
}