#include "ovat-ctl.h"
#include "ovat-netsock.h"
#include "ovat-utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int ovat_core_exit;

static int
ovat_core_get_exit(void)
{
    return ovat_core_exit;
}

static void
ovat_core_set_exit(int fd, void *msg, void *aux)
{
    ovat_netsock_msg_ack(fd, (struct netsock *)aux);
    ovat_core_exit = 1;
}

static void
ovat_core_load_module(int fd, void *msg, void *aux)
{
    struct ovat_netsock_msg *load_msg = (struct ovat_netsock_msg *)msg;
    printf("load module %s\n", load_msg->argv[2]);
    ovat_netsock_msg_ack(fd, (struct netsock *)aux);
}

static int
ovat_core_msg_handler(int fd, struct netsock *netsock_, void *msg)
{
    ovat_ctl_command_append(fd, netsock_, msg);
    return OVAT_EOK;
}

static void
ovat_core_commands_dump(int fd, void *msg, void *aux)
{
    struct ovat_ctl_command *command = NULL;
    struct netsock *netsock = (struct netsock *)aux;
    struct list_head *ovat_commands = ovat_ctl_commands_list();
    struct ovat_netsock_msg reply;

    list_for_each_entry(command, ovat_commands, command_node) {
        if (command) {
            memset(&reply, 0, sizeof(struct ovat_netsock_msg));
            reply.argc = 3;
            snprintf(reply.argv[0], sizeof(reply.argv[0]), "%s", "ovat-appctl");
            snprintf(reply.argv[1], sizeof(reply.argv[1]), "%s", command->name);
            snprintf(reply.argv[2], sizeof(reply.argv[2]), "%s", command->usage);
            ovat_netsock_msg_reply(fd, aux, &reply);
        }
    }
    ovat_netsock_msg_ack(fd, netsock);
}


int
main(int argc, char *argv[])
{
    int ret = OVAT_EOK;
    struct netsock *netsock = NULL;

    ovat_ctl_command_init();
    ret = ovat_netsock_create("ovat-ctl-server", NETSOCK_CONN_TYPE_SERVER,
                                "/tmp/ovat-ctl-server.sock", &netsock, ovat_core_msg_handler);
    if (ret < 0) {
        printf("unix server create failed\n");
        goto out;
    }

    ovat_ctl_command_register("commands/list", "List commands", 0, 0, ovat_core_commands_dump, netsock);
    ovat_ctl_command_register("ovat/exit", "Exit ovat platform", 0, 0, ovat_core_set_exit, netsock);
    ovat_ctl_command_register("module/load", "Load Autosar BSW module", 1, 1, ovat_core_load_module, netsock);

    while (!ovat_core_get_exit()) {
        ovat_netsock_run(netsock);
    }

    ovat_netsock_destroy(netsock);
out:
    return ret;
}
