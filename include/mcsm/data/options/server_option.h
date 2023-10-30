#ifndef __MCSM_SERVER_OPTION_H__
#define __MCSM_SERVER_OPTION_H__

#include <mcsm/data/option.h>
#include <mcsm/server/server.h>
#include <memory>

namespace mcsm {
    class ServerOption {
    private:
        std::unique_ptr<mcsm::Option> option;

    public:
        ServerOption(const std::string& name);
        ~ServerOption();
    };
}

#endif // __MCSM_SERVER_OPTION_H__