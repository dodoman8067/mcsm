#ifndef __MCSM_ASK_INPUT_H__
#define __MCSM_ASK_INPUT_H__

#include <iostream>
#include <vector>
#include <string>

namespace mcsm {
    void askInput(std::vector<std::string>& arguments, std::string& input, std::string& outputWhenNone);
}

#endif