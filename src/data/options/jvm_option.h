#ifndef __MCSM_JVM_OPTION_H__
#define __MCSM_JVM_OPTION_H__

#include "../global_option.h"

namespace mcsm {
    class JvmOption {
    private:
        mcsm::GlobalOption option;
    public:
        JvmOption();
        ~JvmOption();
        void init();
        void getJvmPath();
    };
}

#endif