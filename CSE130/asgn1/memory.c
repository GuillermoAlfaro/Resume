#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

int fileNameGetter(char *buf, char *chunk, int pos) {
    int i = 0;
    while (buf[pos] != '\n') {
        if (buf[pos] == '/') {
            return -1;
        }
        if (buf[pos] == '\0') {
            return -1;
        }
        if (i > 255) {
            return -1;
        }
        if (buf[pos] == ' ') {
            return -1;
        }

        chunk[i] = buf[pos];
        pos++;
        i++;
    }

    i++;
    chunk[i] = '\0';

    return i;
}

int setText(char *buf, char *chunk, int pos) {

    int i = 0;
    while (buf[pos] != '\0') {
        chunk[i] = buf[pos];
        pos++;
        i++;
    }
    return i;
}

int fileDumper(char *name) {
    char buf[5000] = { '\0' };
    struct stat path;
    stat(name, &path);
    if (S_ISREG(path.st_mode) == 0) {
        return -1;
    }
    int fd = open(name, O_RDONLY);
    if (fd == -1) {
        return -1;
    }

    int bytes_read = read(fd, buf, 5000);

    while (1) {

        if (bytes_read == 5000) {
            int bytes_written = 0;
            while (bytes_written < 5000) {
                int bytes = write(STDOUT_FILENO, buf + bytes_written, sizeof(buf) - bytes_written);
                bytes_written += bytes;
            }

            bytes_read = 0;
        }
        int saved = bytes_read;
        bytes_read += read(fd, buf + bytes_read, sizeof(buf) - bytes_read);

        if (saved == bytes_read) {
            int bytes_written = 0;
            while (bytes_written < bytes_read) {
                int bytes = write(STDOUT_FILENO, buf + bytes_written, bytes_read - bytes_written);
                bytes_written += bytes;
            }

            break;
        }
    }

    close(fd);

    return 0;
}

int main() {
    char buf[5000] = { '\0' };
    char fileNa[5000] = { '\0' };
    char text[5000] = { '\0' };
    int bytes_read = read(0, buf, sizeof(buf));
    while (bytes_read > 0) {
        int saved = bytes_read;
        bytes_read += read(0, buf + bytes_read, sizeof(buf) - bytes_read);
        if (bytes_read == 5000) {
            break;
        }
        if (saved == bytes_read) {
            break;
        }
    }
    if (buf[0] == 'g' && buf[1] == 'e' && buf[2] == 't' && buf[3] == ' ') {
        int pos = fileNameGetter(buf, fileNa, 4);
        if (pos < 0) {
            fprintf(stderr, "Invalid Command\n");
            return 1;
        }
        int textLen = setText(buf, text, pos + 5);

        if (textLen > 0 && text[0] != '\n') {
            fprintf(stderr, "Invalid Command\n");
            return 1;
        }

        if (fileDumper(fileNa) == -1) {
            fprintf(stderr, "Invalid Command\n");
            return 1;
        }

    } else if (buf[0] == 's' && buf[1] == 'e' && buf[2] == 't' && buf[3] == ' ') {
        int pos = fileNameGetter(buf, fileNa, 4) + 5;
        if (pos - 5 < 0) {
            fprintf(stderr, "Invalid Command\n");
            return 1;
        }

        remove(fileNa);
        int fd = open(fileNa, O_WRONLY | O_CREAT, 00777);

        write(fd, buf + pos - 1, bytes_read - pos + 1);
        pos = 0;
        while (1) {
            bytes_read = read(0, buf, sizeof(buf));
            if (bytes_read == 0) {
                break;
            }
            write(fd, buf, bytes_read);
        }

        close(fd);
        fprintf(stdout, "OK\n");
        return 0;
    } else {
        fprintf(stderr, "Invalid Command\n");
        return 1;
    }

    return 0;
}
