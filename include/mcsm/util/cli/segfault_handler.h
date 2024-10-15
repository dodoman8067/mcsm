#ifndef __MCSM_SEGFAULT_HANDLER_H__
#define __MCSM_SEGFAULT_HANDLER_H__

#include <mcsm/util/cli/logging.h>
#include <csignal>
#ifdef __linux__
    #include <execinfo.h>
#endif

namespace mcsm {
    namespace segfault_handler {
        void handle(int signal);
    }
}

#endif // __MCSM_SEGFAULT_HANDLER_H__