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

#ifndef __MCSM_RESULT_H__
#define __MCSM_RESULT_H__

#include <vector>
#include <mcsm/util/cli/logging.h>

namespace mcsm {
    enum class ResultType {
        MCSM_OK,
        MCSM_WARN,
        MCSM_FAIL,
        MCSM_UNKNOWN
    };

    class Result {
    private:
        mcsm::ResultType result;
        std::vector<std::string> message;
        std::pair<mcsm::ResultType, std::vector<std::string>> resPair;
    public:
        Result(const mcsm::ResultType& type, const std::vector<std::string>& message);
        ~Result();
        mcsm::ResultType getResult() const;
        std::vector<std::string> getMessage() const;
        std::pair<mcsm::ResultType, std::vector<std::string>> getResultPair() const;
        bool isSuccess() const;
        void printMessage();
        void printMessage(const mcsm::ResultType& type);
    };

    namespace message_utils {
        inline std::vector<std::string> jsonWrongType(const std::string& key, const std::string& type);
        inline std::vector<std::string> jsonNotFound(const std::string& key, const std::string& type);
        inline std::vector<std::string> jsonNotFoundPlusFix(const std::string& key, const std::string& type, const std::string& changeInto);
        inline std::vector<std::string> jsonParseFailed();
        inline std::vector<std::string> jsonParseFailedCannotBeModified();

        inline std::vector<std::string> serverAlreadyConfigured();
        inline std::vector<std::string> serverWrongInstanceGenerated(const std::string& serverName);
        inline std::vector<std::string> serverUnsupportedVersion();
        inline std::vector<std::string> serverUnsupportedVersion(const std::string& build);

        inline std::vector<std::string> fileCreateFailed(const std::string& name);
        inline std::vector<std::string> fileSaveFailed(const std::string& name);
        inline std::vector<std::string> fileNotFound(const std::string& name);

        inline std::vector<std::string> curlInitFailed();
        inline std::vector<std::string> getRequestFailed(const std::string& url);
        inline std::vector<std::string> downloadRequestFailed(const std::string& url);
        inline std::vector<std::string> downloadTargetIsNotText(const std::string& url);
    }

    std::pair<mcsm::ResultType, std::vector<std::string>> getLastResult();
    void updateLastResult(const std::pair<mcsm::ResultType, std::vector<std::string>> newRes);
    void printResultMessage();
    void printResultMessage(const std::pair<mcsm::ResultType, std::vector<std::string>> res);
}


#endif // __MCSM_RESULT_H__