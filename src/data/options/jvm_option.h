#ifndef __MCSM_JVM_OPTION_H__
#define __MCSM_JVM_OPTION_H__

#include "../global_option.h"
#include "../option.h"
#include "../../jvm/java_detection.h"
#include "../../util/string_utils.h"
#include <memory>

namespace mcsm {
    enum SearchTarget {
        GLOBAL,
        CURRENT,
        ALL
    };

    class JvmOption {
    private:
        std::unique_ptr<mcsm::Configurable> option;
        std::string name;
    public:
        JvmOption(const std::string& name);
        JvmOption(const std::string& name, const SearchTarget& target);
        ~JvmOption();
        void create();
        void create(const std::string& jvmPath, const SearchTarget& target);
        void create(const std::string& jvmPath, const std::string& jvmOptions, const SearchTarget& target);
        void create(const std::string& jvmPath, const std::string& jvmOptions, const std::string& serverOptions, const SearchTarget& target);
        bool exists();
        std::string getJvmPath();
        std::string getJvmArguments();
        std::string getServerArguments();
    };
}

#endif