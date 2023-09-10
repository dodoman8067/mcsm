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

#include "ask_input.h"

void mcsm::askInput(std::vector<std::string>& arguments, std::string& input, const std::string &outputWhenNone){
    for(std::string& s : arguments){
        std::cout << s << std::endl;
    }
    std::cout << " " << std::endl;
    while(true){
        std::cout << ">> ";
        std::getline(std::cin, input);
            
        if(!mcsm::isWhitespaceOrEmpty(input)){
            break;
        }else{
            if(!outputWhenNone.empty()){
                input = outputWhenNone;
                break;
            }
            std::cout << "Not a valid input; Please try again." << std::endl;
        }
    }
}