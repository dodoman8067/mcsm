#ifndef __MCSM_IMPORT_H__
#define __MCSM_IMPORT_H__

#include <mcsm/jvm/jvm_option.h>
#include <mcsm/util/cli/arg_parser.h>

namespace mcsm {
    class JvmImportSubCommand {
    private:
        mcsm::SearchTarget getSaveTarget(const mcsm::ArgumentParser& args) const;
        
        std::unique_ptr<mcsm::JvmOption> getJvmOption(const mcsm::ArgumentParser& parser);

        void copyToRepo(const mcsm::JvmOption& opt, const mcsm::SearchTarget& target);
    public:
        JvmImportSubCommand() = default;
        ~JvmImportSubCommand() = default;
        void execute(const std::vector<std::string>& args);
    };
}
#endif // __MCSM_IMPORT_H__