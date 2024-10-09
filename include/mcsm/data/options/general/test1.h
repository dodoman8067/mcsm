#ifndef __MCSM_TEST1_H__
#define __MCSM_TEST1_H__

#include <mcsm/data/options/general_option.h>

namespace mcsm {
    class Test1 : public mcsm::GeneralProperty {
    private:
        std::string name;
        nlohmann::json value;
    public:
        Test1(const std::string& name): GeneralProperty(name){}
        ~Test1() = default;

        nlohmann::json getDefaultValue() override {
            nlohmann::json a;
            a["asd"] = false;
            return a;
        }
    };
}

#endif // __MCSM_TEST1_H__