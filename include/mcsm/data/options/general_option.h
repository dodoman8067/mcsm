#ifndef __MCSM_GENERAL_OPTION_H__
#define __MCSM_GENERAL_OPTION_H__

#include <mcsm/data/global_option.h>
#include <algorithm>
#include <mcsm/data/options/general_property.h>
#include <mcsm/server/server_registry.h>

// how it is gonna be implemented:
// save method serializes the current properties vector. add/remove method will modify the vector and call save()

namespace mcsm {
    class GeneralOption {
    private:
        GeneralOption() = default;

        std::unique_ptr<mcsm::GlobalOption> optionHandle;
        std::vector<mcsm::GeneralProperty*> properties;

        mcsm::Result save();
        mcsm::Result load();
    public:
        GeneralOption(const mcsm::GeneralOption&) = delete;
        GeneralOption& operator=(const mcsm::GeneralOption&) = delete;
        ~GeneralOption() = default;

        // initializes optionHandle
        static mcsm::Result initialize();
        // returns instance of GeneralOption
        static mcsm::GeneralOption& getGeneralOption();

        std::vector<mcsm::GeneralProperty*>& getProperties() const;

        mcsm::Result setProperty(const std::string& propertyName, nlohmann::json newValue);

        mcsm::GlobalOption* getOption();
    };
}
#endif // __MCSM_GENERAL_OPTION_H__