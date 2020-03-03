#include <stdio.h>
#include <stdlib.h>
#include <git2.h>

#define PRINTIF(A) if (flags & A) { printf("  " #A "\n"); }

int my_status_cb(const char* path, unsigned int flags, void* payload) {
	(void) payload;
	printf("%s:\n", path);
	PRINTIF(GIT_STATUS_CURRENT);
	PRINTIF(GIT_STATUS_INDEX_NEW);
	PRINTIF(GIT_STATUS_INDEX_MODIFIED);
	PRINTIF(GIT_STATUS_INDEX_DELETED);
	PRINTIF(GIT_STATUS_INDEX_RENAMED);
	PRINTIF(GIT_STATUS_INDEX_TYPECHANGE);
	PRINTIF(GIT_STATUS_WT_NEW);
	PRINTIF(GIT_STATUS_WT_MODIFIED);
	PRINTIF(GIT_STATUS_WT_DELETED);
	PRINTIF(GIT_STATUS_WT_TYPECHANGE);
	PRINTIF(GIT_STATUS_WT_RENAMED);
	PRINTIF(GIT_STATUS_WT_UNREADABLE);
	PRINTIF(GIT_STATUS_IGNORED);
	PRINTIF(GIT_STATUS_CONFLICTED);
	return 0;
}

void git_perror(const char* fn) {
	printf("%s: %s\n", fn, git_error_last()->message);
}

int main(int argc, char **argv)
{
	int unborn = 0;
	git_repository *repo = NULL;
	git_reference *ref = NULL;
	const char* path = ".";
	const char* branch_name = NULL;

	if (argc == 2) path = argv[1];

	git_libgit2_init();

	if (git_repository_open_ext(&repo, path, 0, NULL)) {
		git_perror("repository_open_ext");
		return 1;
	}

	switch (git_repository_head(&ref, repo)) {
	case 0: break;
	case GIT_EUNBORNBRANCH:
		unborn = 1;
		break;
	default:
		git_perror("repository_head");
		return 1;
	}

	if (unborn) {
		printf("## empty branch\n");
	} else {
		if (git_branch_name(&branch_name, ref)) {
			git_perror("branch_name");
			return 1;
		}
		printf("## %s\n", branch_name);
	}

	git_status_foreach(repo, &my_status_cb, NULL);
	return EXIT_SUCCESS;
}
