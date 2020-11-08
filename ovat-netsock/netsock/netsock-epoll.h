#ifndef NETSOCK_EPOLL_H
#define NETSOCK_EPOLL_H

#include <sys/epoll.h>

/* epoll wait timeout 50ms */
#define NETSOCK_EPOLL_TIMEOUT (50)

int netsock_epoll_init(void);
void netsock_epoll_uninit(int epollfd);
int netsock_epoll_ops(int epollfd, int ops, int fd, void *netsock_conn);
int netsock_epoll_process(int epollfd, struct epoll_event *events, int maxevents,
                          int timeout);

#endif /* NETSOCK_EPOLL_H */