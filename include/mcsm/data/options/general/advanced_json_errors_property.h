#ifndef __MCSM_ADVANCED_JSON_ERRORS_PROPERTY_H__
#define __MCSM_ADVANCED_JSON_ERRORS_PROPERTY_H__

#include <mcsm/data/options/general_option.h>

namespace mcsm {
    class AdvancedJsonErrorsProperty : public mcsm::GeneralProperty {
    public:
        AdvancedJsonErrorsProperty(const std::string& name): GeneralProperty(name){}
        ~AdvancedJsonErrorsProperty() = default;
        
        // virtual function; means constexpr is impossible
        inline nlohmann::json getDefaultValue() override {
            return false;
        }
    };
}

#endif // __MCSM_ADVANCED_JSON_ERRORS_PROPERTY_H__