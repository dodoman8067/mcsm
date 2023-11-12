#ifndef __MCSM_CLI_UTILS_H__
#define __MCSM_CLI_UTILS_H__

#include <string>
#include <cstdlib>
#include <iostream>
#include <mcsm/util/os/os_detection.h>
#ifdef __linux__
    #include <sys/wait.h>
#endif

namespace mcsm {
    int runCommandQuietly(const std::string& command);
    int runCommand(const std::string& command);
}

#endif // __MCSM_CLI_UTILS_H__