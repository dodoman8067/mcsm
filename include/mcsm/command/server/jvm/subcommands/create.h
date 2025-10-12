#ifndef __MCSM_CREATE_H__
#define __MCSM_CREATE_H__


#include <mcsm/jvm/jvm_option.h>
#include <mcsm/util/cli/arg_parser.h>

namespace mcsm {
    class JvmCreateSubCommand {
    private:
        mcsm::SearchTarget getSaveTarget(const mcsm::ArgumentParser& args);
        std::string getJvmPath(const mcsm::ArgumentParser& args) const;
        std::vector<std::string> getJvmArguments(const mcsm::ArgumentParser& args) const;
        std::vector<std::string> getServerArguments(const mcsm::ArgumentParser& args) const;
        std::string getOptionName(const mcsm::ArgumentParser& args) const;
        std::string getProfileName(const mcsm::ArgumentParser& args, const mcsm::SearchTarget& target) const;
        inline void createProfile(const mcsm::ArgumentParser& args, const mcsm::SearchTarget& target);
    public:
        JvmCreateSubCommand() = default;
        ~JvmCreateSubCommand() = default;
        void execute(const std::vector<std::string>& args);
    };
}

#endif // __MCSM_CREATE_H__