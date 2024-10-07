#include <mcsm/data/options/general_option.h>

static auto& instance = mcsm::GeneralOption::getGeneralOption();
static bool init = false;

// loads poperties vector from json
mcsm::Result mcsm::GeneralOption::load(){
    return {mcsm::ResultType::MCSM_SUCCESS, {"Success"}};
}

// saves current properties vector value to json
mcsm::Result mcsm::GeneralOption::save(){
    return {mcsm::ResultType::MCSM_SUCCESS, {"Success"}};
}

mcsm::Result mcsm::GeneralOption::initialize(){
    if(init){
        return {mcsm::ResultType::MCSM_FAIL, {
            "GeneralOption::initialize() called more than once.",
            "Likely an internal issue. Open an issue at Github."
        }};
    }
    init = true;
    
    instance.optionHandle = std::make_unique<mcsm::GlobalOption>("/", "general_options");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    return instance.load();
}

mcsm::Result mcsm::GeneralOption::addProperty(mcsm::GeneralProperty& property){
    instance.properties.push_back(&property);
    return {mcsm::ResultType::MCSM_SUCCESS, {"Success"}};
}

mcsm::Result mcsm::GeneralOption::removeProperty(const std::string& propertyName){
    properties.erase(std::remove_if(properties.begin(), properties.end(),
        [&](mcsm::GeneralProperty* property){
            return property->getName() == propertyName;
        }), properties.end());

    return {mcsm::ResultType::MCSM_SUCCESS, {"Success"}};
}

mcsm::Result mcsm::GeneralOption::setProperty(const std::string& propertyName, nlohmann::json newValue){
    for(auto& v : instance.properties){
        if(v->getName() == propertyName){
            v->setCurrentValue(newValue);
            break;
        }
    }
    return {mcsm::ResultType::MCSM_SUCCESS, {"Success"}};
}

mcsm::GlobalOption* mcsm::GeneralOption::getOption(){
    return &(*instance.optionHandle);
}

std::vector<mcsm::GeneralProperty*>& mcsm::GeneralOption::getProperties() const {
    return instance.properties;
}

mcsm::GeneralOption& mcsm::GeneralOption::getGeneralOption(){
    static mcsm::GeneralOption o;
    return o;
}