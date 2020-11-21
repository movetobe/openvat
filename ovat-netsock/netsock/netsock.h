#ifndef NETSOCK_H
#define NETSOCK_H

#include "ovat-log.h"
#include "ovat-list.h"
#include <sys/epoll.h>
#include <pthread.h>

#define MAX_NETSOCK_NAME (32)
#define MAX_NETSOCK_PATH (32)
#define MAX_EPOLL_EVENTS (1024)
#define NETSOCK_LISTEN_BACKLOG (1)

enum netsock_conn_type {
    NETSOCK_CONN_TYPE_NONE,
    NETSOCK_CONN_TYPE_CLIENT,
    NETSOCK_CONN_TYPE_SERVER,
    NETSOCK_CONN_TYPE_MAX
};

struct netsock {
    char name[MAX_NETSOCK_NAME];
    int conn_type; /* conn type: client/server */
    char path[MAX_NETSOCK_PATH]; /* path: unix path is /tmp/tapx.sock, tcp path is 192.168.100.100:5566 */
    int epollfd;
    pthread_t thread_id;
    struct epoll_event events[MAX_EPOLL_EVENTS];
    int sock_family;
    int sock_type; /* socket type, SOCK_STREAM/SOCK_DGRAM... */
    int sock_protocol;
    struct netsock_class *class;
    struct list_head conn_list;
};

typedef int msg_handler_cb(int fd, struct netsock *netsock_, void *msg);
struct netsock_class {
    char *type; /* netsock type: unix_sock/tcp_sock/tcp_sock_ssl... */
    int (*init)(void);
    int (*run)(struct netsock *netsock_);
    struct netsock * (*alloc)(void);
    int (*dealloc)(struct netsock *netsock_);
    int (*construct)(struct netsock *netsock_);
    int (*destruct)(struct netsock *netsock_);
    int (*recv)(int fd, struct netsock *netsock_, void *msg);
    int (*send)(int fd, struct netsock *netsock_, const void *msg, size_t count);
    msg_handler_cb *msg_handler;
};

int netsock_initialize(void);
int netsock_open(char *name, int conn_type, char *path, char *class_type, struct netsock **netsockp);
int netsock_close(struct netsock *netsock_);
int netsock_class_register(struct netsock_class *new_class);
void netsock_class_unregister(struct netsock_class *new_class);
#endif /* NETSOCK_H */