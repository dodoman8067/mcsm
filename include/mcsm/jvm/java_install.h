#ifndef __MCSM_JAVA_INSTALL_H__
#define __MCSM_JAVA_INSTALL_H__

#include <mcsm/util/cli/cli_utils.h>

namespace mcsm {

    struct JavaInstall {
        JavaInstall(const std::string& path) : path(path){}
        std::string path;
    };
    using JavaInstallPtr = std::shared_ptr<mcsm::JavaInstall>;
}

#endif // __MCSM_JAVA_INSTALL_H__