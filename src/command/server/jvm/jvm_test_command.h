#ifndef __MCSM_JVM_TEST_COMMAND_H__
#define __MCSM_JVM_TEST_COMMAND_H__

#include "../../base/command.h"
#include "../../../data/options/jvm_option.h"
#include "../../../util/cli_utils.h"

namespace mcsm {
    class JvmTestCommand : public mcsm::Command {
    private:
        mcsm::SearchTarget getSearchTarget(const std::vector<std::string>& args);
        std::unique_ptr<mcsm::JvmOption> searchOption(const mcsm::SearchTarget& target, const std::string& name);
        std::string getProfileName(const std::vector<std::string>& args) const;
        inline void performTest(std::unique_ptr<mcsm::JvmOption> option);
    public:
        JvmTestCommand(const std::string& name, const std::string& description);
        ~JvmTestCommand();
        void execute(const std::vector<std::string>& args) override;
    };
}

#endif // __MCSM_JVM_TEST_COMMAND_H__