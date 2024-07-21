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
#include <mcsm/server/server_generator.h>

mcsm::Result mcsm::server::generateBukkit(const std::string& name, mcsm::JvmOption& option, const std::string& version, const mcsm::BukkitServerType& type, const bool& autoUpdate){
    switch(type){
        case mcsm::BukkitServerType::PAPER: {
            std::shared_ptr<mcsm::PaperServer> server = std::make_shared<mcsm::PaperServer>();
            bool vExists = server->hasVersion(version);
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
                std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
                mcsm::Result res(resp.first, resp.second);
                return res;
            }
            if(!vExists){
                mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverUnsupportedVersion()});
                return res;
            }
            mcsm::ServerOption serverOption(version, server);
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
                std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
                mcsm::Result res(resp.first, resp.second);
                return res;
            }
            return configure(serverOption, name, option, autoUpdate);
        }
        case mcsm::BukkitServerType::PURPUR: {
            std::shared_ptr<mcsm::PurpurServer> server = std::make_shared<mcsm::PurpurServer>();
            bool vExists = server->hasVersion(version);
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
                std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
                mcsm::Result res(resp.first, resp.second);
                return res;
            }
            if(!vExists){
                mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverUnsupportedVersion()});
                return res;
            }
            mcsm::ServerOption serverOption(version, server);
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
                std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
                mcsm::Result res(resp.first, resp.second);
                return res;
            }
            return configure(serverOption, name, option, autoUpdate);
        }
        default: {
            mcsm::Result res({mcsm::ResultType::MCSM_WARN, {"This kind of Bukkit-API based server implementation is not supported right now."}});
            return res;
        }
    }
    mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {"Invaild call.", "High chance to be a software issue; Please report this to GitHub (https://github.com/dodoman8067/mcsm)."}});
    return res;
}

mcsm::Result mcsm::server::generateVanilla(const std::string& name, mcsm::JvmOption& option, const std::string& version, const bool& autoUpdate){
    std::shared_ptr<mcsm::VanillaServer> server = std::make_shared<mcsm::VanillaServer>();
    bool vExists = server->hasVersion(version);
    if(!vExists){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverUnsupportedVersion()});
        return res;
    }
    mcsm::ServerOption serverOption(version, server);
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }
    return configure(serverOption, name, option, autoUpdate);
}

mcsm::Result mcsm::server::generateForge(const std::string& /* name */, mcsm::JvmOption& /* option */, const std::string& /* version */, const bool& /* autoUpdate */){
    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
    return res;
}

mcsm::Result mcsm::server::generateFabric(const std::string& name, mcsm::JvmOption& option, const std::string& version, const bool& autoUpdate){
    std::shared_ptr<mcsm::FabricServer> server = std::make_shared<mcsm::FabricServer>();
    bool vExists = server->hasVersion(version);
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }
    if(!vExists){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverUnsupportedVersion()});
        return res;
    }
    mcsm::FabricServerOption serverOption(version, server);
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }
    bool sExists = serverOption.exists();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }
    if(sExists){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverAlreadyConfigured()});
        return res;
    }
    mcsm::Result sRes = serverOption.create(name, option, autoUpdate);
    if(!sRes.isSuccess()) return sRes;

    std::string sName = serverOption.getServerName();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    std::string type = serverOption.getServerType();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    std::string sVersion = serverOption.getServerVersion();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    std::string profile = serverOption.getDefaultOption()->getProfileName();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    mcsm::success("Configured server's information : ");
    mcsm::info("Server name : " + sName);
    mcsm::info("Server type : " + type);
    mcsm::info("Server version : " + sVersion);
    mcsm::info("Server JVM launch profile : " + profile);
    if(!autoUpdate) mcsm::info("Automatic updates : disabled");

    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
    return res;
}

mcsm::Result mcsm::server::generateSponge(const std::string& /* name */, mcsm::JvmOption& /* option */, const std::string& /* version */, const bool& /* autoUpdate */){
    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
    return res;
}

mcsm::Result mcsm::server::generateCustom(const std::string& /* name */, mcsm::JvmOption& /* option */, const std::string& /* filePath */, const std::string& /* version */, const bool& /* autoUpdate */){
    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
    return res;
}

mcsm::Result mcsm::server::configure(mcsm::ServerOption& serverOption, const std::string& name, mcsm::JvmOption& option, const bool& autoUpdate){
    bool sExists = serverOption.exists();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }
    if(sExists){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::serverAlreadyConfigured()});
        return res;
    }
    mcsm::Result sRes = serverOption.create(name, option, autoUpdate);
    if(!sRes.isSuccess()) return sRes;

    std::string sName = serverOption.getServerName();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    std::string type = serverOption.getServerType();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    std::string sVersion = serverOption.getServerVersion();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    std::string profile = serverOption.getDefaultOption()->getProfileName();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    mcsm::success("Configured server's information : ");
    mcsm::info("Server name : " + sName);
    mcsm::info("Server type : " + type);
    mcsm::info("Server version : " + sVersion);
    mcsm::info("Server JVM launch profile : " + profile);
    if(!autoUpdate) mcsm::info("Automatic updates : disabled");

    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
    return res;
}

std::shared_ptr<mcsm::Server> mcsm::server::detectServerType(const std::string& server){
    std::shared_ptr<mcsm::Server> sPtr = mcsm::ServerRegistry::getServerRegistry().getServer(server);
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS) return nullptr;

    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
    return sPtr;
}