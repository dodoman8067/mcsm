#include "version_getter.h"

mcsm::VersionGetter::VersionGetter(CURL* curl){
    this->curl = curl;
}