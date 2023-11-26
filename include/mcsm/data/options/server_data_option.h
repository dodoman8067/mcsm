#ifndef __SERVER_DATA_OPTION_H__
#define __SERVER_DATA_OPTION_H__

#include <mcsm/data/option.h>
#include <chrono>

namespace mcsm {
    class ServerDataOption {
    private:
        std::unique_ptr<mcsm::Option> option;
    public:
        ServerDataOption();
        ~ServerDataOption();
        void create(const std::string& lastTimeLaunched, const std::string& build);
        void reset();
        std::string getLastTimeLaunched() const;
        void updateLastTimeLaunched();
        std::string getCurrentBuild() const;
        void updateCurrentBuild(const std::string& build);
        bool exists() const;
    };
}

#endif // __SERVER_DATA_OPTION_H__