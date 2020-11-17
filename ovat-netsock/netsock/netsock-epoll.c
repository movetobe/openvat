#include "netsock-epoll.h"
#include "ovat-utils.h"
#include "netsock-conn.h"
#include "ovat-log.h"

/*
typedef union epoll_data {
    void        *ptr;
    int          fd;
    uint32_t     u32;
    uint64_t     u64;
} epoll_data_t;

struct epoll_event {
    uint32_t     events;      // Epoll events
    epoll_data_t data;        // User data variable
};
*/
int
netsock_epoll_init(void)
{
    int efd = epoll_create(65535);

    if (efd < 0) {
        OVAT_LOG(ERR, NETSOCK, "epoll create error, epollfd %d\n", efd);
        goto out;
    }

out:
    return efd;
}

void
netsock_epoll_uninit(int epollfd)
{
    close(epollfd);
}

int
netsock_epoll_ops(int epollfd, int ops, int fd, void *netsock_conn)
{
    int ret = OVAT_EOK;
    struct epoll_event ev;

    memset(&ev, 0, sizeof(ev));
    ev.events = EPOLLIN;
    ev.data.ptr = netsock_conn;
    if (epoll_ctl(epollfd, ops, fd, &ev) < 0) {
        OVAT_LOG(ERR, NETSOCK, "epoll_ctl error, errno %d\n", errno);
        ret = -OVAT_ESYSCALL;
        goto out;
    }

out:
    return ret;
}

int
netsock_epoll_process(int epollfd, struct epoll_event *events, int maxevents,
                      int timeout)
{
    int ret = OVAT_EOK;
    int nfds = 0;
    int i = 0;
    struct netsock_conn *conn = NULL;

    /* timeout -1, indefinitely, timeout 0, return immediately.*/
    nfds = epoll_wait(epollfd, events, maxevents, timeout);

    if (nfds == 0) {
        goto out;
    }

    if (nfds < 0) {
        OVAT_LOG(ERR, NETSOCK, "epoll_wait error, errno %d\n", errno);
        ret = -OVAT_ESYSCALL;
        goto out;
    }

    for (i = 0; i < nfds; i++) {
        conn = (struct netsock_conn *)events[i].data.ptr;
        if ((events[i].events & EPOLLIN) && (conn->fd > 0) && (conn->handler)) {
            ret = conn->handler(conn);
        }
    }

out:
    return ret;
}