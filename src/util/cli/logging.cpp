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

#include <mcsm/util/cli/logging.h>

/**
 * Prints normal information message to the console.
 * @param message the string to print
*/
void mcsm::info(const std::string& message){
    std::cout << "[mcsm/INFO] " << message << "\n";
}

/**
 * Prints successful (green) message to the console.
 * @param message the string to print
*/
void mcsm::success(const std::string& message){
    std::cout << termcolor::bright_green << "[mcsm/INFO] " << message << termcolor::reset << "\n";
}

/**
 * Prints warning (yellow) message to the console.
 * @param message the string to print
*/
void mcsm::warning(const std::string& message){
    std::cerr << termcolor::bright_yellow << "[mcsm/WARN] " << message << termcolor::reset << "\n";
}

/**
 * Prints error (red) message to the console.
 * @param message the string to print
*/
void mcsm::error(const std::string& message){
    std::cerr << termcolor::bright_red << "[mcsm/ERROR] " << message << termcolor::reset << "\n";
}