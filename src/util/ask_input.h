#ifndef __MCSM_ASK_INPUT_H__
#define __MCSM_ASK_INPUT_H__

#include <iostream>
#include <vector>
#include <string>
#include "string_utils.h"

namespace mcsm {
    void askInput(std::vector<std::string>& arguments, std::string& input, const std::string &outputWhenNone);
}

#endif