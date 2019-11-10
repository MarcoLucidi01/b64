.POSIX:

CC     := cc
CFLAGS := -std=c89 -pedantic -Wall -Wextra -Werror
PREFIX := /usr/local

all: b64.debug

b64.debug: b64.c
	$(CC) $(CFLAGS) -g -DDEBUG $^ -o $@

b64: b64.c
	$(CC) $(CFLAGS) $^ -o $@

test: b64
	sh b64.test

install: b64
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	install -m 755 b64 $(DESTDIR)$(PREFIX)/bin

uninstall:
	rm $(DESTDIR)$(PREFIX)/bin/b64

clean:
	rm -f b64 b64.debug
