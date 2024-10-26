#ifndef __MCSM_SKIP_VERSION_CHECK_PROPERTY_H__
#define __MCSM_SKIP_VERSION_CHECK_PROPERTY_H__

#include <mcsm/data/options/general_option.h>

namespace mcsm {
    class SkipVersionCheckProperty : public mcsm::GeneralProperty {
    public:
        SkipVersionCheckProperty(const std::string& name): GeneralProperty(name){}
        ~SkipVersionCheckProperty() = default;

        // virtual function; means constexpr is impossible
        inline nlohmann::json getDefaultValue() override {
            return false;
        }
    };
}

#endif // __MCSM_SKIP_VERSION_CHECK_PROPERTY_H__