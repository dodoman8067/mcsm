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
#include <vector>

namespace mcsm {
    /**
     * Checks if the string `str` starts with the specified `value`.
     * @param str string to check
     * @param value prefix to check for at the beginning of `str`
     * @return true if `str` starts with `value`, otherwise false
     */
    bool startsWith(const std::string& str, const std::string& value);

    /**
     * Checks if the string `str` ends with the specified `value`.
     * @param str string to check
     * @param value suffix to check for at the end of `str`
     * @return true if `str` ends with `value`, otherwise false
     */
    bool endsWith(const std::string& str, const std::string& value);

    /**
     * Checks if the string `str` is either whitespace or empty.
     * @param str string to check
     * @return true if `str` is whitespace or empty, otherwise false
     */
    bool isWhitespaceOrEmpty(const std::string& str);
    
    /**
     * Replaces all occurrences of a specified `value` with a `replacement` in the string `str`.
     * @param str string to modify
     * @param value substring to replace
     * @param replacement string to replace `value` with
     */
    void replaceAll(std::string& str, const std::string& value, const std::string& replacement);

    std::string safeString(const std::string& str);

    bool isSafeString(const std::string& str);

    bool is_number(const std::string& s);

    std::string normalizePath(const std::string& p);

    // used on formatting "jvm_version" to "Jvm version"
    std::string formatPrompt(const std::string& key);

    std::string vecToString(const std::vector<std::string>& vec);
}

#endif