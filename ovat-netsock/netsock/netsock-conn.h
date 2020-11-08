#ifndef NETSOCK_CONN_H
#define NETSOCK_CONN_H

#include "ovat-list.h"

#include <linux/types.h>

struct netsock_conn {
    int fd;
    void *context;
    int (*handler)(void *ctx);
    struct list_head conn_node;
};

int netsock_conn_construct(int fd, int (*handler)(void *ctx), void *context);
int netsock_conn_destruct(int fd, int epollfd, struct list_head *list);

#endif /* NETSOCK_CONN_H */