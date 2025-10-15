#ifndef __MCSM_JVM_SEARCH_H__
#define __MCSM_JVM_SEARCH_H__

#include <mcsm/jvm/jvm_option.h>
#include <mcsm/util/cli/arg_parser.h>

namespace mcsm {
    class JvmSearchSubCommand {
    private:
        mcsm::SearchTarget getSearchTarget(const mcsm::ArgumentParser& args) const;

        std::string getOptionName(const mcsm::ArgumentParser& args) const;

        std::vector<std::unique_ptr<mcsm::JvmOption>> search(const mcsm::SearchTarget& target, const std::string& name);
    public:
        JvmSearchSubCommand() = default;
        ~JvmSearchSubCommand() = default;
        void execute(const std::vector<std::string>& args);
    };
}

#endif // __MCSM_JVM_SEARCH_H__