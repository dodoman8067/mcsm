#ifndef __MCSM_SCREEN_BIN_PATH_PROPERTY_H__
#define __MCSM_SCREEN_BIN_PATH_PROPERTY_H__

#include <mcsm/data/options/general_option.h>

namespace mcsm {
    class ScreenBinPathProperty : public mcsm::GeneralProperty {
    public:
        ScreenBinPathProperty(const std::string& name): GeneralProperty(name){}
        ~ScreenBinPathProperty() = default;
        
        // virtual function; means constexpr is impossible
        inline nlohmann::json getDefaultValue() override {
        #ifdef __linux__
            return "/usr/bin/screen";
        #else
            return "";
        #endif
        }
    };
}

#endif // __MCSM_SCREEN_BIN_PATH_PROPERTY_H__