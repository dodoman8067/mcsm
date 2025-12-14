#ifndef __MCSM_GENERAL_OPTION_H__
#define __MCSM_GENERAL_OPTION_H__

#include <algorithm>
#include <mcsm/data/options/general_property.h>
#include <mcsm/server/server_registry.h>

// how it is gonna be implemented:
// save method serializes the current properties vector. add/remove method will modify the vector and call save()

namespace mcsm {
    class GlobalOption;  // Forward declaration to break circular dependency

    class GeneralOption {
    private:
        GeneralOption() = default;

        std::unique_ptr<mcsm::Option> optionHandle;
        std::vector<mcsm::GeneralProperty*> properties;

        mcsm::VoidResult save();
        mcsm::VoidResult load();

        bool advp;
        bool advpb;
        std::string sbp;
    public:
        GeneralOption(const mcsm::GeneralOption&) = delete;
        GeneralOption& operator=(const mcsm::GeneralOption&) = delete;
        ~GeneralOption() = default;

        // initializes optionHandle
        static mcsm::VoidResult initialize();
        // returns instance of GeneralOption
        static mcsm::GeneralOption& getGeneralOption();

        std::vector<mcsm::GeneralProperty*>& getProperties() const;

        mcsm::GeneralProperty* getProperty(const std::string& propertyName) const;
        mcsm::VoidResult setProperty(const std::string& propertyName, nlohmann::json newValue);

        mcsm::Option* getOption();

        bool advancedParseEnabled() const;
        bool colorDownloadProgressBar() const;
        std::string screenBinPathProperty() const;
    };
}

#endif // __MCSM_GENERAL_OPTION_H__