#include "ovat-ctl.h"
#include "ovat-list.h"
#include "ovat-utils.h"
#include "ovat-netsock.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static struct list_head ovat_ctl_commands;
static struct list_head ovat_pending_commands;
static ovat_pthread_mutex_t ovat_pc_mutex;

static struct ovat_ctl_command *
ovat_ctl_command_lookup(const char *name)
{
    struct ovat_ctl_command *command = NULL;

    list_for_each_entry(command, &ovat_ctl_commands, command_node) {
        if (!strcmp(command->name, name)) {
            return command;
        }
    }
    return NULL;
}

struct list_head *
ovat_ctl_commands_list(void)
{
    return &ovat_ctl_commands;
}

void
ovat_ctl_command_init(void)
{
    INIT_LIST_HEAD(&ovat_ctl_commands);
    INIT_LIST_HEAD(&ovat_pending_commands);
    ovat_pthread_mutex_init(&ovat_pc_mutex);
}

void
ovat_ctl_command_uninit(void)
{
    struct ovat_ctl_command *command = NULL, *tmp = NULL;
    struct ovat_pending_command *pcommand = NULL, *ptmp = NULL;

    list_for_each_entry_safe(command, tmp, &ovat_ctl_commands, command_node) {
        if (command) {
            list_del(&command->command_node);
            free(command);
        }
    }

    list_for_each_entry_safe(pcommand, ptmp, &ovat_pending_commands, command_node) {
        if (pcommand) {
            list_del(&pcommand->command_node);
            free(pcommand);
        }
    }
}

int
ovat_ctl_command_append(int fd, void *aux, void *msg)
{
    int ret = OVAT_EOK;
    struct ovat_pending_command *command = calloc(1, sizeof(struct ovat_pending_command));

    if (command == NULL) {
        ret = OVAT_ESYSCALL;
        goto err;
    }
    command->fd = fd;
    command->aux = aux;
    memcpy(&command->msg_body, msg, sizeof(struct ovat_netsock_msg));
    ovat_pthread_mutex_lock(&ovat_pc_mutex);
    list_add_tail(&(command->command_node), &ovat_pending_commands);
    ovat_pthread_mutex_unlock(&ovat_pc_mutex);

err:
    return ret;
}

void
ovat_ctl_command_process(void *aux)
{
    struct ovat_pending_command *pd_command = NULL;
    struct ovat_pending_command *pd_command_tmp = NULL;
    struct ovat_ctl_command *command = NULL;
    struct ovat_netsock_msg reply;

    /* check pending command is ok or not */
    ovat_pthread_mutex_lock(&ovat_pc_mutex);
    list_for_each_entry_safe(pd_command, pd_command_tmp, &ovat_pending_commands, command_node) {
        if ((pd_command->msg_body.argc > 1) && (pd_command->msg_body.argv[1]))
        command = ovat_ctl_command_lookup(pd_command->msg_body.argv[1]);
        if (command == NULL) {
            memset(&reply, 0, sizeof(struct ovat_netsock_msg));
            reply.argc = 2;
            snprintf(reply.argv[0], sizeof(reply.argv[0]),
                    "%s", "command has not been registered,");
            snprintf(reply.argv[1], sizeof(reply.argv[1]),
                    "%s", "try <ovat-appctl <ecu> commands/list> to show commands");
            ovat_netsock_msg_reply(pd_command->fd, pd_command->aux, &reply);
            ovat_netsock_msg_ack(pd_command->fd, pd_command->aux);
            list_del(&pd_command->command_node);
            free(pd_command);
            continue;
        }
        if ((pd_command->msg_body.argc < (command->min_args + 2))
            || (pd_command->msg_body.argc > (command->max_args + 2))) {
            memset(&reply, 0, sizeof(struct ovat_netsock_msg));
            reply.argc = 1;
            snprintf(reply.argv[0], sizeof(reply.argv[0]),
                    "command args number isn't between [%d, %d]\n",
                    command->min_args, command->max_args);
            ovat_netsock_msg_reply(pd_command->fd, pd_command->aux, &reply);
            ovat_netsock_msg_ack(pd_command->fd, pd_command->aux);
            list_del(&pd_command->command_node);
            free(pd_command);
            continue;
        }
        command->cb(pd_command->fd, &pd_command->msg_body, command->aux);

        list_del(&pd_command->command_node);
        free(pd_command);
    }
    ovat_pthread_mutex_unlock(&ovat_pc_mutex);
}

int
ovat_ctl_command_register(const char *name, const char *usage, int min_args,
                                    int max_args, ovat_ctl_cb *cb, void *aux)
{
    int ret = OVAT_EOK;
    struct ovat_ctl_command *command = NULL;

    command = ovat_ctl_command_lookup(name);
    if (command) {
        ret = -OVAT_EEXIST;
        goto err;
    }

    command = calloc(1, sizeof(struct ovat_ctl_command));
    if (command == NULL) {
        OVAT_LOG(INFO, CORE, "malloc failed\n");
        ret = OVAT_ESYSCALL;
        goto err;
    }
    command->name = name;
    command->usage = usage;
    command->min_args = min_args;
    command->max_args = max_args;
    command->cb = cb;
    command->aux = aux;
    list_add_tail(&(command->command_node), &ovat_ctl_commands);

err:
    return ret;
}

void
ovat_ctl_command_unregister(const char *name)
{
    struct ovat_ctl_command *command = NULL;

    command = ovat_ctl_command_lookup(name);
    if (command) {
        list_del(&command->command_node);
        free(command);
    }

}

