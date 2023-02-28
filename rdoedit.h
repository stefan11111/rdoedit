#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <string.h>
#include <shadow.h>
#include <termios.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <time.h>

#define PWD_MAX 200
#define FNM_MAX 512
#define ALLOWED_USER "stefan"		/*the allowed user*/
#define EDITOR "/bin/nano"		/*fallback editor*/
#define TMPDIR "/tmp/"			/*directory to store temporary files*/
/*#define EXTERN_EDITOR*/		/*uncomment to set editor using $EDITOR envvar*/
#define LENGTH 16			/*temporary file length*/
#define BUFSIZE 4096			/*file buffer size, default is 4k*/
#define REQUIRE_PASSWORD		/*comment to not require password for the allowed user*/
#define ALLOW_ROOT			/*comment to not allow the root user to use rdoedit*/
#define CHARSET "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"	/*charset for temporary filename*/
/* GLOBALS */

static char file_existed = 1;
