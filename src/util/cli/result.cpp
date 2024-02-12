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

#include <mcsm/util/cli/result.h>

static std::pair<mcsm::ResultType, std::vector<std::string>> res = {mcsm::ResultType::MCSM_UNKNOWN, { "Not updated" }};

mcsm::Result::Result(const mcsm::ResultType& type, const std::vector<std::string>& message){
    this->result = type;
    this->message = message;
    this->resPair = {type, message};
    mcsm::updateLastResult(this->resPair);
}

mcsm::Result::~Result(){

}

mcsm::ResultType mcsm::Result::getResult() const {
    return this->result;
}

std::vector<std::string> mcsm::Result::getMessage() const {
    return this->message;
}

std::pair<mcsm::ResultType, std::vector<std::string>> mcsm::Result::getResultPair() const{
    return this->resPair;
}

void mcsm::Result::printMessage(){
    printMessage(this->result);
}

void mcsm::Result::printMessage(const mcsm::ResultType& type){
    switch (type){
        case mcsm::ResultType::MCSM_OK:
            for(const std::string& str : message){
                mcsm::info(str);
            }
            break;
        case mcsm::ResultType::MCSM_WARN:
            for(const std::string& str : message){
                mcsm::warning(str);
            }
            break;
        case mcsm::ResultType::MCSM_FAIL:
            for(const std::string& str : message){
                mcsm::error(str);
            }
            break;
        case mcsm::ResultType::MCSM_UNKNOWN:
            for(const std::string& str : message){
                mcsm::warning(str);
            }
            break;
        default:
            for(const std::string& str : message){
                mcsm::error(str);
            }
            mcsm::error("FATAL : Unknown usage of mcsm::ResultType detected. Please report this to GitHub. (https://github.com/dodoman8067/mcsm)");
            break;    
    }
}

std::pair<mcsm::ResultType, std::vector<std::string>> mcsm::getLastResult(){
    return res;
}

void mcsm::updateLastResult(const std::pair<mcsm::ResultType, std::vector<std::string>> newRes){
    res = newRes;
}