#ifndef __VERSION_GETTER_H__
#define __VERSION_GETTER_H__

#include <string>
#include <curl/curl.h>
namespace mcsm {
    class VersionGetter {
    
    private:
        CURL* curl;
        void initVaribles();

    public:
        VersionGetter(CURL* curl);
        ~VersionGetter();

        int getPaperVersion(std::string& mc_version);
    };
}

#endif