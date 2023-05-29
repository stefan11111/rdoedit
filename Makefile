CFLAGS = -Wall -Wpedantic -Wextra -Wl,-z,now
CFLAGS_RELEASE = ${CFLAGS} -O2 -s -D_FORTIFY_SOURCE=2
CFLAGS_DEBUG = ${CFLAGS} -O0 -g -fsanitize=undefined
CFLAGS_STATIC = ${CFLAGS_RELEASE} -static-pie
LIBS = -lcrypt
CC = gcc
PREFIX ?= /usr/local

all: rdoedit.c rdoedit.h
	${CC} ${CFLAGS_RELEASE} rdoedit.c -o rdoedit ${LIBS}

static: rdoedit.c rdoedit.h
	${CC} ${CFLAGS_STATIC} rdoedit.c -o rdoedit ${LIBS}

debug: rdoedit.c rdoedit.h
	${CC} ${CFLAGS_DEBUG} rdoedit.c -o rdoedit ${LIBS}

install: rdoedit
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp rdoedit ${DESTDIR}${PREFIX}/bin
	chown root:root ${DESTDIR}${PREFIX}/bin/rdoedit
	chmod 4755 ${DESTDIR}${PREFIX}/bin/rdoedit

uninstall:
	rm ${DESTDIR}/usr/local/bin/rdoedit

clean:
	rm rdoedit
