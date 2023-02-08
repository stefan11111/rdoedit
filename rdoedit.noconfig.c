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

    char *editor = getenv("EDITOR");

    if(!editor) {
        editor = EDITOR;
    }
    int i;
    for (i = 1; i < argc; i++) {
        if (modify_file(argv[i], editor)) {
            return 1;
        }
    }
    return 0;
}
