CFLAGS = -Wall -Wpedantic -Werror -Wextra -Wl,-z,now
CFLAGS_RELEASE = ${CFLAGS} -O2 -s -D_FORTIFY_SOURCE=2
CFLAGS_DEBUG = ${CFLAGS} -O0 -g -fsanitize=undefined
CFLAGS_STATIC = ${CFLAGS_RELEASE} -static-pie
LIBS = -lcrypt
CC = gcc

all: rdoedit.c rdoedit.h
	${CC} ${CFLAGS_RELEASE} rdoedit.c -o rdoedit ${LIBS}

static: rdoedit.c rdoedit.h
	${CC} ${CFLAGS_STATIC} rdoedit.c -o rdoedit ${LIBS}

debug: rdoedit.c rdoedit.h
	${CC} ${CFLAGS_DEBUG} rdoedit.c -o rdoedit ${LIBS}

install: rdoedit
	cp rdoedit ${DESTDIR}/usr/local/bin
	chown root:root ${DESTDIR}/usr/local/bin/rdoedit
	chmod 4755 ${DESTDIR}/usr/local/bin/rdoedit

uninstall:
	rm ${DESTDIR}/usr/local/bin/rdoedit

clean:
	rm rdoedit
