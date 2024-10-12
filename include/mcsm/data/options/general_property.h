#ifndef __MCSM_GENERAL_PROPERTY_H__
#define __MCSM_GENERAL_PROPERTY_H__

#include <nlohmann/json.hpp>
#include <mcsm/util/cli/result.h>
#include <string>

namespace mcsm {
    class GeneralProperty {
    private:
        std::string name;
        nlohmann::json value;
    public:
        GeneralProperty(const std::string& name);
        ~GeneralProperty();

        std::string getName() const;

        virtual inline nlohmann::json getDefaultValue() = 0;

        nlohmann::json getCurrentValue();
        void setCurrentValue(nlohmann::json& value);
    };
}

#endif // __MCSM_GENERAL_PROPERTY_H__