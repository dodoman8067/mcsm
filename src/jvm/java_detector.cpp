#include <mcsm/jvm/java_detector.h>

#ifdef _WIN32
    const std::string javaExecutable = "java.exe";
#else
    const std::string javaExecutable = "java";
#endif

std::set<std::string> addJavasFromEnv(){
    auto env = mcsm::getEnvStr("MCSM_JAVA_PATHS");
    std::set<std::string> entries;
    
    std::string mcsmPathStr(env);
    mcsm::replaceAll(mcsmPathStr, "\\", "/");

    size_t found = mcsmPathStr.find_first_of(';');
    while(found != std::string::npos){
        std::string directory = mcsmPathStr.substr(0, found);
        std::filesystem::path javaPath = std::filesystem::path(directory) / javaExecutable;
            
        std::error_code ec;
        bool exists = std::filesystem::exists(javaPath, ec);
        if(ec){
            continue;
        }
        if(exists){
            entries.insert({javaPath.string()});
        }
        mcsmPathStr = mcsmPathStr.substr(found + 1);
        found = mcsmPathStr.find_first_of(';');
    }

    #ifdef _WIN32
        auto env1 = mcsm::getEnvStr("PATH");

        std::string mcsmPathStr1(env1);
        mcsm::replaceAll(mcsmPathStr1, "\\", "/");

        size_t found1 = mcsmPathStr1.find_first_of(';');
        while(found1 != std::string::npos){
            std::string directory1 = mcsmPathStr1.substr(0, found1);
            std::filesystem::path javaPath1 = std::filesystem::path(directory1) / javaExecutable;

            std::error_code ec;
            bool exists = std::filesystem::exists(javaPath1, ec);
            if(ec){
                continue;
            }
            if(exists){
                entries.insert({javaPath1.string()});
            }
            mcsmPathStr1 = mcsmPathStr1.substr(found1 + 1);
            found1 = mcsmPathStr1.find_first_of(';');
        }
    #endif
    return entries;
}

