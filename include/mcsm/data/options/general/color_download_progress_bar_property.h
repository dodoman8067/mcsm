#ifndef __MCSM_COLOR_DOWNLOAD_PROGRESS_BAR_PROPERTY_H__
#define __MCSM_COLOR_DOWNLOAD_PROGRESS_BAR_PROPERTY_H__

#include <mcsm/data/options/general_option.h>

namespace mcsm {
    class ColorDownloadProgressBarProperty : public mcsm::GeneralProperty {
    public:
        ColorDownloadProgressBarProperty(const std::string& name): GeneralProperty(name){}
        ~ColorDownloadProgressBarProperty() = default;
        
        // virtual function; means constexpr is impossible
        nlohmann::json getDefaultValue() const override {
            return true;
        }
    };
}

#endif // __MCSM_COLOR_DOWNLOAD_PROGRESS_BAR_PROPERTY_H__