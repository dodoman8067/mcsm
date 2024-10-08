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

#include <mcsm/util/mc/mc_utils.h>
#include <iostream>
#include <sstream>

std::vector<std::string> mcsm::getMinecraftVersions(){
    std::vector<std::string> versions = {
        "1.8",
        "1.8.8",
        "1.9.4",
        "1.10.2",
        "1.11.2",
        "1.12",
        "1.12.1",
        "1.12.2",
        "1.13",
        "1.13.1",
        "1.13.2",
        "1.14",
        "1.14.1",
        "1.14.2",
        "1.14.3",
        "1.14.4",
        "1.15",
        "1.15.1",
        "1.15.2",
        "1.16",
        "1.16.1",
        "1.16.2",
        "1.16.3",
        "1.16.4",
        "1.16.5",
        "1.17",
        "1.17.1",
        "1.18",
        "1.18.1",
        "1.18.2",
        "1.19",
        "1.19.1",
        "1.19.2",
        "1.19.3",
        "1.19.4",
        "1.20",
        "1.20.1",
        "1.20.2",
        "1.20.3",
        "1.20.4"
    };
    return versions;
}

std::vector<int> splitVersion(const std::string& version){
    std::stringstream ss(version);
    std::string segment;
    std::vector<int> segments;

    while (std::getline(ss, segment, '.')){
        segments.push_back(std::stoi(segment));
    }

    return segments;
}

int mcsm::compareVersions(const std::string& a, const std::string& b) {
    std::vector<int> version1 = splitVersion(a);
    std::vector<int> version2 = splitVersion(b);

    size_t length = std::max(version1.size(), version2.size());
    for(size_t i = 0; i < length; ++i){
        int num1 = (i < version1.size()) ? version1[i] : 0;
        int num2 = (i < version2.size()) ? version2[i] : 0;

        if(num1 < num2){
            return -1;  // a is less than b
        }else if (num1 > num2){
            return 1;   // a is greater than b
        }
    }

    return 0;  // a is equal to b
}