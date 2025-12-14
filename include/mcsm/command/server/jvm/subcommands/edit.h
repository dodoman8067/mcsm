#ifndef __MCSM_EDIT_H__
#define __MCSM_EDIT_H__

#include <mcsm/jvm/jvm_option.h>
#include <mcsm/util/cli/arg_parser.h>

namespace mcsm {
    class JvmEditSubCommand {
    private:
        mcsm::SearchTarget getSaveTarget(const mcsm::ArgumentParser& args);
        
        std::string getJvmPath(const mcsm::ArgumentParser& args) const;

        std::vector<std::string> getJvmArguments(const mcsm::ArgumentParser& args) const;
        std::vector<std::string> getServerArguments(const mcsm::ArgumentParser& args) const;

        std::string getOptionName(const mcsm::ArgumentParser& args) const;
        std::string getProfileName(const mcsm::ArgumentParser& args, const mcsm::SearchTarget& target) const;

        inline void editProfile(const mcsm::ArgumentParser& args, const mcsm::SearchTarget& target);
        inline void printDifference(const std::string& category, const std::vector<std::string>& from, const std::vector<std::string>& to) const;
    public:
        JvmEditSubCommand() = default;
        ~JvmEditSubCommand() = default;
        void execute(const std::vector<std::string>& args);
    };
}

#endif // __MCSM_EDIT_H__