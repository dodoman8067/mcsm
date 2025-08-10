#ifndef __MCSM_SPONGE_API_SEARCH_RECOMMENDED_VERSIONS_PROPERTY_H__
#define __MCSM_SPONGE_API_SEARCH_RECOMMENDED_VERSIONS_PROPERTY_H__

#include <mcsm/data/options/general_option.h>

namespace mcsm {
    class SpongeAPISearchRecommendedVersionsProperty : public mcsm::GeneralProperty {
    public:
        SpongeAPISearchRecommendedVersionsProperty(const std::string& name): GeneralProperty(name){}
        ~SpongeAPISearchRecommendedVersionsProperty() = default;

        // virtual function; means constexpr is impossible
        // this will set the default sponge value while configuring
        inline nlohmann::json getDefaultValue() const override {
            return false;
        }
    };
}

#endif // __MCSM_SPONGE_API_SEARCH_RECOMMENDED_VERSIONS_PROPERTY_H__