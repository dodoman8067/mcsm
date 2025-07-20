#include <mcsm/data/options/general_option.h>

static auto& instance = mcsm::GeneralOption::getGeneralOption();
static bool init = false;

// loads poperties vector from json
mcsm::VoidResult mcsm::GeneralOption::load(){
    instance.properties.reserve(3);
    mcsm::VoidResult jLoadRes = instance.optionHandle->load();
    if(!jLoadRes) return jLoadRes;

    nlohmann::json json = instance.optionHandle->getData();

    bool recovered = false;

    for(auto* v : mcsm::ServerRegistry::getServerRegistry().getRegisteredProperties()){
        if(json[v->getName()] == nullptr){ 
            json[v->getName()] = v->getDefaultValue();
            recovered = true; 
        }
    }

    if(recovered){
        mcsm::VoidResult saveRes = instance.optionHandle->save(json);

        if(!saveRes) return saveRes;
    }

    for(auto it = json.begin(); it != json.end(); ++it){
        auto property = mcsm::ServerRegistry::getServerRegistry().getGeneralProperty(it.key());
        if(!property){
            return tl::unexpected(property.error());
        }
        property.value()->setCurrentValue(it.value());
        instance.properties.push_back(property.value());
    }

    auto* property = getProperty("advanced_json_parse_fail_errors");
    if(property == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_NOT_FOUND_PLUS_FIX, {"advanced_json_parse_fail_errors", "general option", "Value example: \"advanced_json_parse_fail_errors\": false"});
        return tl::unexpected(err);
    }

    const nlohmann::json& propertyValue = property->getCurrentValue();
    if(!propertyValue.is_boolean()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_WRONG_TYPE_PLUS_FIX, {"advanced_json_parse_fail_errors", "general option", "boolean", "Value example: \"advanced_json_parse_fail_errors\": false"});
        return tl::unexpected(err);
    }

    this->advp = propertyValue;

    auto* property1 = getProperty("screen_binary_path");
    if(property1 == nullptr){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_NOT_FOUND_PLUS_FIX, {"screen_binary_path", "general option", "Value example: \"/usr/bin/screen\""});
        return tl::unexpected(err);
    }

    const nlohmann::json& propertyValue1 = property1->getCurrentValue();
    if(!propertyValue1.is_string()){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::JSON_WRONG_TYPE_PLUS_FIX, {"screen_binary_path", "general option", "string", "Value example: \"/usr/bin/screen\""});
        return tl::unexpected(err);
    }

    this->sbp = propertyValue1;

    return {};
}

// saves current properties vector value to json
mcsm::VoidResult mcsm::GeneralOption::save(){
    nlohmann::json json;

    for(auto* v : mcsm::ServerRegistry::getServerRegistry().getRegisteredProperties()){
        json[v->getName()] = v->getCurrentValue();
    }

    mcsm::VoidResult saveRes = instance.optionHandle->save(json);

    return saveRes;
}

mcsm::VoidResult mcsm::GeneralOption::initialize(){
    if(init){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::INTERNAL_FUNC_EXECUTION_FAILED, {"GeneralOption::initialize()", "Called more than once"});
        return tl::unexpected(err);
    }
    init = true;
    
    instance.optionHandle = std::make_unique<mcsm::Option>(mcsm::asGlobalConfigPath("/"), "general_options");

    return instance.load();
}

mcsm::GeneralProperty* mcsm::GeneralOption::getProperty(const std::string& propertyName) const {
    for(auto& v : instance.properties){
        if(v->getName() == propertyName){
            return v;
        }
    }
    return nullptr;
}

mcsm::VoidResult mcsm::GeneralOption::setProperty(const std::string& propertyName, nlohmann::json newValue){
    for(auto& v : instance.properties){
        if(v->getName() == propertyName){
            v->setCurrentValue(newValue);
            break;
        }
    }
    return {};
}

mcsm::Option* mcsm::GeneralOption::getOption(){
    return &(*instance.optionHandle);
}

std::vector<mcsm::GeneralProperty*>& mcsm::GeneralOption::getProperties() const {
    return instance.properties;
}

bool mcsm::GeneralOption::advancedParseEnabled() const {
    return instance.advp;
}

std::string mcsm::GeneralOption::screenBinPathProperty() const {
    return instance.sbp;
}

mcsm::GeneralOption& mcsm::GeneralOption::getGeneralOption(){
    static mcsm::GeneralOption o;
    return o;
}