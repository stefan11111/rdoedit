#include "rdoedit.h"

static void copy_file(const char *src_file, const char *dest_file) {
    FILE *src, *dest;
    char buffer[BUFSIZE];
    size_t bytes_read;

    src = fopen(src_file, "rb");
    if (src == NULL) {
        src = fopen(src_file, "wb+");
        file_existed = 0;
        if (src == NULL) {
            perror("Error opening source file");
            exit(EXIT_FAILURE);
        }
    }

    dest = fopen(dest_file, "wb");
    if (dest == NULL) {
        perror("Error opening destination file");
        exit(EXIT_FAILURE);
    }

    while ((bytes_read = fread(buffer, 1, sizeof buffer, src)) > 0) {
        if (fwrite(buffer, 1, bytes_read, dest) != bytes_read) {
            perror("Error writing to destination file");
            exit(EXIT_FAILURE);
        }
    }

    fclose(src);
    fclose(dest);
}

static inline int edit_file(const char *file, const char *editor) {
    if (execl(editor, editor, file, (char*)NULL) == -1) {
        printf("Error editing file\n");
        return -1;
    }
    return 0;
}

static void rand_str(char *dest, size_t length) {
    char *charset = CHARSET;
    strcpy(dest, TMPDIR);
    dest += sizeof(TMPDIR) - 1;

    while (length-- > 0) {
        *dest++ = *(charset +(size_t)((double) rand() / RAND_MAX * (sizeof CHARSET - 1)));
    }
    *dest = '\0';
}

static int modify_file(char *file, char *editor) {
    char filename[sizeof(TMPDIR) + LENGTH + 1];
    struct stat statbuf;
    do {
        rand_str(filename, LENGTH);
    }
    while(!stat(filename, &statbuf));
    copy_file(file, filename);
    if (fork() == 0) {
        if (edit_file(filename, editor)) {
            return remove(filename);
        }
    }
    else {
        wait(NULL);
    }
    if(stat(filename, &statbuf)) {
        printf("stat error");
        return -1;
    }
    if(statbuf.st_size <= 1 && !file_existed) {
        remove(filename);
        file_existed = 1;
        return remove(file);
    }
    file_existed = 1;
    copy_file(filename, file);
    return remove(filename);
}

int main(int argc, char** argv) {
    if (geteuid() != 0) {
        printf("The rdoedit binary needs to be installed as SUID.\n");
        return 1;
    }

    if (argc == 1) {
        printf("Usage: %s [files]\n", argv[0]);
        return 0;
    }

    const uid_t ruid = getuid();

    struct passwd *user = getpwuid(ruid);
#ifdef ALLOW_ROOT
    if (strcmp(user->pw_name, ALLOWED_USER) && ruid) {
	printf("You are not the allowed user.\n");
	return 1;
    }
#else
    if (strcmp(user->pw_name, ALLOWED_USER)) {
        printf("You are not the allowed user.\n");
        return 1;
    }
#endif
    srand(time(NULL));
    char *editor = NULL;
#ifdef EXTERN_EDITOR
    editor = getenv("EDITOR");
#endif
    if (!editor) {
        editor = EDITOR;
    }
    if (!ruid) {
        int i;
        for (i = 1; i < argc; i++) {
            if (modify_file(argv[i], editor)) {
                return 1;
            }
        }
    return 0;
    }
#ifdef REQUIRE_PASSWORD
    char pass[PWD_MAX + 1];
    struct termios term;
    tcgetattr(1, &term);
    term.c_lflag &= ~ECHO;
    tcsetattr(1, 0, &term);
    term.c_lflag |= ECHO;
    if (write(1, "Enter the password: ", 20) < 0) {
	tcsetattr(1, 0, &term);
    }
    if (scanf("%200s", pass) != 1) {
	printf("Error reading password.\n");
	tcsetattr(1, 0, &term);
	return 0;
    }
    tcsetattr(1, 0, &term);
    printf("\n");

    struct spwd* shadow = getspnam(user->pw_name);

    if (!shadow || !shadow->sp_pwdp) {
        printf("Could not get shadow entry.\n");
	return 1;
    }

    char *hashed = hashed = crypt(pass, shadow->sp_pwdp);
#ifdef HARDENED
    memset(pass, 0, sizeof(pass));
#endif
    if (!hashed) {
	printf("Could not hash password, does your user have a password?");
	return 1;
    }

    if (strcmp(hashed, shadow->sp_pwdp)) {
	printf("Wrong password.\n");
	return 1;
    }
#endif
    int i;
    for (i = 1; i < argc; i++) {
        if (modify_file(argv[i], editor)) {
            return 1;
        }
    }
    return 0;
}
