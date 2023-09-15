/*
Copyright (c) 2023 dodoman8067

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "command.h"

mcsm::Command::Command(const std::string& name, const std::string& description){
    this->name = name;
    this->description = description;
    this->aliases = new std::vector<std::string>();
    this->aliases->shrink_to_fit();
}

mcsm::Command::~Command(){
    if(this->aliases != nullptr){
        delete this->aliases;
        this->aliases = nullptr;
    }
}

std::string mcsm::Command::getName() const {
    return this->name;
}

std::string mcsm::Command::getDescription() const {
    return this->description;
}

void mcsm::Command::addAliases(const std::string& value){
    this->aliases->push_back(value);
}

bool mcsm::Command::hasAliases(const std::string& value) const{
    for(std::string& alias : *this->aliases){
        if(alias == value){
            return true;
        }
    }
    return false;
}

const std::vector<std::string>& mcsm::Command::getAliases() const {
    return *this->aliases;
}