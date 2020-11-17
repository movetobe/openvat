#include "ovat-if.h"
#include "ovat-nmif.h"
#include "ovat-cannmif.h"
#include "ovat-list.h"
#include "ovat-utils.h"
#include "ovat-netsock-msg.h"
#include "ovat-netsock.h"
#include "timeval.h"
#include "ovat-log.h"
#include "Nm.h"
#include "CanNm.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define OVAT_IF_MODULE_NAME_LEN 32

struct list_head ovat_if_modules;
struct ovat_if_module_ops {
    void (*init)(const void *configs);
    void (*mainfunc)(void);
};
struct ovat_if_module {
    char name[OVAT_IF_MODULE_NAME_LEN];
    pthread_t tid;
    int module_loaded;
    unsigned int mainfunc_period;
    struct list_head module_node;
    struct ovat_if_module_ops ops;
};

#define OVAT_IF_REGISTER_MODULE(MODULE_NAME)                                    \
do {                                                                            \
    struct ovat_if_module *module = calloc(1, sizeof(struct ovat_if_module));   \
    snprintf(module->name, OVAT_IF_MODULE_NAME_LEN, "%s", #MODULE_NAME);        \
    module->ops.init = (void *)MODULE_NAME##_Init;                                     \
    module->ops.mainfunc = MODULE_NAME##_MainFunction;                         \
    list_add_tail(&module->module_node, &ovat_if_modules);                      \
} while(0);

void
ovat_if_init(void *aux)
{
    INIT_LIST_HEAD(&ovat_if_modules);
    OVAT_IF_REGISTER_MODULE(Nm);
    ovat_nmif_command_register(aux);
    OVAT_IF_REGISTER_MODULE(CanNm);
    ovat_cannmif_command_register(aux);
}

void
ovat_if_uninit(void)
{
    struct ovat_if_module *module = NULL, *tmp = NULL;

    list_for_each_entry_safe(module, tmp, &ovat_if_modules, module_node) {
        if (module) {
            list_del(&module->module_node);
            free(module);
        }
    }
}

static void*
ovat_if_module_mainfunc(void *arg)
{
    struct ovat_if_module *module = (struct ovat_if_module *)arg;

    while (module->module_loaded) {
        module->ops.mainfunc();
        OVAT_LOG(INFO, IF, "MainFunction");
        /* sleep */
        ovat_msleep(module->mainfunc_period);
    }

    return NULL;
}

static struct ovat_if_module *
ovat_if_module_lookup(const char *name)
{
    struct ovat_if_module *module = NULL;

    list_for_each_entry(module, &ovat_if_modules, module_node) {
        if (!strcmp(module->name, name)) {
            break;
        }
    }
    return module;
}

int
ovat_if_module_unload(const char *name)
{
    struct ovat_if_module *module = NULL;

    module = ovat_if_module_lookup(name);
    if (module == NULL) {
        return -OVAT_ENOEXIST;
    }

    module->module_loaded = 0;
    return OVAT_EOK;
}

int
ovat_if_module_load(const char *name, unsigned int msecs)
{
    struct ovat_if_module *module = NULL;
    char thread_name[OVAT_IF_MODULE_NAME_LEN] = {0};

    module = ovat_if_module_lookup(name);
    if (module == NULL) {
        return -OVAT_ENOEXIST;
    }
    if (module->module_loaded) {
        return OVAT_EOK;
    }

    /* module init */
    if (module->ops.init == NULL) {
        return -OVAT_ENOEXIST;
    }
    module->ops.init(NULL);

    module->module_loaded = 1;
    module->mainfunc_period = msecs;

    snprintf(thread_name, OVAT_IF_MODULE_NAME_LEN, "%s_MainFunction", name);
    /* create a detach thread to execute mainfunction */
    ovat_pthread_create_detach(&module->tid, name, ovat_if_module_mainfunc, module);
    return OVAT_EOK;
}

void
ovat_if_action_reply(int fd, void *aux, const char *action, const char *ret)
{
    struct ovat_netsock_msg reply;
    struct netsock *netsock_ = (struct netsock *)aux;

    memset(&reply, 0, sizeof(struct ovat_netsock_msg));
    reply.argc = 1;
    snprintf(reply.argv[0], sizeof(reply.argv[0]), "%s %s", action, ret);

    ovat_netsock_msg_reply(fd, netsock_, &reply);
    ovat_netsock_msg_ack(fd, netsock_);
}

OVAT_LOG_REGISTER(if_logtype, ovat.if, INFO);
