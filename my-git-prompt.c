#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* get_name(const char* string);
char get_commit_status(char state);

int main(void)
{
	size_t n = 0;
	ssize_t sz = 0;
	char *lineptr = NULL;
	const char *branch_name = NULL;
	char commit_status = '\0';

	size_t staged[256] = {};
	size_t unstaged[256]  = {};

	while ((sz = getline(&lineptr, &n, stdin)) != EOF)
	{
		if (sz > 0) lineptr[sz - 1] = '\0';
		switch (lineptr[0])
		{
		case '#': {
			if (! branch_name) {
				const char* name = get_name(lineptr);
				if (name) {
					branch_name = name;
				}
			}
			break;
		}
		case '?': break;
		case '!': break;
		case '1':
		case '2': {
			if (sz <= 4) continue;
			staged[(unsigned)lineptr[2]]++;
			unstaged[(unsigned)lineptr[3]]++;
			if (! commit_status)
			{
				commit_status = get_commit_status(lineptr[2]);
			}
		}
		}
	}
#define ZSH_RED     "%%F{r}"
#define ZSH_GREEN   "%%F{g}"
#define ZSH_BLUE    "%%F{blu}"
#define ZSH_UNCOLOR "%%f"
#define ZSH_BOLD    "%%B"
#define ZSH_UNBOLD  "%%b"
	if (! branch_name)
	{
		return EXIT_SUCCESS;
	}

	printf(ZSH_BOLD);
	printf("%c", commit_status ? commit_status : ' ');
	printf("%s", branch_name);
	int status = unstaged['D'] || unstaged['M'] || staged['A'];
	if (status) printf(" ");
	if (unstaged['D']) printf(ZSH_RED   "%ld", unstaged['D']);
	if (unstaged['M']) printf(ZSH_BLUE  "%ld", unstaged['M']);
	if (  staged['A']) printf(ZSH_GREEN "%ld",   staged['A']);
	if (status) printf(ZSH_UNCOLOR);
	printf(ZSH_UNBOLD);

	return EXIT_SUCCESS;
}

const char* get_name(const char* string)
{
	const char *s = "# branch.head ";
	if (strncmp(string, s, strlen(s)) == 0) {
		return strdup(string + strlen(s));
	}
	return NULL;
}

char get_commit_status(char state)
{
	switch (state)
	{
	case '.':
	case '!':
	case '?': return '\0';
	default: return  '+';
	}
}
