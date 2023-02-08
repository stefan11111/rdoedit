#include "rdoedit.h"

static void copy_file(const char *src_file, const char *dest_file) {
    FILE *src, *dest;
    char buffer[4096];
    size_t bytes_read;

    src = fopen(src_file, "rb");
    if (src == NULL) {
        src = fopen(src_file, "wb+");
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

static int modify_file(char *file, char *editor)
{
    copy_file(file, FILENAME);
    if (fork() == 0) {
        if (edit_file(FILENAME, editor)) {
            return remove(FILENAME);
        }
    }
    else {
        wait(NULL);
    }
    struct stat stat_record;
    if(stat(FILENAME, &stat_record)) {
        printf("stat error");
        return -1;
    }
    if(stat_record.st_size <= 1) {
        remove(FILENAME);
        return remove(file);
    }
    copy_file(FILENAME, file);
    return remove(FILENAME);
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

    char *editor = getenv("EDITOR");

    if(!editor) {
        editor = EDITOR;
    }
    if (!ruid && ALLOW_ROOT) {
	edit_file(argv[1], editor);
        return remove(FILENAME);
    }
    if (strcmp(user->pw_name, ALLOWED_USER)) {
	printf("You are not the allowed user.\n");
	return 1;
    }

    if (!REQUIRE_PASSWORD) {
        int i;
        for (i = 1; i < argc; i++) {
            if (modify_file(argv[i], editor)) {
                return 1;
            }
        }
        return 0;
    }
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

    char *hashed = NULL;
    hashed = crypt(pass, shadow->sp_pwdp);
    if (!hashed) {
	printf("Could not hash password, does your user have a password?");
	return 1;
    }

    if (strcmp(hashed, shadow->sp_pwdp)) {
	printf("Wrong password.\n");
	return 1;
    }

    int i;
    for (i = 1; i < argc; i++) {
        if (modify_file(argv[i], editor)) {
            return 1;
        }
    }
    return 0;
}
