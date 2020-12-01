#include "netsock-can-sock.h"
#include "netsock.h"
#include "netsock-conn.h"
#include "ovat-utils.h"

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <fcntl.h>
#include <linux/can.h>
#include <linux/can/raw.h>


struct netsock_can_sock {
    struct netsock up;
};

static int
netsock_can_sock_receive_conn(void *context)
{
    int ret = 0;
    struct netsock_conn *conn = (struct netsock_conn *) context;
    struct netsock *netsock = (struct netsock *) (conn->context);
    struct netsock_can_sock *can_sock = container_of(netsock, struct netsock_can_sock, up);;
    struct can_frame frame;
    int fd = conn->fd;

    ret = can_sock->up.class->recv(fd, &(can_sock->up), &frame);
    if (ret < 0) {
        OVAT_LOG(ERR, NETSOCK, "recv error, errno %d\n", errno);
        goto out;
    }

    if ((can_sock->up.class->msg_handler == NULL)
        || (can_sock->up.class->msg_handler(fd, &(can_sock->up), &frame) < 0)) {
        OVAT_LOG(ERR, NETSOCK, "process message error\n");
        ret = -OVAT_ECALL;
    }

out:
    return ret;
}

static int
netsock_can_sock_init(void)
{
    return 0;
}

static int
netsock_can_sock_run(struct netsock *netsock_)
{
    return 0;
}

static struct netsock *
netsock_can_sock_alloc(void)
{
    struct netsock_can_sock *can_sock = NULL;

    can_sock = calloc(1, sizeof(struct netsock_can_sock));
    if (can_sock == NULL) {
        OVAT_LOG(ERR, NETSOCK, "netsock_can_sock_alloc: malloc error\n");
        return NULL;
    }

    return &(can_sock->up);
}

static int
netsock_can_sock_dealloc(struct netsock *netsock_)
{
    struct netsock_can_sock *can_sock = container_of(netsock_, struct netsock_can_sock, up);

    free(can_sock);

    return 0;
}

static int
netsock_can_sock_construct(struct netsock *netsock_)
{
    int ret = 0;
    int sockfd = 0;
    struct sockaddr_can sockaddr;
    struct ifreq ifr;
    struct netsock_can_sock *can_sock = container_of(netsock_, struct netsock_can_sock, up);

    can_sock->up.sock_family = PF_CAN;
    can_sock->up.sock_type = SOCK_RAW;
    can_sock->up.sock_protocol = CAN_RAW;

    sockfd = socket(can_sock->up.sock_family, can_sock->up.sock_type, can_sock->up.sock_protocol);
    
    strncpy(ifr.ifr_name, netsock_->path, sizeof(ifr.ifr_name) - 1);
    ioctl(sockfd, SIOCGIFINDEX, &ifr);

    memset(&sockaddr, 0, sizeof(struct sockaddr_can));
    sockaddr.can_family = AF_CAN;
    sockaddr.can_ifindex = ifr.ifr_ifindex;

    ret = fcntl(sockfd, F_SETFL, O_NONBLOCK);
    if (ret < 0) {
        OVAT_LOG(ERR, NETSOCK, "set fd non-blocking failed, errno %d\n", errno);
        goto err;
    }

    if (can_sock->up.conn_type == NETSOCK_CONN_TYPE_LOOPBACK) {

        ret = bind(sockfd, (struct sockaddr *) &sockaddr, sizeof(struct sockaddr_can));
        if (ret < 0) {
            OVAT_LOG(ERR, NETSOCK, "socket bind error, errno %d\n", errno);
            goto err;
        }

        ret = netsock_conn_construct(sockfd, netsock_can_sock_receive_conn, (void *) &(can_sock->up));
        if (ret < 0) {
            OVAT_LOG(ERR, NETSOCK, "netsock_conn_construct error\n");
            goto err;
        }
    } else {
        OVAT_LOG(ERR, NETSOCK, "connection type error, type is %d\n", can_sock->up.conn_type);
        ret = -OVAT_ENOEXIST;
        goto err;
    }

    return ret;

err:
    close(sockfd);
    return ret;
}

static int
netsock_can_sock_destruct(struct netsock *netsock_)
{
    /* should close connection fd */
    return OVAT_EOK;

}

static int
netsock_can_sock_recv(int fd, struct netsock *netsock_, void *msg)
{
    int ret = 0;

    ret = read(fd, msg, sizeof(struct can_frame));

    return ret;
}

static int
netsock_can_sock_send(int fd, struct netsock *netsock_, const void *msg, size_t count)
{
    int ret = 0;

    ret = write(fd, msg, sizeof(struct can_frame));

    return ret;
}

static struct netsock_class ns_can_sock = {
    .type = "can_sock",
    .init = netsock_can_sock_init,
    .run = netsock_can_sock_run,
    .alloc = netsock_can_sock_alloc,
    .dealloc = netsock_can_sock_dealloc,
    .construct = netsock_can_sock_construct,
    .destruct = netsock_can_sock_destruct,
    .recv = netsock_can_sock_recv,
    .send = netsock_can_sock_send
};

int
netsock_can_sock_register(msg_handler_cb *cb)
{
    ns_can_sock.msg_handler = cb;
    return netsock_class_register(&ns_can_sock);
}

void
netsock_can_sock_unregister(void)
{
    netsock_class_unregister(&ns_can_sock);
}

