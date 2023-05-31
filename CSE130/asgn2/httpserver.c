#include "asgn2_helper_funcs.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <regex.h>
#include <assert.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

#define MAX_LEN 2048
// I need to add another regex for the rest of the field

#define RE_COMMAND  "([a-zA-Z]{1,8})"
#define RE_FILE     "([a-zA-Z0-9._]{1,64})"
#define RE_HTTP     "([a-zA-Z0-9/._]{1,64})"
#define ContentFind "^Content-Length: [0-9]*"
#define PARSE_REGEX                                                                                \
    "^([a-zA-Z]{1,8}) /([a-zA-Z0-9._]{1,64}) ([a-zA-Z0-9/._]{1,64})\r\n\r?\n?(Content-Length: "    \
    "[0-9]*)?(\r\n\r\n)?"

typedef struct {

    char buf[MAX_LEN + 1];
    char bufCmp[MAX_LEN + 1];
    char *ptr;
    uint16_t bufsize;
    char *command;
    char *location;
    char *http;
    char *contentLen;
    bool func;
    int status;
    int contentNum;
    char *statusText;
    int len;
    int importantNum;
    bool noContent;

} Command;

static void debug(Command *c) {
    fprintf(stderr, "Command:       %s\n", c->command);
    fprintf(stderr, "Location:      %s\n", c->location);
    fprintf(stderr, "HTTP:          %s\n", c->http);
    fprintf(stderr, "ContentLen:    %s\n", c->contentLen);
    fprintf(stderr, "Bufsize        %d\n", c->bufsize);
    fprintf(stderr, "Func:          %d\n", c->func);
    fprintf(stderr, "Status:        %d\n", c->status);
    fprintf(stderr, "Status Text:   %s\n", c->statusText);
    fprintf(stderr, "ImportantNum:  %d\n", c->importantNum);
    fprintf(stderr, "ContentNum:    %d\n", c->contentNum);
}

int fileExist(Command *c) {
    struct stat path;
    char *name = c->location;
    stat(name, &path);
    if (S_ISDIR(path.st_mode) != 0) {
        c->status = 403;
        return 1;
    }

    if (S_ISREG(path.st_mode) == 0) {

        c->status = 404;
        return 1;
    } else {
        c->len = path.st_size;
    }
    return 0;
}

static void protocols(Command *c) {
    int code = c->status;

    switch (code) {
    case 200: c->statusText = "OK"; break;
    case 201:
        c->statusText = "Created";
        c->len = 8;
        break;
    case 400:
        c->statusText = "Bad Request";
        c->len = 12;
        break;
    case 403:
        c->statusText = "Forbidden";
        c->len = 10;
        break;
    case 404:
        c->statusText = "Not Found";
        c->len = 10;
        break;
    case 500:
        c->statusText = "Internal Server Error";
        c->len = 22;
        break;
    case 501:
        c->statusText = "Not Implemented";
        c->len = 16;
        break;
    case 505:
        c->statusText = "Version Not Supported";
        c->len = 22;
        break;
    default:
        c->statusText = "BRO BAD SHIT WENT DOWN";
        c->len = 23;
        break;
    }
}

static void header(Command *c, int fd) {
    char buffer[MAX_LEN];
    int pos;
    if ((c->status == 200) && c->func) {
        pos = sprintf(buffer, "HTTP/1.1 %d %s\r\nContent-Length: %d\r\n\r\n", c->status,
            c->statusText, c->len);
    } else if (c->status == 201) {
        pos = sprintf(buffer, "HTTP/1.1 %d %s\r\nContent-Length: %d\r\n\r\n%s\n", c->status,
            c->statusText, c->len, c->statusText);
    } else {
        pos = sprintf(buffer, "HTTP/1.1 %d %s\r\nContent-Length: %d\r\n\r\n%s\n", c->status,
            c->statusText, c->len, c->statusText);
    }
    write(fd, buffer, pos);
}

static void get(Command *c, int fdd) {
    char buf[5000] = { '\0' };
    char *name = c->location;

    if (fileExist(c) == 1) {
        protocols(c);
        header(c, fdd);
        return;
    }

    int fd = open(name, O_RDONLY);
    if (fd == -1) {
        c->status = 403;
        protocols(c);
        header(c, fdd);
        return;
    }

    c->status = 200;
    protocols(c);
    header(c, fdd); // We got len of file, so we fulfil info for beginning of header

    int bytes_read = read(fd, buf, 5000);

    while (1) {
        if (bytes_read == 5000) {
            int bytes_written = 0;
            while (bytes_written < 5000) {
                int bytes = write(fdd, buf + bytes_written, sizeof(buf) - bytes_written);
                bytes_written += bytes;
            }

            bytes_read = 0;
        }
        int saved = bytes_read;
        bytes_read += read(fd, buf + bytes_read, sizeof(buf) - bytes_read);

        if (saved == bytes_read) {
            int bytes_written = 0;
            while (bytes_written < bytes_read) {
                int bytes = write(fdd, buf + bytes_written, bytes_read - bytes_written);
                bytes_written += bytes;
            }
            break;
        }
    }
    close(fd);
}

