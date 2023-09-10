#ifndef __MCSM_OS_DETECTION_H__
#define __MCSM_OS_DETECTION_H__

#if defined(_WIN32) || defined(_WIN64)
#define TARGET_OS WINDOWS
#elif defined(__unix__) || defined(__linux__)
#define TARGET_OS UNIX
#endif

namespace mcsm {
    enum OS {
        WINDOWS,
        UNIX,
        LINUX
    };
    OS getCurrentOS();
}

#endif