#if defined(__linux__) || defined(__FreeBSD__)
std::set<std::string> mcsm::findJavaPaths(){
    std::set<std::string> javas;
    auto scanJavaDir = [&javas](
        const std::string& dirPath,
        const std::function<bool(const std::filesystem::directory_entry&)>& filter =
            [](const std::filesystem::directory_entry&) { return true; }) 
    {
        std::filesystem::path dir(dirPath);
        if(!std::filesystem::exists(dir) || !std::filesystem::is_directory(dir))
            return;

        for(const auto& entry : std::filesystem::directory_iterator(dir)){
            if (!entry.is_directory())
                continue;
            if (!filter(entry))
                continue;

            std::filesystem::path prefix = std::filesystem::canonical(entry.path());
            javas.insert((prefix / "java").string());
            javas.insert((prefix / "jre" / "bin" / "java").string());
            javas.insert((prefix / "bin" / "java").string());
            std::cout << prefix << " (DEBUG)\n";
        }
    };

    auto snap = mcsm::getEnvStr("SNAP");

    auto scanJavaDirs = [scanJavaDir, snap](const std::string& dirPath) {
        scanJavaDir(dirPath);
        if(!snap.empty()){
            scanJavaDir(snap + dirPath);
        }
    };
#ifdef __linux__
    scanJavaDirs("/usr/java");
    // general locations used by distro packaging
    scanJavaDirs("/usr/lib/jvm");
    scanJavaDirs("/usr/lib64/jvm");
    scanJavaDirs("/usr/lib32/jvm");
    auto gentooFilter = [](const std::filesystem::directory_entry& e) {
        const std::string name = e.path().filename().string();
        // starts_with (C++20) alternative using rfind at pos 0 for C++17 compatibility
        return (name.rfind("openjdk-", 0) == 0) || (name.rfind("openj9-", 0) == 0);
    };

    auto aoscFilter = [](const std::filesystem::directory_entry& e) {
        const std::string name = e.path().filename().string();
        return name == "java" || (name.rfind("java-", 0) == 0);
    };
    scanJavaDir("/usr/lib64", gentooFilter);
    scanJavaDir("/usr/lib", gentooFilter);
    scanJavaDir("/opt", gentooFilter);
    scanJavaDir("/usr/lib", aoscFilter);
    // javas stored in Prism Launcher's folder
    scanJavaDirs("java");
    // manually installed JDKs in /opt
    scanJavaDirs("/opt/jdk");
    scanJavaDirs("/opt/jdks");
    scanJavaDirs("/opt/ibm");  // IBM Semeru Certified Edition
    // flatpak
    scanJavaDirs("/app/jdk");
#elif defined(__FreeBSD__)
    scanJavaDirs("/usr/local");
#endif
    auto home = mcsm::getEnvStr("HOME");

    scanJavaDirs(mcsm::joinPath(home, ".jdks"));

    auto sdkmanDir = mcsm::getEnvStr("SDKMAN_DIR");
    if(mcsm::isWhitespaceOrEmpty(sdkmanDir)) sdkmanDir = mcsm::joinPath(home, ".sdkman");

    scanJavaDirs(mcsm::joinPath(sdkmanDir, "candidates/java"));

    auto asdfDataDir = mcsm::getEnvStr("ASDF_DATA_DIR");
    if(mcsm::isWhitespaceOrEmpty(asdfDataDir)) asdfDataDir = mcsm::joinPath(home, ".asdf");
    
    scanJavaDirs(mcsm::joinPath(asdfDataDir, "installs/java"));

    scanJavaDirs(mcsm::joinPath(home, ".gradle/jdks"));
    //javas.insert(getMinnecraftJava());
    for(auto& s : addJavasFromEnv()){
        javas.insert(s);
    }
    return javas;
}
#elif defined(__APPLE__)
std::set<std::string> mcsm::findJavaPaths(){
    std::set<std::string> javas;
    javas.insert({"java"});
    javas.insert({"/Applications/Xcode.app/Contents/Applications/Application Loader.app/Contents/MacOS/itms/java/bin/java"});
    javas.insert({"/Library/Internet Plug-Ins/JavaAppletPlugin.plugin/Contents/Home/bin/java"});
    javas.insert({"/System/Library/Frameworks/JavaVM.framework/Versions/Current/Commands/java"});
    std::filesystem::path libJvmDir("/Library/Java/JavaVirtualMachines/");
    if(std::filesystem::exists(libJvmDir) && std::filesystem::is_directory(libJvmDir)){
        for(const auto& entry : std::filesystem::directory_iterator(libJvmDir)){
            if(!entry.is_directory())
                continue;
            std::filesystem::path prefix = std::filesystem::canonical(entry.path());
            auto finalPath = prefix / "Contents" / "Home" / "bin" / "java";
            auto finalPath1 = prefix / "Contents" / "Home" / "jre" / "bin" / "java";
            javas.insert(mcsm::normalizePath(finalPath.string()));
            javas.insert(mcsm::normalizePath(finalPath1.string()));
        }
    }

    std::filesystem::path sysLibJvmDir("/System/Library/Java/JavaVirtualMachines/");
    if(std::filesystem::exists(sysLibJvmDir) && std::filesystem::is_directory(sysLibJvmDir)){
        for(const auto& entry : std::filesystem::directory_iterator(sysLibJvmDir)){
            if(!entry.is_directory())
                continue;
            std::filesystem::path prefix = std::filesystem::canonical(entry.path());
            auto finalPath = prefix / "Contents" / "Home" / "bin" / "java";
            javas.insert(mcsm::normalizePath(finalPath.string()));
            auto finalPath1 = prefix / "Contents" / "Commands" / "java";
            javas.insert(mcsm::normalizePath(finalPath1.string()));
        }
    }

    std::string home = mcsm::getEnvStr("HOME");

    std::string sdkManDir = mcsm::getEnvStr("SDKMAN_DIR");
    if(mcsm::isWhitespaceOrEmpty(sdkManDir)) sdkManDir = mcsm::joinPath(home, ".sdkman");
    std::string sdkManJavaDir = mcsm::joinPath(sdkManDir, "candidates/java");
    if(std::filesystem::exists(sdkManJavaDir) && std::filesystem::is_directory(sdkManJavaDir)){
        for(const auto& entry : std::filesystem::directory_iterator(sdkManJavaDir)){
            if(!entry.is_directory())
                continue;
            std::string toAdd = mcsm::normalizePath(entry.path().string() + "/bin/java");
            javas.insert({toAdd});
        }
    }

    std::string asdfDir = mcsm::getEnvStr("ASDF_DATA_DIR");
    if(mcsm::isWhitespaceOrEmpty(asdfDir)) asdfDir = mcsm::joinPath(home, ".asdf");
    std::string asdfJavaDir = mcsm::joinPath(asdfDir, "installs/java");
    if(std::filesystem::exists(asdfJavaDir) && std::filesystem::is_directory(asdfJavaDir)){
        for(const auto& entry : std::filesystem::directory_iterator(asdfJavaDir)){
            if(!entry.is_directory())
                continue;
            std::string toAdd = mcsm::normalizePath(entry.path().string() + "/bin/java");
            javas.insert({toAdd});
        }
    }

    std::filesystem::path homeLibJvmDir(mcsm::joinPath(home, "/Library/Java/JavaVirtualMachines/"));
    if(std::filesystem::exists(homeLibJvmDir) && std::filesystem::is_directory(homeLibJvmDir)){
        for(const auto& entry : std::filesystem::directory_iterator(homeLibJvmDir)){
            if(!entry.is_directory())
                continue;
            std::filesystem::path prefix = std::filesystem::canonical(entry.path());
            auto finalPath  = prefix / "Contents" / "Home" / "bin" / "java";
            javas.insert(mcsm::normalizePath(finalPath.string()));
            auto finalPath1 = prefix / "Contents" / "Commands" / "java";
            javas.insert(mcsm::normalizePath(finalPath1.string()));
        }
    }

    for(auto& s : addJavasFromEnv()){
        javas.insert(s);
    }
    return javas;
}
#else
std::set<std::string> mcsm::findJavaPaths(){
    std::set<std::string> javas;
    for(auto& s : addJavasFromEnv()){
        std::cout << s;
        javas.insert(s);
    }
    return javas;
}
#endif