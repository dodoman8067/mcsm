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

#include <mcsm/util/string_utils.h>

namespace mcsm {
    /**
     * Checks if the string `str` starts with the specified `value`.
     * @param str the string to check
     * @param value the prefix to check for at the beginning of `str`
     * @return true if `str` starts with `value`, otherwise false
     */
    bool startsWith(const std::string& str, const std::string& value){
        return str.rfind(value, 0) == 0;
    }

    /**
     * Checks if the string `str` ends with the specified `value`.
     * @param str the string to check
     * @param value the suffix to check for at the end of `str`
     * @return true if `str` ends with `value`, otherwise false
     */
    bool endsWith(const std::string& str, const std::string& value){
        // Ensure that `value` is not longer than `str` to avoid out-of-bounds comparisons
        if (str.length() < value.length()) {
            return false;
        }
        return str.compare(str.length() - value.length(), value.length(), value) == 0;
    }

    /**
     * Checks if the string `str` is either whitespace or empty.
     * @param str the string to check
     * @return true if `str` is whitespace or empty, otherwise false
     */
    bool isWhitespaceOrEmpty(const std::string& str){
        // If the string is empty, consider it whitespace
        if (str.empty()) {
            return true;
        }
        // Check if the string has only whitespace characters
        return str.find_first_not_of(" \t\n\v\f\r") == std::string::npos;
    }

    /**
     * Replaces all occurrences of a specified `value` with a `replacement` in the string `str`.
     * @param str the string to modify
     * @param value the substring to replace
     * @param replacement the string to replace `value` with
     */
    void replaceAll(std::string& str, const std::string& value, const std::string& replacement){
        std::string::size_type pos = 0;
        // Iterate through the string and replace all occurrences of `value`
        while ((pos = str.find(value, pos)) != std::string::npos) {
            str.replace(pos, value.length(), replacement);
            pos += replacement.length();
        }
    }
}
