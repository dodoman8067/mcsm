#ifndef __MCSM_CONFIGURABLE_H__
#define __MCSM_CONFIGURABLE_H__

namespace mcsm {
    class Configurable {
    public:
        virtual bool exists() const = 0; 
    };
}

#endif