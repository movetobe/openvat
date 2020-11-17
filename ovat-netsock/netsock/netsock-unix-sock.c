#include "netsock-unix-sock.h"
#include "netsock.h"
#include "netsock-conn.h"
#include "ovat-utils.h"

#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>

struct netsock_unix_sock {
    struct netsock up;
};

static int
netsock_unix_sock_receive_conn(void *context)
{
    int ret = 0;
    struct netsock_conn *conn = (struct netsock_conn *) context;
    struct netsock *netsock = (struct netsock *) (conn->context);
    struct netsock_unix_sock *unix_sock = container_of(netsock, struct netsock_unix_sock, up);;
    char buf[NETSOCK_UNIX_SOCK_MSG_BUF_LEN] = {0};
    int fd = conn->fd;

    ret = unix_sock->up.class->recv(fd, &(unix_sock->up), buf);
    if (ret < 0) {
        OVAT_LOG(ERR, NETSOCK, "recv error, errno %d\n", errno);
        goto out;
    }
    if (ret == 0) {
        ret = netsock_conn_destruct(fd, netsock->epollfd, &(conn->conn_node));
        close(fd);
        goto out;
    }

    if ((unix_sock->up.class->msg_handler == NULL)
        || (unix_sock->up.class->msg_handler(fd, &(unix_sock->up), buf) < 0)) {
        OVAT_LOG(ERR, NETSOCK, "process message error\n");
        ret = -OVAT_ECALL;
    }

out:
    return ret;
}

static int
netsock_unix_sock_accept_conn(void *context)
{
    int ret = 0;
    struct netsock_conn *listen_conn = (struct netsock_conn *) context;
    struct netsock *netsock = (struct netsock *) (listen_conn->context);
    struct netsock_unix_sock *unix_sock = container_of(netsock, struct netsock_unix_sock, up);
    struct sockaddr_un sockaddr;
    socklen_t sockaddr_len = 0;
    int connfd = 0;

    memset(&sockaddr, 0, sizeof(struct sockaddr_un));
    connfd = accept(listen_conn->fd, (struct sockaddr *) &sockaddr, &sockaddr_len);
    if (connfd < 0) {
        OVAT_LOG(ERR, NETSOCK, "accept error, connfd is %d\n", connfd);
        ret = -OVAT_ESYSCALL;
        goto err;
    }
    ret = fcntl(connfd, F_SETFL, O_NONBLOCK);
    if (ret < 0) {
        OVAT_LOG(ERR, NETSOCK, "set fd non-blocking failed, errno %d\n", errno);
        goto err;
    }
    ret = netsock_conn_construct(connfd, netsock_unix_sock_receive_conn, (void *) &(unix_sock->up));
    if (ret < 0) {
        OVAT_LOG(ERR, NETSOCK, "netsock_conn_construct error\n");
        goto err;
    }
    
err:
    return ret;
}

static int
netsock_unix_sock_init(void)
{
    return 0;
}

static int
netsock_unix_sock_run(struct netsock *netsock_)
{
    return 0;
}

static struct netsock *
netsock_unix_sock_alloc(void)
{
    struct netsock_unix_sock *unix_sock = NULL;

    unix_sock = calloc(1, sizeof(struct netsock_unix_sock));
    if (unix_sock == NULL) {
        OVAT_LOG(ERR, NETSOCK, "netsock_unix_sock_alloc: malloc error\n");
        return NULL;
    }

    return &(unix_sock->up);
}

static int
netsock_unix_sock_dealloc(struct netsock *netsock_)
{
    struct netsock_unix_sock *unix_sock = container_of(netsock_, struct netsock_unix_sock, up);

    free(unix_sock);

    return 0;
}

