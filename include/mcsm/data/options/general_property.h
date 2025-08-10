#ifndef __MCSM_GENERAL_PROPERTY_H__
#define __MCSM_GENERAL_PROPERTY_H__

#include <nlohmann/json.hpp>
#include <mcsm/util/cli/result.h>
#include <string>

namespace mcsm {
    class GeneralProperty {
    private:
        std::string name_;
        nlohmann::json value_;

    public:
        explicit GeneralProperty(const std::string& name) : name_(name) {}
        virtual ~GeneralProperty() = default;

        // ASan-friendly
        static void operator delete(void* p) noexcept { ::operator delete(p); }
        static void operator delete(void* p, std::size_t) noexcept { ::operator delete(p); }

        std::string_view getName() const { return name_; }

        virtual nlohmann::json getDefaultValue() const = 0;   // const & no inline keyword

        const nlohmann::json& getCurrentValue() const { return value_; }
        void setCurrentValue(const nlohmann::json& v) { value_ = v; }         // or by value + std::move
        // void setCurrentValue(nlohmann::json v) { value_ = std::move(v); }
    };
}

#endif // __MCSM_GENERAL_PROPERTY_H__