#ifndef __MCSM_SERVER_CONFIG_LOADER_H__
#define __MCSM_SERVER_CONFIG_LOADER_H__

#include <mcsm/data/option.h>
#include <mcsm/server/server.h>

namespace mcsm {
    class ServerConfigLoader {
    public:
        explicit ServerConfigLoader(const std::string& path);
        ServerConfigLoader(const ServerConfigLoader& other)
            : configPath(other.configPath),
              optionHandle(other.optionHandle ? std::make_unique<mcsm::Option>(*other.optionHandle) : nullptr),
              isLoaded(other.isLoaded){}
        ~ServerConfigLoader();

        mcsm::VoidResult loadConfig();

        template <typename T>
        inline tl::expected<T, mcsm::Error> get(const std::string& key) const{
            if(!this->isLoaded){
                mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {
                    "ServerConfigLoader function called without loadConfig.",
                    "High chance to be an internal issue. Please open an issue in Github."
                }});
                return T();
            }

            nlohmann::json value = this->optionHandle->getValue(key);
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) {
                return T();
            }

            if(value == nullptr){
                mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFound("\"" + key + "\"", this->optionHandle->getName())});
                return T();
            }

            if constexpr(std::is_same<T, std::vector<int>>::value ||
                        std::is_same<T, std::vector<double>>::value ||
                        std::is_same<T, std::vector<bool>>::value ||
                        std::is_same<T, std::vector<std::string>>::value){
                if(value.type() != nlohmann::json::value_t::array){
                    mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"" + key + "\"", "array")});
                    return T();
                }
            }else if (value.type() != getJsonType<T>()){
                mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"" + key + "\"", value.type_name())});
                return T();
            }
            if constexpr (std::is_same<T, std::string>::value){
                if(!mcsm::isSafeString(value)){
                    mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::unsafeString(value)});
                    return T();
                }
            }

            mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
            return value.get<T>();
        }

        mcsm::StringResult getServerName() const;
        mcsm::VoidResult setServerName(const std::string& name);

        mcsm::StringResult getServerVersion() const;
        mcsm::VoidResult setServerVersion(const std::string& version);

        tl::expected<std::unique_ptr<mcsm::JvmOption>, mcsm::Error> getDefaultOption() const;
        mcsm::VoidResult setDefaultOption(mcsm::JvmOption& jvmOption);

        mcsm::StringResult getServerType() const;

        mcsm::StringResult getServerJarFile() const;
        mcsm::VoidResult setServerJarFile(const std::string& name);

        mcsm::StringResult getServerJarBuild() const;
        mcsm::VoidResult setServerJarBuild(const std::string& build);

        mcsm::BoolResult doesAutoUpdate() const;
        mcsm::VoidResult setAutoUpdate(const bool& update);

        mcsm::Option* getHandle() const;

        bool isFullyLoaded() const;

        tl::expected<mcsm::Server*, mcsm::Error> getServerInstance();
        
    private:
        std::string configPath;
        std::unique_ptr<mcsm::Option> optionHandle;
        bool isLoaded;

        template <typename T>
        nlohmann::json::value_t getJsonType() const;
    };
}

#endif // __MCSM_SERVER_CONFIG_LOADER_H__