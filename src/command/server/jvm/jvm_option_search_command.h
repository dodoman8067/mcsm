#ifndef __MCSM_JVM_OPTION_SEARCH_COMMAND_H__
#define __MCSM_JVM_OPTION_SEARCH_COMMAND_H__

#include "../../base/command.h"
#include "../../../data/options/jvm_option.h"

namespace mcsm {
    class JvmOptionSearchCommand : public mcsm::Command {
    private:
        std::vector<std::unique_ptr<mcsm::JvmOption>> search(const mcsm::SearchTarget& target, const std::string& name);
        std::string getProfileName(const std::vector<std::string>& args) const;
        mcsm::SearchTarget getSearchTarget(const std::vector<std::string>& args);
    public:
        JvmOptionSearchCommand(const std::string& name, const std::string& description);
        ~JvmOptionSearchCommand();
        void execute(const std::vector<std::string>& args) override;
    };
}


#endif // __MCSM_JVM_OPTION_SEARCH_COMMAND_H__