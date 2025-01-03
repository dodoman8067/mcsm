#include <mcsm/data/options/running_sessions_option.h>
#include <mcsm/data/options/general_option.h>

mcsm::RunningSessionsOption::RunningSessionsOption(){

}

mcsm::RunningSessionsOption::~RunningSessionsOption(){
    
}

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

    //need to implement importing runninggroup from json. this class is designed to have multiple instance created
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

    }
}

std::vector<const mcsm::RunningGroup*> mcsm::RunningSessionsOption::getRunningGroups() const {

}