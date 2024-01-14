#include <mcsm/plugin/bukkit/bukkit_plugin.h>

mcsm::BukkitPlugin::BukkitPlugin(const std::string& name, const mcsm::PluginRepo& repo) : BukkitPlugin(name, repo, 1) {}

mcsm::BukkitPlugin::BukkitPlugin(const std::string& name, const mcsm::PluginRepo& repo, const int& result){
    this->name = name;
    this->repo = repo;
    this->result = result;
    init();
}

mcsm::BukkitPlugin::~BukkitPlugin(){

}

void mcsm::BukkitPlugin::init(){
    this->found = true;
    if(this->repo == mcsm::PluginRepo::SPIGET){
        std::string spigetres = mcsm::get("https://api.spiget.org/v2/search/resources/" + this->name + "?field=name&size=" + std::to_string(result));
        nlohmann::json json = nlohmann::json::parse(spigetres, nullptr, false);
        if(json.is_discarded()){
            if(!mcsm::isDebug()){ this->found = false; }
            mcsm::error("Parse of json or API request failed.");
            mcsm::error("If you believe that this is a software issue, please report it to GitHub. (https://github.com/dodoman8067/mcsm)");
            this->found = false;
        }
        this->author = spigetGetAuthor();

    }
}

std::string mcsm::BukkitPlugin::spigetGetCreatedAt(){}

std::string mcsm::BukkitPlugin::spigetGetDownloads(){}

std::string mcsm::BukkitPlugin::spigetGetNativeVersion(){}

std::vector<std::string> mcsm::BukkitPlugin::spigetGetSupportedVersions(){}

std::string mcsm::BukkitPlugin::spigetGetAuthor(){
    std::string spigetres = mcsm::get("https://api.spiget.org/v2/search/resources/" + this->name + "?field=name&size=" + std::to_string(this->result));
    nlohmann::json json = nlohmann::json::parse(spigetres, nullptr, false);
    if(json.is_discarded()){
        if(!mcsm::isDebug()){ this->found = false; return ""; }
        mcsm::error("Parse of json or API request failed.");
        mcsm::error("If you believe that this is a software issue, please report it to GitHub. (https://github.com/dodoman8067/mcsm)");
        this->found = false;
        return "";
    }

    int i = 0;

    if((this->result - 1) < 0){
        i = 0;
    }else{
        i = this->result - 1;
    }
    nlohmann::json plugin = json[i];
    
    if(plugin == nullptr){
        if(!mcsm::isDebug()){ this->found = false; return ""; }
        mcsm::warning("No result was found.");
        this->found = false;
        return "";
    }

    if(plugin["author"] == nullptr){
        if(!mcsm::isDebug()){ this->found = false; return ""; }
        mcsm::error("API request failed. (Failed to get author)");
        mcsm::error("If you believe that this is a software issue, please report it to GitHub. (https://github.com/dodoman8067/mcsm)");
        this->found = false;
        return "";
    }
    
    nlohmann::json author = plugin["author"];

    if(!author.is_object()){
        if(!mcsm::isDebug()){ this->found = false; return ""; }
        mcsm::error("API request failed. (Failed to get author id number)");
        mcsm::error("If you believe that this is a software issue, please report it to GitHub. (https://github.com/dodoman8067/mcsm)");
        this->found = false;
        return "";
    }

    if(author["id"] == nullptr){
        if(!mcsm::isDebug()){ this->found = false; return ""; }
        mcsm::error("API request failed. (Failed to get author id number)");
        mcsm::error("If you believe that this is a software issue, please report it to GitHub. (https://github.com/dodoman8067/mcsm)");
        this->found = false;
        return "";
    }

    if(!author["id"].is_number_integer()){
        if(!mcsm::isDebug()){ this->found = false; return ""; }
        mcsm::error("API request failed. (Failed to get author id number)");
        mcsm::error("If you believe that this is a software issue, please report it to GitHub. (https://github.com/dodoman8067/mcsm)");
        this->found = false;
        return "";
    }

    int authorNum = author["id"];

    std::string authorRes = mcsm::get("https://api.spiget.org/v2/authors/" + std::to_string(authorNum));
    nlohmann::json authorJson = nlohmann::json::parse(authorRes, nullptr, false);
    if(json.is_discarded()){
        if(!mcsm::isDebug()){ this->found = false; return ""; }
        mcsm::error("Parse of json or API request failed.");
        mcsm::error("If you believe that this is a software issue, please report it to GitHub. (https://github.com/dodoman8067/mcsm)");
        this->found = false;
        return "";
    }

    if(authorJson["error"] != nullptr){
        if(!mcsm::isDebug()){ this->found = false; return ""; }
        std::string authorReason = authorJson["error"];
        mcsm::error("API request failed. Reason : " + authorReason);
        mcsm::error("If you believe that this is a software issue, please report it to GitHub. (https://github.com/dodoman8067/mcsm)");
        this->found = false;
        return "";
    }

    if(authorJson["name"] == nullptr){
        if(!mcsm::isDebug()){ this->found = false; return ""; }
        mcsm::error("API request failed. (Failed to get author name)");
        mcsm::error("If you believe that this is a software issue, please report it to GitHub. (https://github.com/dodoman8067/mcsm)");
        this->found = false;
        return "";
    }

    if(!authorJson["name"].is_string()){
        if(!mcsm::isDebug()){ this->found = false; return ""; }
        mcsm::error("API request failed. (Failed to get author name)");
        mcsm::error("If you believe that this is a software issue, please report it to GitHub. (https://github.com/dodoman8067/mcsm)");
        this->found = false;
        return "";
    }

    std::string returnValue = authorJson["name"];
    return returnValue;
}

std::string mcsm::BukkitPlugin::getAuthor() const {
    return this->author;
}

bool mcsm::BukkitPlugin::isFound() const {
    return this->found;
}