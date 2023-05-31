// Asgn 2: A simple HTTP server.
// By: Eugene Chou
//     Andrew Quinn
//     Brian Zhao

#include "asgn2_helper_funcs.h"
#include "connection.h"
#include "debug.h"
#include "response.h"
#include "request.h"
#include "queue.h"

#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <getopt.h>

#include <sys/stat.h>
#include <sys/file.h>

void handle_connection(int);

void handle_get(conn_t *);
void handle_put(conn_t *);
void handle_unsupported(conn_t *);

void *worker_station(void *queue);
void *dispatcher_station(void *queue);
void irs(char *method, char *uri, uint16_t code, char *request_id);

size_t port = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t not_full = PTHREAD_COND_INITIALIZER;

int main(int argc, char **argv) {
    int opt = 0;
    int thread_count = 4;
    if (argc < 2) {
        return EXIT_FAILURE;
    }

    while ((opt = getopt(argc, argv, "t:")) != -1) {
        switch (opt) {
        case 't': thread_count = atoi(optarg); break;
        default: break;
        }
    }

    char *endptr = NULL;
    port = (size_t) strtoull(argv[optind], &endptr, 10);

    signal(SIGPIPE, SIG_IGN);

    pthread_t workers[thread_count];
    pthread_t dispatcher;

    queue_t *job_queue = queue_new(thread_count);

    pthread_create(&dispatcher, NULL, dispatcher_station, (void *) job_queue);

    for (int i = 0; i < thread_count; i++) {
        pthread_create(&workers[i], NULL, worker_station, (void *) job_queue);
    }

    pthread_join(dispatcher, NULL);
    for (int i = 0; i < thread_count; i++) {
        pthread_cancel(workers[i]);
        pthread_join(workers[i], NULL);
    }

    queue_delete(&job_queue);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&not_empty);
    pthread_cond_destroy(&not_full);

    return EXIT_SUCCESS;
}

void handle_connection(int connfd) {

    conn_t *conn = conn_new(connfd);

    const Response_t *res = conn_parse(conn);

    if (res != NULL) {
        conn_send_response(conn, res);
    } else {
        const Request_t *req = conn_get_request(conn);
        if (req == &REQUEST_GET) {
            handle_get(conn);
        } else if (req == &REQUEST_PUT) {
            handle_put(conn);
        } else {
            handle_unsupported(conn);
        }
    }

    conn_delete(&conn);
}

void handle_get(conn_t *conn) {

    char *uri = conn_get_uri(conn);
    const Response_t *res = NULL;
    char *request_id = conn_get_header(conn, "Request-Id");

    bool existed = access(uri, F_OK) == 0;

    int fd = open(uri, O_RDONLY);

    flock(fd, LOCK_SH);

    if (fd < 0) {
        debug("%s: %d", uri, errno);
        if (errno == EACCES || errno == EISDIR || errno == ENOENT) {
            irs("GET", uri, 403, request_id);
            res = &RESPONSE_FORBIDDEN;
            conn_send_response(conn, res);
        } else {
            irs("GET", uri, 500, request_id);
            res = &RESPONSE_INTERNAL_SERVER_ERROR;
            conn_send_response(conn, res);
        }
    }

    struct stat file_info;
    fstat(fd, &file_info);

    uint64_t size = file_info.st_size;

    if (S_ISDIR(file_info.st_mode)) {
        irs("GET", uri, 403, request_id);
        res = &RESPONSE_FORBIDDEN;
        conn_send_response(conn, res);
    }

    res = conn_send_file(conn, fd, size);

    if (res == NULL && existed) {
        res = &RESPONSE_OK;
        irs("GET", uri, 200, request_id);

    } else if (res == NULL && !existed) {
        res = &RESPONSE_CREATED;
        irs("GET", uri, 201, request_id);
    }

    close(fd);
}

void handle_unsupported(conn_t *conn) {

    // send responses
    conn_send_response(conn, &RESPONSE_NOT_IMPLEMENTED);
}

void handle_put(conn_t *conn) {

    char *uri = conn_get_uri(conn);
    const Response_t *res = NULL;
    char *request_id = conn_get_header(conn, "Request-Id");

    // Check if file already exists before opening it.

    // Open the file..
    pthread_mutex_lock(&mutex);
    bool existed = access(uri, F_OK) == 0;

    int fd = open(uri, O_CREAT | O_WRONLY, 0600);

    flock(fd, LOCK_EX);
    ftruncate(fd, 0);
    if (fd < 0) {
        debug("%s: %d", uri, errno);
        if (errno == EACCES || errno == EISDIR || errno == ENOENT) {
            irs("PUT", uri, 403, request_id);
            res = &RESPONSE_FORBIDDEN;
            goto out;
        } else {
            irs("PUT", uri, 500, request_id);
            res = &RESPONSE_INTERNAL_SERVER_ERROR;
            goto out;
        }
    }
    pthread_mutex_unlock(&mutex);
    res = conn_recv_file(conn, fd);

    if (res == NULL && existed) {
        res = &RESPONSE_OK;
        irs("PUT", uri, 200, request_id);
    } else if (res == NULL && !existed) {
        res = &RESPONSE_CREATED;
        irs("PUT", uri, 201, request_id);
    }

    close(fd);

out:
    conn_send_response(conn, res);
}

void *worker_station(void *queue) {
    queue_t *job_queue = (queue_t *) queue;
    while (1) {
        uint64_t connfd = 0;
        queue_pop(job_queue, (void *) &connfd);

        pthread_cond_broadcast(&not_empty);
        handle_connection((int) connfd);
        close(connfd);
    }
}

void *dispatcher_station(void *queue) {
    queue_t *job_queue = (queue_t *) queue;

    Listener_Socket sock;
    listener_init(&sock, port);

    while (1) {
        uint64_t connfd = listener_accept(&sock);

        queue_push(job_queue, (void *) connfd);
        pthread_cond_broadcast(&not_full);
    }
}

void irs(char *method, char *uri, uint16_t code, char *request_id) {
    fprintf(stderr, "%s,/%s,%hu,%s\n", method, uri, code, request_id);
    //FILE *p = fopen("audit_out.txt", "a+");
    //fprintf(p, "%s,/%s,%hu,%s\n", method, uri, code, request_id);
    //fclose(p);
}