static int
netsock_unix_sock_construct(struct netsock *netsock_)
{
    int ret = 0;
    int sockfd = 0;
    struct sockaddr_un sockaddr;
    struct netsock_unix_sock *unix_sock = container_of(netsock_, struct netsock_unix_sock, up);

    unix_sock->up.sock_family = AF_UNIX;
    unix_sock->up.sock_type = SOCK_STREAM;

    sockfd = socket(unix_sock->up.sock_family, unix_sock->up.sock_type, unix_sock->up.sock_protocol);
    
    memset(&sockaddr, 0, sizeof(struct sockaddr_un));
    sockaddr.sun_family = AF_UNIX;
    strncpy(sockaddr.sun_path, netsock_->path, sizeof(sockaddr.sun_path) - 1);

    ret = fcntl(sockfd, F_SETFL, O_NONBLOCK);
    if (ret < 0) {
        OVAT_LOG(ERR, NETSOCK, "set fd non-blocking failed, errno %d\n", errno);
        goto err;
    }

    if (unix_sock->up.conn_type == NETSOCK_CONN_TYPE_SERVER) {
        (void)unlink(netsock_->path);
    
        ret = bind(sockfd, (struct sockaddr *) &sockaddr, sizeof(struct sockaddr_un));
        if (ret < 0) {
            OVAT_LOG(ERR, NETSOCK, "listening socket bind error, errno %d\n", errno);
            goto err;
        }

        ret = listen(sockfd, NETSOCK_LISTEN_BACKLOG);
        if (ret < 0) {
            OVAT_LOG(ERR, NETSOCK, "listening socket error, errno %d\n", errno);
            goto err;
        }

        ret = netsock_conn_construct(sockfd, netsock_unix_sock_accept_conn, (void *) &(unix_sock->up));
        if (ret < 0) {
            OVAT_LOG(ERR, NETSOCK, "netsock_conn_construct error\n");
            goto err;
        }
    } else if (unix_sock->up.conn_type == NETSOCK_CONN_TYPE_CLIENT) {
        ret =  connect(sockfd, (struct sockaddr *) &sockaddr, sizeof(struct sockaddr_un));
        if (ret < 0) {
            OVAT_LOG(ERR, NETSOCK, "connecting to server error, errno %d\n", errno);
            goto err;
        }

        ret = netsock_conn_construct(sockfd, netsock_unix_sock_receive_conn, (void *) &(unix_sock->up));
        if (ret < 0) {
            OVAT_LOG(ERR, NETSOCK, "netsock_conn_construct error\n");
            goto err;
        }
    } else {
        OVAT_LOG(ERR, NETSOCK, "connection type error, type is %d\n", unix_sock->up.conn_type);
        ret = -OVAT_ENOEXIST;
        goto err;
    }

    return ret;

err:
    close(sockfd);
    return ret;
}

static int
netsock_unix_sock_destruct(struct netsock *netsock_)
{
    int ret = OVAT_EOK;
    struct netsock_unix_sock *unix_sock = container_of(netsock_, struct netsock_unix_sock, up);

    if (unix_sock->up.conn_type == NETSOCK_CONN_TYPE_SERVER) {
        (void)unlink(netsock_->path);
    }
    return ret;
}

static int
netsock_unix_sock_recv(int fd, struct netsock *netsock_, void *msg)
{
    int ret = 0;

    ret = read(fd, msg, NETSOCK_UNIX_SOCK_MSG_BUF_LEN);
    
    return ret;
}

static int
netsock_unix_sock_send(int fd, struct netsock *netsock_, const void *msg, size_t count)
{
    int ret = 0;

    ret = write(fd, msg, count);

    return ret;
}

static struct netsock_class ns_unix_sock = {
    .type = "unix_sock",
    .init = netsock_unix_sock_init,
    .run = netsock_unix_sock_run,
    .alloc = netsock_unix_sock_alloc,
    .dealloc = netsock_unix_sock_dealloc,
    .construct = netsock_unix_sock_construct,
    .destruct = netsock_unix_sock_destruct,
    .recv = netsock_unix_sock_recv,
    .send = netsock_unix_sock_send
};

int
netsock_unix_sock_register(msg_handler_cb *cb)
{
    ns_unix_sock.msg_handler = cb;
    return netsock_class_register(&ns_unix_sock);
}