#include <mcsm/util/git/clone_utils.h>

mcsm::VoidResult mcsm::cloneRepo(const std::string& url){
    return cloneRepo(url, std::filesystem::current_path().string() + "/clone");
}

mcsm::VoidResult mcsm::cloneRepo(const std::string& url, const std::string& path){
    git_libgit2_init();
    git_repository *repo = nullptr;
    int error = git_clone(&repo, url.c_str(), path.c_str(), NULL);
	if(error != 0){
        mcsm::Error err = mcsm::makeError(mcsm::ErrorStatus::ERROR, mcsm::errors::GIT_CLONE_FAILED, {url, git_error_last()->message});
        git_repository_free(repo);
        git_libgit2_shutdown();
		return tl::unexpected(err);
	}
    git_repository_free(repo);
    git_libgit2_shutdown();
    return {};
}