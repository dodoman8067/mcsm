#ifndef __MCSM_JVM_OPTION_EDIT_COMMAND_H__
#define __MCSM_JVM_OPTION_EDIT_COMMAND_H__

#include <mcsm/command/base/command.h>
#include <mcsm/data/options/jvm_option.h>

namespace mcsm {
    class JvmOptionEditCommand : public mcsm::Command {
    private:
        mcsm::SearchTarget getSaveTarget(const std::vector<std::string>& args);
        std::string getJvmPath(const std::vector<std::string>& args) const;
        std::vector<std::string> getJvmArguments(const std::vector<std::string>& args) const;
        std::vector<std::string> getServerArguments(const std::vector<std::string>& args) const;
        std::string getOptionName(const std::vector<std::string>& args) const;
        std::string getProfileName(const std::vector<std::string>& args, const mcsm::SearchTarget& target) const;
        inline void editProfile(const std::vector<std::string>& args, const mcsm::SearchTarget& target);
    public:
        JvmOptionEditCommand(const std::string& name, const std::string& description);
        ~JvmOptionEditCommand();
        void execute(const std::vector<std::string>& args) override;
    };
}

#endif // __MCSM_JVM_OPTION_EDIT_COMMAND_H__