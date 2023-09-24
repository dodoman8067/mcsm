#ifndef __MCSM_JVM_OPTION_GENERATOR_COMMAND_H__
#define __MCSM_JVM_OPTION_GENERATOR_COMMAND_H__

#include "command.h"
#include "../data/options/jvm_option.h"

namespace mcsm {
    class JvmOptionGeneratorCommand : public mcsm::Command {
    public:
        JvmOptionGeneratorCommand(const std::string& name, const std::string& description);
        ~JvmOptionGeneratorCommand();
        void execute(const std::vector<std::string>& args) override;
    private:
        mcsm::SearchTarget getSaveTarget(const std::vector<std::string>& args);
        std::string getJvmPath(const std::vector<std::string>& args) const;
        std::vector<std::string> getJvmArguments(const std::vector<std::string>& args) const;
        std::vector<std::string> getServerArguments(const std::vector<std::string>& args) const;
        std::string getOptionName(const std::vector<std::string>& args) const;
        std::string getProfileName(const std::vector<std::string>& args, const mcsm::SearchTarget& target) const;
        inline void createProfile(const std::vector<std::string>& args, const mcsm::SearchTarget& target);
    };
}

#endif