#include "os_detection.h"

mcsm::OS mcsm::getCurrentOS() {
#if TARGET_OS == WINDOWS
    return mcsm::OS::WINDOWS;
#elif TARGET_OS == UNIX
    return mcsm::OS::UNIX;
#elif TARGET_OS == LINUX
    return mcsm::OS::LINUX;
#else
    return nullptr;
#endif
}