#ifndef __MCSM_DOWNLOADABLE_H__
#define __MCSM_DOWNLOADABLE_H__

#include <vector>
#include <memory>

namespace mcsm {
    /**
     * @brief Represents a downloadable server interface.
     */
    class Downloadable {
    public:
        /**
        * @brief Returns vector of downloadable versions.
        * 
        * @return std::unique_ptr<std::vector<std::string>> 
        */
        virtual std::unique_ptr<std::vector<std::string>> getAvailableVersions() = 0;

        /**
         * @brief Downloads a server in current directory.
         * 
         * @param version version of the server
         */
        virtual void download(const std::string& version) = 0;

        /**
         * @brief Downloads a server in specified directory.
         * 
         * @param version version of the server
         * @param path file's path
         */
        virtual void download(const std::string& version, const std::string& path) = 0;

        /**
         * @brief Downloads a server in specified directory with the following name.
         * 
         * @param version version of the server
         * @param path file's path
         * @param name file's name
         */
        virtual void download(const std::string& version, const std::string& path, const std::string& name) = 0;

        /**
         * @brief Returns the following server has this version
         * 
         * @param version version of the server
         * @return true when version exists, otherwise false
         */
        virtual bool hasVersion(const std::string& version) = 0;
    };
}

#endif // __MCSM_DOWNLOADABLE_H__