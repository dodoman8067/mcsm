#include <mcsm/util/git/clone_utils.h>

void mcsm::cloneRepo(const std::string& url){
    cloneRepo(url, std::filesystem::current_path().string() + "clone");
}

void mcsm::cloneRepo(const std::string& url, const std::string& path){
    git_libgit2_init();
    git_repository *repo = nullptr;
    int error = git_clone(&repo, url.c_str(), path.c_str(), NULL);
	if(error < 0){
        mcsm::error("Failed to clone git repository " + url + " with the following error : " + git_error_last()->message);
        git_repository_free(repo);
        git_libgit2_shutdown();
		exit(1);
	}
    git_repository_free(repo);
    git_libgit2_shutdown();
}