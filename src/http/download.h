#ifndef __MCSM_DOWNLOAD_H__
#define __MCSM_DOWNLOAD_H__

#include <string>
#include <iostream>
#include <cstdio>
#include <filesystem>
#include <curl/curl.h>

namespace mcsm {
    void download(const std::string& name, const std::string& url);
    void download(const std::string& name, const std::string& url, const std::string& path);
}

#endif // __MCSM_DOWNLOAD_H__