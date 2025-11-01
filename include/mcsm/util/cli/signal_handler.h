#ifndef __MCSM_SEGFAULT_HANDLER_H__
#define __MCSM_SEGFAULT_HANDLER_H__

#include <mcsm/util/cli/logging.h>
#include <mcsm/http/holder.h>
#include <mcsm/command/command_manager.h>
#include <csignal>
#if defined(__linux__) && !defined(__ANDROID__)
    #include <execinfo.h>
#endif

namespace mcsm {
    namespace signal_handler {
        void handle(int signal);
        void new_handle();
    }
}

#endif // __MCSM_SEGFAULT_HANDLER_H__