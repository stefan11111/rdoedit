#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <string.h>
#include <shadow.h>
#include <termios.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define PWD_MAX 200
#define FNM_MAX 512
#define ALLOWED_USER "stefan"		/*the allowed user*/
#define FILENAME "aaaabbbbccccdddd"	/*temporary filename*/
#define EDITOR "/bin/nano"		/*fallback editor*/
#define REQUIRE_PASSWORD 1		/*require password for the allowed user*/
					/*1 is true, 0 is false*/
#define ALLOW_ROOT 1			/*allow the root user to use rdo*/
					/*1 is true, 0 is false*/
