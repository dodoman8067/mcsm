#include <mcsm/data/options/general_property.h>

mcsm::GeneralProperty::GeneralProperty(const std::string& name){
    this->name = name;
}

mcsm::GeneralProperty::~GeneralProperty(){
    
}

std::string mcsm::GeneralProperty::getName() const {
    return this->name;
}

nlohmann::json mcsm::GeneralProperty::getCurrentValue(){
    return this->value;
}

void mcsm::GeneralProperty::setCurrentValue(nlohmann::json& value){
    this->value = value;
}