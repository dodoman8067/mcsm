#ifndef __MCSM_DETECT_INSTALL_H__
#define __MCSM_DETECT_INSTALL_H__

#include <mcsm/jvm/jvm_option.h>
#include <mcsm/util/cli/arg_parser.h>
#include <mcsm/jvm/java_detector.h>

namespace mcsm {
    class JvmDetectInstallSubCommand {
    public:
        JvmDetectInstallSubCommand() = default;
        ~JvmDetectInstallSubCommand() = default;
        void execute(const std::vector<std::string>& args);
    };
}


#endif // __MCSM_DETECT_INSTALL_H__