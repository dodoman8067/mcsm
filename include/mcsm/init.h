#ifndef __MCSM_INIT_H__
#define __MCSM_INIT_H__

#include <mcsm/command/command_manager.h>
#include <mcsm/command/base/command.h>
#include <mcsm/command/util/version_command.h>
#include <mcsm/command/util/help_command.h>
#include <mcsm/command/server/generate_server_command.h>
#include <mcsm/command/server/start_server_command.h>
#include <mcsm/command/server/jvm/jvm_option_generator_command.h>
#include <mcsm/command/server/jvm/jvm_option_search_command.h>
#include <mcsm/command/server/jvm/jvm_test_command.h>
#include <mcsm/command/server/jvm/jvm_option_edit_command.h>
#include <mcsm/command/server/view_server_command.h>
#include <mcsm/command/server/view_server_type_command.h>

namespace mcsm {
    class init {
    private:
        bool* initialized;

        void initCommands(const std::string& version);
    public:
        init();
        ~init();

        void initMCSM(const std::string& version);
        
        bool isInitialized() const;
    };
}
#endif // __MCSM_INIT_H__