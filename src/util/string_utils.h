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

#ifndef __MCSM_STRING_UTILS_H__
#define __MCSM_STRING_UTILS_H__

#include <string>
#include <algorithm>

namespace mcsm {
    // Returns true when given string starts with specified value.
    // @param str string to check
    // @param value value to check
    // @return true if string starts with specified value, otherwise false
    bool startsWith(const std::string& str, const std::string& value);

    // Returns true when given string ends with specified value.
    // @param str string to check
    // @param value value to check
    // @return true if string ends with specified value, otherwise false
    bool endsWith(const std::string& str, const std::string& value);

    // Returns true when given string only contains whitespaces(spaces, tabs etc...).
    // @param str string to check
    // @return true if string only contains whitespaces, otherwise false
    bool isWhitespaceOrEmpty(const std::string& str);

    // Replaces all value found in the str variable with replacement value.
    // @param str string to replace
    // @param value value that is going to be replaced
    // @param replacement replacement
    void replaceAll(std::string& str, const std::string& value, const std::string& replacement);
}

#endif