#ifndef __MCSM_CREATE_H__
#define __MCSM_CREATE_H__


#include <mcsm/jvm/jvm_option.h>

namespace mcsm {
    class JvmCreateSubCommand {
    private:
        mcsm::SearchTarget getSaveTarget(const std::vector<std::string>& args);
        std::string getJvmPath(const std::vector<std::string>& args) const;
        std::vector<std::string> getJvmArguments(const std::vector<std::string>& args) const;
        std::vector<std::string> getServerArguments(const std::vector<std::string>& args) const;
        std::string getOptionName(const std::vector<std::string>& args) const;
        std::string getProfileName(const std::vector<std::string>& args, const mcsm::SearchTarget& target) const;
        inline void createProfile(const std::vector<std::string>& args, const mcsm::SearchTarget& target);
    public:
        JvmCreateSubCommand();
        ~JvmCreateSubCommand() = default;
        void execute(const std::vector<std::string>& args);
    };
}

#endif // __MCSM_CREATE_H__