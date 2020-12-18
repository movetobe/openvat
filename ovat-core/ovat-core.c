#include "ovat-ctl.h"
#include "ovat-netsock.h"
#include "ovat-utils.h"
#include "ovat-if.h"
#include "ovat-log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <linux/limits.h>

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
ovat_core_module_load(int fd, void *msg, void *aux)
{
    struct ovat_netsock_msg *load_msg = (struct ovat_netsock_msg *)msg;

    OVAT_LOG(INFO, CORE, "load module: %s, period: %s\n", load_msg->argv[2], load_msg->argv[3]);
    if (ovat_if_module_load(load_msg->argv[2], atoi(load_msg->argv[3])) != OVAT_EOK) {
        ovat_if_action_reply(fd, aux, "Maybe it has not been registered, Load module", OVAT_IF_ACTION_NOT_OK);
        return;
    }
    ovat_if_action_reply(fd, aux, "Load module", OVAT_IF_ACTION_OK);
    return;
}

static void
ovat_core_module_unload(int fd, void *msg, void *aux)
{
    struct ovat_netsock_msg *unload_msg = (struct ovat_netsock_msg *)msg;
    OVAT_LOG(INFO, CORE, "unload module: %s\n", unload_msg->argv[2]);
    ovat_if_module_unload(unload_msg->argv[2]);
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
            reply.argc = 4;
            snprintf(reply.argv[0], sizeof(reply.argv[0]), "%s", "ovat-appctl");
            snprintf(reply.argv[1], sizeof(reply.argv[1]), "%s", "<ecu>");
            snprintf(reply.argv[2], sizeof(reply.argv[2]), "%s", command->name);
            snprintf(reply.argv[3], sizeof(reply.argv[3]), "%s", command->usage);
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
    char log_path[PATH_MAX] = {0};
    char socket_path[PATH_MAX] = {0};

    snprintf(log_path, PATH_MAX, "/var/log/ovat-%s.log", (argc > 1) ? argv[1] : "vecu0");
    ovat_log_init(log_path);
    ovat_ctl_command_init();
    snprintf(socket_path, PATH_MAX, "/tmp/ovat-ctl-server-%s.sock", (argc > 1) ? argv[1] : "vecu0");
    ret = ovat_netsock_create("ctl-srv", NETSOCK_CONN_TYPE_SERVER,
                                socket_path, &netsock, ovat_core_msg_handler);
    if (ret < 0) {
        OVAT_LOG(ERR, CORE, "unix server create failed\n");
        goto out;
    }

    ovat_ctl_command_register("commands/list", "", 0, 0, ovat_core_commands_dump, netsock);
    ovat_ctl_command_register("ovat/exit", "", 0, 0, ovat_core_set_exit, netsock);
    ovat_ctl_command_register("module/load", "[module name] [period]", 2, 2, ovat_core_module_load, netsock);
    ovat_ctl_command_register("module/unload", "[module name]", 1, 1, ovat_core_module_unload, netsock);

    ovat_if_init(netsock);
    OVAT_LOG(INFO, CORE, "ovat-core start\n");
    while (!ovat_core_get_exit()) {
        ovat_netsock_run(netsock);
    }

    ovat_if_uninit();
out:
    if (netsock) {
        ovat_netsock_destroy(netsock);
    }
    ovat_ctl_command_uninit();
    ovat_log_uninit();
    return ret;
}

OVAT_LOG_REGISTER(core_logtype, CORE, INFO);

