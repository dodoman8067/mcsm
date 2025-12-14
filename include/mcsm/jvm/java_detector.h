#ifndef __MCSM_JAVA_DETECTOR_H__
#define __MCSM_JAVA_DETECTOR_H__

#include <mcsm/jvm/java_install.h>
#include <set>

#ifdef _WIN32
    #include <windows.h>
#endif

namespace mcsm {
    inline mcsm::JavaInstallPtr makeJavaInstallPtr(const std::string& path){
        return std::make_shared<mcsm::JavaInstall>(path);
    }

    std::set<std::string> findJavaPaths();
}

#endif // __MCSM_JAVA_DETECTOR_H__