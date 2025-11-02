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
        auto env = mcsm::getEnvStr("PATH");
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
            javas.insert((prefix / "jre" / "bin" / "java").string());
            javas.insert((prefix / "bin" / "java").string());
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

    auto asdfDataDir = mcsm::getEnvStr("ASDF_DATA_DIR");
    if(mcsm::isWhitespaceOrEmpty(asdfDataDir)) asdfDataDir = mcsm::joinPath(home, ".asdf");

    scanJavaDirs(mcsm::joinPath(home, ".gradle/jdks"));
    //javas.insert(getMinnecraftJava());
    for(auto& s : addJavasFromEnv()){
        javas.insert(s);
    }
    return javas;
}
#endif