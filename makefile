CFLAGS=-Wall -Wextra -Os
PREFIX?=/usr/local

my-git-prompt: my-git-prompt.c

clean:
	rm my-git-prompt

install: my-git-prompt
	@echo :: installing 'my-git-prompt' to $(PREFIX)/bin/
	install -m 755 my-git-prompt $(PREFIX)/bin/my-git-prompt

.PHONY: clean install
