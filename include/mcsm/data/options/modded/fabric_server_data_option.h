#ifndef __MCSM_FABRIC_SERVER_DATA_OPTION_H__
#define __MCSM_FABRIC_SERVER_DATA_OPTION_H__

#include <mcsm/data/options/server_data_option.h>

namespace mcsm {
    class FabricServerDataOption : public mcsm::ServerDataOption {
    public:
        FabricServerDataOption();
        ~FabricServerDataOption();
        std::string getLoaderVersion() const;
        void setLoaderVersion(const std::string& version);
        std::string getAPIVersion() const;
        void setAPIVersion(const std::string& version);
    };
}

#endif // __MCSM_FABRIC_SERVER_DATA_OPTION_H__