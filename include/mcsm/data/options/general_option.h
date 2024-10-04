#ifndef __MCSM_GENERAL_OPTION_H__
#define __MCSM_GENERAL_OPTION_H__

#include <mcsm/data/global_option.h>

// how it is gonna be implemented:
// save method serializes the current properties vector. add/remove method will modify the vector and call save()

namespace mcsm {
    class GeneralProperty;
    class GeneralOption {
    private:
        GeneralOption();
        ~GeneralOption();

        std::unique_ptr<mcsm::GlobalOption> optionHandle;
        std::vector<mcsm::GeneralProperty> properties;

        mcsm::Result save();
    public:
        // initializes optionHandle
        static mcsm::Result initialize();
        // returns instance of GeneralOption
        static mcsm::GeneralOption* getGeneralOption();

        std::vector<mcsm::GeneralProperty>& getProperties() const;

        mcsm::Result addProperty(mcsm::GeneralProperty& property);
        mcsm::Result removeProperty(const std::string& propertyName);

        mcsm::Result setProperty(const std::string& propertyName, nlohmann::json newValue);
    };
}
#endif // __MCSM_GENERAL_OPTION_H__