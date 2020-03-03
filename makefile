CFLAGS=-Wall -Wextra -Os
LDLIBS=-lgit2
PREFIX?=/usr/local

src=my-git-prompt.c
tgt=$(src:.c=)

all: $(tgt)
show-git-status: show-git-status.c
clean:
	rm $(tgt)

install: $(PREFIX)/bin/$(tgt)
$(PREFIX)/bin/% : %
	@echo :: installing '$^' to $(PREFIX)/bin/
	install -Tm 755 "$^" "$@"

.PHONY: clean install
