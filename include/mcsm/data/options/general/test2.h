#ifndef __MCSM_TEST2_H__
#define __MCSM_TEST2_H__

#include <mcsm/data/options/general_option.h>

namespace mcsm {
    class Test2 : public mcsm::GeneralProperty {
    private:
        std::string name;
        nlohmann::json value;
    public:
        Test2(const std::string& name): GeneralProperty(name){}
        ~Test2() = default;

        nlohmann::json getDefaultValue() override {
            return 1;
        }
    };
}

#endif // __MCSM_TEST2_H__