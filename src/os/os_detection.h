#ifndef __MCSM_OS_DETECTION_H__
#define __MCSM_OS_DETECTION_H__

namespace mcsm {
    enum OS {
        WINDOWS,
        MAC,
        LINUX
    };
    OS getCurrentOS();
}

#endif