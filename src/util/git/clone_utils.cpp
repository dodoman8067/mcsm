#include <mcsm/util/git/clone_utils.h>

mcsm::Result mcsm::cloneRepo(const std::string& url){
    return cloneRepo(url, std::filesystem::current_path().string() + "/clone");
}

mcsm::Result mcsm::cloneRepo(const std::string& url, const std::string& path){
    git_libgit2_init();
    git_repository *repo = nullptr;
    int error = git_clone(&repo, url.c_str(), path.c_str(), NULL);
	if(error != 0){
        mcsm::Result res(mcsm::ResultType::MCSM_FAIL, {"Failed to clone a git repository " + url + " with the following reason : " + git_error_last()->message});
        git_repository_free(repo);
        git_libgit2_shutdown();
		return res;
	}
    mcsm::Result res(mcsm::ResultType::MCSM_SUCCESS, {"Success"});
    git_repository_free(repo);
    git_libgit2_shutdown();
    return res;
}