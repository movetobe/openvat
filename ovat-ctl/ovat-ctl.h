#ifndef OVAT_CTL_H
#define OVAT_CTL_H

#include "ovat-list.h"
#include "ovat-netsock-msg.h"

typedef void ovat_ctl_cb(int fd, void *msg, void *aux);
struct ovat_ctl_command {
    const char *name;
    const char *usage;
    int min_args;
    int max_args;
    ovat_ctl_cb *cb;
    void *aux;
    struct list_head command_node;
};

struct ovat_pending_command {
    /* msg_body shall be first position */
    struct ovat_netsock_msg msg_body;
    int fd;
    void *aux;
    struct list_head command_node;
};

struct list_head *ovat_ctl_commands_list(void);
void ovat_ctl_command_init(void);
int ovat_ctl_command_append(int fd, void *aux, void *msg);
void ovat_ctl_command_process(void *aux);
int ovat_ctl_command_register(const char *name, const char*usage, int min_args,
                                    int max_args, ovat_ctl_cb *cb, void *aux);
void ovat_ctl_command_unregister(const char *name);

#endif /* OVAT_CTL_H */
