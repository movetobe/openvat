#include "ovat-netsock.h"
#include "ovat-netsock-msg.h"
#include "ovat-utils.h"
#include "ovat-list.h"
#include "netsock-conn.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int ovat_appctl_exit = 0;

static int
ovat_appctl_msg_handler(int fd, struct netsock *netsock_, void *msg)
{
    int i = 0;
    struct ovat_netsock_msg *reply = (struct ovat_netsock_msg *)msg;

    if ((reply->argc == 1) && (!strcmp(reply->argv[0], OVAT_NETSOCK_MSG_ACK))) {
        ovat_appctl_exit = 1;
        goto exit;
    }
    for (i = 0; i < reply->argc; i++) {
        printf("%s  ", reply->argv[i]);
    }
    printf("\n");

exit:
    return 0;
}

int
main(int argc, char *argv[])
{
    int ret = OVAT_EOK;
    struct netsock *netsock = NULL;
    struct netsock_conn *srv_conn = NULL;
    struct ovat_netsock_msg msg;
    int i = 0;
    size_t len = 0;

    ovat_netsock_create("ovat-appctl-client", NETSOCK_CONN_TYPE_CLIENT,
                    "/tmp/ovat-ctl-server.sock", &netsock, ovat_appctl_msg_handler);


    list_for_each_entry (srv_conn, &(netsock->conn_list), conn_node) {
        if (srv_conn->fd > 0) {
            break;
        }
    }

    memset(&msg, 0, sizeof(struct ovat_netsock_msg));
    msg.argc = argc;
    for (i = 0; i < argc; i++) {
        len = strlen(argv[i]) < sizeof(msg.argv[i]) ? (strlen(argv[i]) + 1) : sizeof(msg.argv[i]);
        memcpy(msg.argv[i], argv[i], len);
    }
    ret = netsock->class->send(srv_conn->fd, netsock, (const void *)&msg, sizeof(msg));

    while (!ovat_appctl_exit);
    ovat_netsock_destroy(netsock);

    return ret;
}





