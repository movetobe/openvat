#include "ovat-netsock.h"
#include "netsock-unix-sock.h"
#include "netsock-conn.h"
#include "ovat-utils.h"
#include "ovat-list.h"
#include "ovat-ctl.h"
#include <stdio.h>

void
ovat_netsock_msg_ack(int fd, struct netsock *netsock_)
{
    struct ovat_netsock_msg msg_ack;

    /* send message end notify peer */
    memset(&msg_ack, 0, sizeof(struct ovat_netsock_msg));
    msg_ack.argc = 1;
    snprintf(msg_ack.argv[0], sizeof(msg_ack.argv[0]), "%s", OVAT_NETSOCK_MSG_ACK);
    (void)netsock_->class->send(fd, netsock_, &msg_ack, sizeof(struct ovat_netsock_msg));
}

void
ovat_netsock_msg_reply(int fd, struct netsock *netsock_, void *msg)
{
    (void)netsock_->class->send(fd, netsock_, msg, sizeof(struct ovat_netsock_msg));
}



int
ovat_netsock_create(char *name, int conn_type,
                        char *path, struct netsock **netsockp,
                        msg_handler_cb *cb)
{
    int ret = OVAT_EOK;

    ret = netsock_initialize();
    if (ret < 0) {
        OVAT_LOG(ERR, NETSOCK, "netsock initialize failed, path: %s, conn_type: %d, ret: %d\n",
                path, conn_type, ret);
        goto err;
    }

    ret = netsock_unix_sock_register(cb);
    if (ret < 0) {
        OVAT_LOG(ERR, NETSOCK, "netsock unix sock register failed, path: %s, conn_type: %d, ret: %d\n",
                path, conn_type, ret);
        goto err;
    }
    ret =  netsock_open(name, conn_type, path, "unix_sock", netsockp);
    if (ret < 0) {
        OVAT_LOG(ERR, NETSOCK, "netsock open failed, path: %s, conn_type: %d, ret: %d\n",
                path, conn_type, ret);
        goto err;
    }

err:
    return ret;
}

void
ovat_netsock_run(struct netsock *netsock_)
{
    ovat_ctl_command_process(netsock_);
}

int
ovat_netsock_destroy(struct netsock *netsock_)
{
    int ret = netsock_close(netsock_);

    if (ret <0) {
        OVAT_LOG(ERR, NETSOCK, "netsock close failed, ret: %d\n", ret);
        goto err;
    }

err:
    return ret;
}