static void put(Command *c, int fdd) {
    char *fileNa = c->location;

    if (fileExist(c) == 1) {
        c->status = 201;
        protocols(c);
        header(c, fdd);
    } else {
        c->status = 200;
        protocols(c);
        header(c, fdd);
    }

    int fd = open(fileNa, O_WRONLY | O_CREAT | O_TRUNC, 00777);
    if (c->noContent) {
        fprintf(stderr, "NO CONTENT\n");
        close(fd);
        return;
    }
    if (fd == -1) {
        fprintf(stderr, "OPEN() errno:   %s\n", strerror(errno));
    }

    pass_bytes(fdd, fd, c->contentNum);

    close(fd);
}

static void parse(Command *c, int fd) {
    regex_t re;
    regex_t re2;
    regmatch_t matches[7];
    regmatch_t matches2[1];
    int rc;
    int rc2;

    c->bufsize = read_until(fd, c->buf, MAX_LEN, "\r\n\r\n");
    c->buf[MAX_LEN] = '\0';
    memcpy(c->bufCmp, c->buf, c->bufsize);
    c->bufCmp[MAX_LEN] = '\0';
    if (c->bufsize > 0) { // If it is not empty
        // Null terminate string
        c->buf[c->bufsize] = '\0';
        c->bufCmp[c->bufsize] = '\0';
        // Compile regex
        rc = regcomp(&re, PARSE_REGEX, REG_EXTENDED);
        assert(!rc);
        rc2 = regcomp(&re2, ContentFind, REG_EXTENDED | REG_NEWLINE);
        assert(!rc2);
        printf("%s", c->buf);
        rc = regexec(&re, c->buf, 7, matches, 0);
        rc2 = regexec(&re2, c->buf, 1, matches2, 0);
        if (rc == REG_NOMATCH) {
            printf("No match found\n");
        } else if (rc == REG_ESPACE) {
            printf("Out of memory error\n");
        } else if (rc == REG_ERANGE) {
            printf("Invalid match array\n");
        } else if (rc == REG_BADPAT) {
            printf("Invalid regular expression pattern\n");
        } else if (rc == REG_EBRACK) {
            printf("Unbalanced brackets in pattern\n");
        } else {
            printf("Unknown error code: %d\n", rc);
        }
        if (rc == 0) {
            c->command = c->buf + matches[1].rm_so;
            c->location = c->buf + matches[2].rm_so;
            c->http = c->buf + matches[3].rm_so;

            c->command[matches[1].rm_eo] = '\0';
            c->location[matches[2].rm_eo - matches[2].rm_so] = '\0';
            c->http[matches[3].rm_eo - matches[3].rm_so] = '\0';
            c->status = -1;
            c->noContent = false;
            c->ptr = NULL;
        } else {
            c->command = NULL;
            c->location = NULL;
            c->http = NULL;
        }
        if (rc2 == 0) {
            c->contentLen = (&c->buf[matches2[0].rm_so]);
            c->contentLen[matches2[0].rm_eo] = '\0';
            c->importantNum = matches2[0].rm_eo;
            c->ptr = NULL;

            char *one = strstr(c->contentLen, " ");
            char *two = strstr(c->contentLen, "\n");
            int start = (int) (one - c->contentLen);
            int end = (int) (two - c->contentLen);

            char ed[end - start];
            for (int i = 0; i < end - start - 1; i++) {
                ed[i] = *(c->contentLen + start + i + 1);
            }
            c->contentNum = atoi(ed);

        } else {

            c->contentLen = "";
            c->importantNum = -1;
        }
    }
    regfree(&re);
    regfree(&re2);
}

static void commander(Command *c, int fd) {
    if ((c->func == true) && (c->status != 400) && (c->status != 505) && (c->status != 501)) {
        get(c, fd);
    } else if ((c->func == false) && (c->status != 400) && (c->status != 505)
               && (c->status != 501)) {
        put(c, fd);
    } else {
        return;
    }
}

static void analysze(Command *c, int fd) {
    if (strcmp(c->command, "GET") == 0) {
        c->func = true;

    } else if (strcmp(c->command, "PUT") == 0) {
        c->func = false;
    } else {
        c->status = 501;
        protocols(c);
        header(c, fd);
        return;
    }

    if (strcmp(c->http, "HTTP/1.1") != 0) {
        if (strcmp(c->http, "HTTP/1.10") == 0) {
            c->status = 400;
            protocols(c);
            header(c, fd);
            return;
        }
        c->status = 505;
        protocols(c);
        header(c, fd);
        return;
    }

    if ((strncmp(c->contentLen, "Content-Length:", 15) != 0) && c->func == false) {
        if ((strcmp(c->contentLen, "") == 0)) {
            c->noContent = true;
            return;
        }
        c->status = 400;
        protocols(c);
        header(c, fd);
    }
}

int main(int argc, char *argv[]) {
    Command c;
    (void) c;
    (void) argc;
    int port = atoi(argv[1]);

    Listener_Socket *sock = malloc(sizeof(Listener_Socket));
    int i = listener_init(sock, port);

    if (i != 0) {
        //printf("Localhost Port opened\n");
        fprintf(stderr, "Invalid Port\n");
        return 1;
    }

    while (1) {
        int fd = listener_accept(sock);

        parse(&c, fd); // Reads input and parses it into categories
        analysze(&c, fd); // Checks which method and if http protocol is correct
        commander(&c, fd); // Does GET or PUT

        close(fd);
    }
    free(sock);
    free((&c)->buf);
    free((&c)->bufCmp);

    return 0;
}
