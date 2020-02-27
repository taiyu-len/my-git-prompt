CFLAGS=-Wall -Wextra -Os
LDLIBS=-lgit2
PREFIX?=/usr/local

src=my-git-prompt.c
tgt=$(src:.c=)

all: $(tgt)
clean:
	rm $(tgt)

install: $(PREFIX)/bin/$(tgt)
$(PREFIX)/bin/% : %
	@echo :: installing '$^' to $(PREFIX)/bin/
	install -Tm 755 "$^" "$@"

.PHONY: clean install
