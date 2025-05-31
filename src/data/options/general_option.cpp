#include <mcsm/data/options/general_option.h>

static auto& instance = mcsm::GeneralOption::getGeneralOption();
static bool init = false;

// loads poperties vector from json
mcsm::VoidResult mcsm::GeneralOption::load(){
    instance.properties.reserve(3);
    mcsm::Result jLoadRes = instance.optionHandle->load();
    if(!jLoadRes.isSuccess()) return jLoadRes;

    nlohmann::json json = instance.optionHandle->getData();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        //if(resp.second[0].find("parse json") == std::string::npos){
            mcsm::Result res(resp.first, resp.second);
            return res;
        //}
    }

    bool recovered = false;

    for(auto* v : mcsm::ServerRegistry::getServerRegistry().getRegisteredProperties()){
        if(json[v->getName()] == nullptr){ 
            json[v->getName()] = v->getDefaultValue();
            recovered = true; 
        }
    }

    if(recovered){
        mcsm::Result saveRes = instance.optionHandle->save(json);

        if(!saveRes.isSuccess()) return saveRes;
    }

    for(auto it = json.begin(); it != json.end(); ++it){
        auto property = mcsm::ServerRegistry::getServerRegistry().getGeneralProperty(it.key());
        if(property){
            property->setCurrentValue(it.value());
            instance.properties.push_back(property);
        }else{
            return {mcsm::ResultType::MCSM_FAIL, {"Non-registered general property " + it.key() + " detected."}};
        }
    }

    auto* property = getProperty("advanced_json_parse_fail_errors");
    if(property == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFoundPlusFix("advanced_json_parse_fail_errors", "general option", "\"advanced_json_parse_fail_errors\": false")});
        return res;
    }

    const nlohmann::json& propertyValue = property->getCurrentValue();
    if(!propertyValue.is_boolean()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongTypePlusFix("advanced_json_parse_fail_errors", "general option", "boolean", "false or true")});
        return res;
    }

    this->advp = propertyValue;

    auto* property1 = getProperty("screen_binary_path");
    if(property1 == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFoundPlusFix("screen_binary_path", "general option", "\"screen_binary_path\": \"/usr/bin/screen\"")});
        return res;
    }

    const nlohmann::json& propertyValue1 = property1->getCurrentValue();
    if(!propertyValue1.is_string()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongTypePlusFix("screen_binary_path", "general option", "string", "\"/usr/bin/screen\"")});
        return res;
    }

    this->sbp = propertyValue1;

    return {mcsm::ResultType::MCSM_SUCCESS, {"Success"}};
}

// saves current properties vector value to json
mcsm::VoidResult mcsm::GeneralOption::save(){
    nlohmann::json json;

    for(auto* v : mcsm::ServerRegistry::getServerRegistry().getRegisteredProperties()){
        json[v->getName()] = v->getCurrentValue();
    }

    mcsm::Result saveRes = instance.optionHandle->save(json);

    return saveRes;
}

mcsm::VoidResult mcsm::GeneralOption::initialize(){
    if(init){
        return {mcsm::ResultType::MCSM_FAIL, {
            "GeneralOption::initialize() called more than once.",
            "Likely an internal issue. Open an issue at Github."
        }};
    }
    init = true;
    
    instance.optionHandle = std::make_unique<mcsm::Option>(mcsm::asGlobalConfigPath("/"), "general_options");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

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
    return {mcsm::ResultType::MCSM_SUCCESS, {"Success"}};
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