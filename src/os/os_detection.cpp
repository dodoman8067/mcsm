#include "os_detection.h"

mcsm::OS mcsm::getCurrentOS() {
#ifdef _WIN32
    return mcsm::OS::WINDOWS;
#elif __linux__
    return mcsm::OS::LINUX; // This includes WSL
#elif __APPLE__
    return mcsm::OS::MAC;
#else
    return nullptr;
#endif
}