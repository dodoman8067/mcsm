#include <mcsm/data/options/running_sessions_option.h>
#include <mcsm/data/options/general_option.h>

mcsm::RunningSessionsOption::RunningSessionsOption(){

}

mcsm::RunningSessionsOption::~RunningSessionsOption(){
    
}


/** example json format
 * running_sessions_do_not_edit_this.json
 * {
 *     "running": [
 *         {
 *             "group_path": "/group/path1",
 *             "running_servers": [ "/path/to/server", "path/to/server2" ]
 *         },
 *         {
 *             "group_path": "/group/path2",
 *             "running_servers": [ "/path/to/server3", "path/to/server4" ]
 *         }
 *     ]
 * }
 * 
*/
mcsm::Result mcsm::RunningSessionsOption::load(){
    this->handle = std::make_unique<mcsm::GlobalOption>("", "running_sessions_do_not_edit_this");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    bool advp = mcsm::GeneralOption::getGeneralOption().advancedParseEnabled();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    mcsm::Result lRes = this->handle->load(advp);
    if(!lRes.isSuccess()) return lRes;

    nlohmann::json arr = this->handle->getValue("running");
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }

    if(arr == nullptr){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFound("\"running\"", this->handle->getName())});
        return res;
    }
    if(!arr.is_array()){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"running\"", "array of objects")});
        return res;
    }

    //need to implement importing runninggroup from json. this class is designed to have multiple instance created(not to do)
    for(nlohmann::json& v : arr){
        if(!v.is_object()){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"running\"", "object")});
            return res;
        }
        std::unique_ptr<mcsm::RunningGroup> rgp = std::make_unique<mcsm::RunningGroup>();

        nlohmann::json groupPath = v["group_path"];
        if(groupPath == nullptr){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFound("\"group_path\"", this->handle->getName())});
            return res;
        }
        if(!groupPath.is_string()){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"group_path\"", "string")});
            return res;
        }

        std::unique_ptr<mcsm::ServerGroupLoader> sgl = std::make_unique<mcsm::ServerGroupLoader>(std::string(groupPath));
        rgp->group = std::move(sgl);

        nlohmann::json rs = v["running_servers"];
        if(rs == nullptr){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonNotFound("\"running_servers\"", this->handle->getName())});
            return res;
        }
        if(!rs.is_array()){
            mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"running_servers\"", "array string")});
            return res;
        }

        for(nlohmann::json& rsv : rs){
            if(!rsv.is_string()){
                mcsm::Result res({mcsm::ResultType::MCSM_FAIL, mcsm::message_utils::jsonWrongType("\"running_servers\"", "string")});
                return res;
            }
            std::unique_ptr<mcsm::ServerConfigLoader> scl = std::make_unique<mcsm::ServerConfigLoader>(std::string(rsv));

            mcsm::Result sclLRes = scl->loadConfig();
            if(!sclLRes.isSuccess()){
                rgp->running.clear(); 
                return sclLRes;
            }

            rgp->running.push_back(std::move(scl));
        }
        this->runningGroups.push_back(std::move(rgp));
    }
    return {mcsm::ResultType::MCSM_SUCCESS, {"Success"}}; // temp
}

mcsm::Result mcsm::RunningSessionsOption::save(){
    return {mcsm::ResultType::MCSM_SUCCESS, {"Success"}};
}

std::vector<const mcsm::RunningGroup*> mcsm::RunningSessionsOption::getRunningGroups() const {
    std::vector<const mcsm::RunningGroup*> rtv;
    for(auto& v : this->runningGroups){
        if(v){
            rtv.push_back(v.get());
        }
    }
    return rtv;
}

std::vector<const mcsm::ServerConfigLoader*> mcsm::RunningSessionsOption::getRunningServersOfGroup(const std::string& groupName) const {
    std::vector<const mcsm::ServerConfigLoader*> rtv;
    mcsm::Result res({mcsm::ResultType::MCSM_SUCCESS, {"Success"}});
    bool success = false;

    for(const auto& group : this->runningGroups){
        if(group->group->getName() != groupName) continue;

        for(const auto& server : group->running){
            if(server){
                rtv.push_back(server.get());
                success = true;
            }else{
                mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {"Null serverconfigloader pointer detected."}});
                success = false;
                break;
            }
        }
    }

    if(!success){
        mcsm::Result res({mcsm::ResultType::MCSM_FAIL, {"No valid running group \"" + groupName + "\" found."}});
    }
    return rtv;
}

mcsm::Result mcsm::RunningSessionsOption::addRunningGroup(std::unique_ptr<mcsm::RunningGroup> group){
    for(auto& g : this->runningGroups){
        std::string rgn = g->group->getName();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
            std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
            mcsm::Result res(resp.first, resp.second);
            return res;
        }
        std::string ggn = group->group->getName();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
            std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
            mcsm::Result res(resp.first, resp.second);
            return res;
        }
        if(rgn == ggn){
            return {mcsm::ResultType::MCSM_FAIL, {"Group already running: " + ggn}};
        }
    }
    this->runningGroups.push_back(std::move(group));
    return this->save();
}

mcsm::Result mcsm::RunningSessionsOption::addRunningServer(const std::string& groupName, const mcsm::ServerConfigLoader* server){
    std::string serverName = server->getServerName();
    if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
        std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
        mcsm::Result res(resp.first, resp.second);
        return res;
    }
    
    for(auto& g : this->runningGroups){
        std::string gn = g->group->getName();
        if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
            std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
            mcsm::Result res(resp.first, resp.second);
            return res;
        }
        if(gn == groupName){
            std::vector<const mcsm::ServerConfigLoader*> rsg = getRunningServersOfGroup(groupName);
            if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
                std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
                mcsm::Result res(resp.first, resp.second);
                return res;
            }

            for(auto& v : rsg){
                std::string sn = v->getServerName();
                if(mcsm::getLastResult().first != mcsm::ResultType::MCSM_OK && mcsm::getLastResult().first != mcsm::ResultType::MCSM_SUCCESS){
                    std::pair<mcsm::ResultType, std::vector<std::string>> resp = mcsm::getLastResult();
                    mcsm::Result res(resp.first, resp.second);
                    return res;
                }

                if(sn == serverName){
                    return {mcsm::ResultType::MCSM_FAIL, {"Same server \"" + serverName + "\" in group \"" + groupName + "\" already running"}};
                }
            }

            g->running.push_back(std::make_unique<mcsm::ServerConfigLoader>(*server));
            return this->save();
        }
    }
    return {mcsm::ResultType::MCSM_FAIL, {"No running group \"" + groupName + "\" found"}};
}

// this doesn't actually stop the server, call ServerGroupManager#stop instead
mcsm::Result mcsm::RunningSessionsOption::removeRunningGroup(std::unique_ptr<mcsm::RunningGroup> /* group */){
    return {mcsm::ResultType::MCSM_SUCCESS, {"Success"}};
}

// this doesn't actually stop the server, call ServerGroupManager#stop instead
mcsm::Result mcsm::RunningSessionsOption::removeRunningServer(const std::string& /* groupName */, const mcsm::ServerConfigLoader* /* server */){
    return {mcsm::ResultType::MCSM_SUCCESS, {"Success"}};
}