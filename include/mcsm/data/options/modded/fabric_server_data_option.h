#ifndef __MCSM_FABRIC_SERVER_DATA_OPTION_H__
#define __MCSM_FABRIC_SERVER_DATA_OPTION_H__

#include <mcsm/data/options/server_data_option.h>

namespace mcsm {
    class FabricServerDataOption : public mcsm::ServerDataOption {
    public:
        FabricServerDataOption();
        FabricServerDataOption(const std::string& path);
        ~FabricServerDataOption();

        std::string getLoaderVersion() const;
        void updateLoaderVersion(const std::string& version);
        
        std::string getInstallerVersion() const;
        void updateInstallerVersion(const std::string& version);
    };
}

#endif // __MCSM_FABRIC_SERVER_DATA_OPTION_H__