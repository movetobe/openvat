#include "ovat-netsock.h"
#include "ovat-netsock-msg.h"
#include "ovat-utils.h"
#include "ovat-list.h"
#include "netsock-conn.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/limits.h>

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
    char sock_path[PATH_MAX] = {0};
    int index = 0;

    snprintf(sock_path, PATH_MAX, "/tmp/ovat-ctl-server-%s.sock", argv[1]);

    ret = ovat_netsock_create("ovat-appctl-client", NETSOCK_CONN_TYPE_CLIENT,
                    sock_path, &netsock, ovat_appctl_msg_handler);
    if (ret < 0) {
        printf("ovat-appctl start failed, maybe ovat-core %s has not been started\n", argv[1]);
        goto out;
    }

    list_for_each_entry (srv_conn, &(netsock->conn_list), conn_node) {
        if (srv_conn->fd > 0) {
            break;
        }
    }

    memset(&msg, 0, sizeof(struct ovat_netsock_msg));
    /* we do not copy argv[1] which indicates which ecu we will connect */
    msg.argc = argc - 1;
    for (i = 0; i < argc; i++) {
        if (i == 1) {
            continue;
        }
        len = strlen(argv[i]) < sizeof(msg.argv[index]) ? (strlen(argv[i]) + 1) : sizeof(msg.argv[index]);
        memcpy(msg.argv[index], argv[i], len);
        index++;
    }
    ret = netsock->class->send(srv_conn->fd, netsock, (const void *)&msg, sizeof(msg));

    while (!ovat_appctl_exit);

out:
    if (netsock) {
        ovat_netsock_destroy(netsock);
    }
    return ret;
}





