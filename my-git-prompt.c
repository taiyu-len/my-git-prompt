#include <stdio.h>
#include <stdlib.h>
#include <git2.h>

typedef struct {
	unsigned index_any;
	unsigned index_new;
	unsigned wt_modified;
	unsigned wt_deleted;
	unsigned conflict;
} my_status_t;

int my_status_cb(const char* path, unsigned int flags, void* payload) {
	(void) path;
	my_status_t* my_status = payload;
	if (flags & (GIT_STATUS_INDEX_NEW
		| GIT_STATUS_INDEX_DELETED
		| GIT_STATUS_INDEX_MODIFIED
		| GIT_STATUS_INDEX_RENAMED
		| GIT_STATUS_INDEX_TYPECHANGE
		| GIT_STATUS_CONFLICTED))
	{
		my_status->index_any = 1;
	}
	if (flags & GIT_STATUS_INDEX_NEW)
	{
		my_status->index_new++;
	}
	if (flags & GIT_STATUS_WT_MODIFIED)
	{
		my_status->wt_modified++;
	}
	if (flags & GIT_STATUS_WT_DELETED)
	{
		my_status->wt_deleted++;
	}
	if (flags & GIT_STATUS_CONFLICTED)
	{
		my_status->conflict++;
	}
	return 0;
}


int main(int argc, char** argv)
{
	int unborn = 0;
	git_repository *repo = NULL;
	git_reference *ref = NULL;
	const char* branch_name = NULL;
	const char* path = ".";
#define COMMIT_NAME_SIZE 8
	char commit_name_buf[COMMIT_NAME_SIZE];

	if (argc == 2) path = argv[1];

	git_libgit2_init();

	if (git_repository_open_ext(&repo, path, 0, NULL)) {
		return EXIT_SUCCESS;
	}

	my_status_t my_status = {};
	git_status_foreach(repo, &my_status_cb, &my_status);

	switch (git_repository_head(&ref, repo)) {
	case 0: break;
	case GIT_EUNBORNBRANCH:
		unborn = 1;
		break;
	default: return EXIT_SUCCESS;
	}

	if (unborn) {
		branch_name = "(new)";
	} else if (!git_branch_name(&branch_name, ref)) {
		// NOOP: branch_name is set if this is entered
	} else {
		git_annotated_commit* commit;
		git_annotated_commit_from_ref(&commit, repo, ref);
		const git_oid* id = git_annotated_commit_id(commit);
		branch_name = git_oid_tostr(commit_name_buf, COMMIT_NAME_SIZE, id);
	}

#define ZSH_MAGENTA "%%F{m}"
#define ZSH_RED     "%%F{r}"
#define ZSH_GREEN   "%%F{g}"
#define ZSH_BLUE    "%%F{blu}"
#define ZSH_UNCOLOR "%f"
#define ZSH_BOLD    "%B"
#define ZSH_UNBOLD  "%b"
	fputs(ZSH_BOLD, stdout);
	fputc(my_status.index_any ? '+' : ' ', stdout);
	fputs(branch_name, stdout);
	if (my_status.index_new | my_status.wt_modified | my_status.wt_deleted | my_status.conflict)
	{
		fputc(' ', stdout);
		if (my_status.wt_deleted) {
			printf(ZSH_RED "%u",   my_status.wt_deleted);
		}
		if (my_status.conflict) {
			printf(ZSH_MAGENTA "%u", my_status.conflict);
		}
		if (my_status.wt_modified) {
			printf(ZSH_BLUE "%u",  my_status.wt_modified);
		}
		if (my_status.index_new) {
			printf(ZSH_GREEN "%u", my_status.index_new);
		}
		fputs(ZSH_UNCOLOR, stdout);
	}
	fputs(ZSH_UNBOLD, stdout);
}
