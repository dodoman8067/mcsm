#ifndef __MCSM_FABRIC_SERVER_DATA_OPTION_H__
#define __MCSM_FABRIC_SERVER_DATA_OPTION_H__

#include <mcsm/data/options/server_data_option.h>

namespace mcsm {
    class FabricServerDataOption : public mcsm::ServerDataOption {
    public:
        FabricServerDataOption();
        FabricServerDataOption(const std::string& path);
        ~FabricServerDataOption();

        mcsm::StringResult getLoaderVersion() const;
        mcsm::VoidResult updateLoaderVersion(const std::string& version);
        
        mcsm::StringResult getInstallerVersion() const;
        mcsm::VoidResult updateInstallerVersion(const std::string& version);
    };
}

#endif // __MCSM_FABRIC_SERVER_DATA_OPTION_H__