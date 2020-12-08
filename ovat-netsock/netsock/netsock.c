#include "netsock.h"
#include "ovat-utils.h"
#include "ovat-list.h"
#include "netsock-epoll.h"
#include <string.h>
#include <malloc.h>

struct netsock_registered_class {
    struct list_head class_node;
    struct netsock_class *class;
};

static struct list_head registered_classes;

static struct netsock_registered_class *
netsock_lookup_class(char *type)
{
    struct netsock_registered_class *rc = NULL;

    list_for_each_entry (rc, &registered_classes, class_node) {
        if (!strcmp(rc->class->type, type)) {
            return rc;
        }
    }

    return NULL;
}

int
netsock_class_register(struct netsock_class *new_class)
{
    int ret = OVAT_EOK;

    if (netsock_lookup_class(new_class->type)) {
        OVAT_LOG(ERR, NETSOCK, "duplicate register netsock class, %s\n", new_class->type);
        ret = -OVAT_EEXIST;
        goto out;
    }

    ret = new_class->init() ? new_class->init() : OVAT_EOK;
    if (ret) {
        OVAT_LOG(ERR, NETSOCK, "new class init error, errno %d\n", ret);
        goto out;
    }

    struct netsock_registered_class *rc;
    rc = calloc(1, sizeof(*rc));
    if (!rc) {
        OVAT_LOG(ERR, NETSOCK, "malloc error\n");
        ret = -OVAT_ESYSCALL;
        goto out;
    }
    list_add_tail(&(rc->class_node), &registered_classes);
    rc->class = new_class;

out:
    return ret;
}

void
netsock_class_unregister(struct netsock_class *new_class)
{
    struct netsock_registered_class *rc = netsock_lookup_class(new_class->type);
    if (rc) {
        list_del(&(rc->class_node));
        free(rc);
    }
}


static void *
netsock_loop(void *arg)
{
    struct netsock *netsock_ = (struct netsock *) arg;

    while (!netsock_->netsock_exit) {
        netsock_epoll_process(netsock_->epollfd, netsock_->events, MAX_EPOLL_EVENTS, NETSOCK_EPOLL_TIMEOUT);
    }
    return NULL;
}

int
netsock_initialize(void)
{
    int ret = OVAT_EOK;

    INIT_LIST_HEAD(&registered_classes);

    return ret;
}

int
netsock_open(char *name, int conn_type, const char *path, char *class_type, struct netsock **netsockp)
{
    int ret = OVAT_EOK;
    struct netsock_registered_class *rc = NULL;
    struct netsock *netsock_ = NULL;

    rc = netsock_lookup_class(class_type);
    if (!rc) {
        OVAT_LOG(ERR, NETSOCK, "unknown sock class type %s\n", class_type);
        ret = -OVAT_ENOEXIST;
        goto err1;
    }
    
    netsock_ = rc->class->alloc();
    if (!netsock_) {
        OVAT_LOG(ERR, NETSOCK, "netsock_ alloc error\n");
        ret = -OVAT_ECALL;
        goto err1;
    }

    memset(netsock_, 0, sizeof(struct netsock));
    snprintf(netsock_->name, sizeof(netsock_->name), "%s", name);
    netsock_->conn_type = conn_type;
    snprintf(netsock_->path, sizeof(netsock_->path), "%s", path);
    netsock_->epollfd = netsock_epoll_init();
    netsock_->class = rc->class;
    INIT_LIST_HEAD(&(netsock_->conn_list));

    ret = rc->class->construct(netsock_);
    if (ret < 0) {
        OVAT_LOG(ERR, NETSOCK, "netsock construct error, name %s\n", name);
        ret = -OVAT_ECALL;
        goto err2;
    }

    ret = ovat_pthread_create(&(netsock_->thread_id), "netsock_events_thread", netsock_loop, (void *)netsock_);
    if (ret < 0) {
        OVAT_LOG(ERR, NETSOCK, "ovat_pthread_create failed, errno %d\n", errno);
        goto err2;
    }

    *netsockp = netsock_;

    return ret;

err2:
    rc->class->dealloc(netsock_);
err1:
    return ret;
}

int
netsock_close(struct netsock *netsock_)
{
    int ret = OVAT_EOK;

    netsock_->netsock_exit = true;

    ret = ovat_pthread_join(netsock_->thread_id, NULL);
    if (ret < 0) {
        OVAT_LOG(ERR, NETSOCK, "pthread join failed, errno %d\n", errno);
    }

    ret = netsock_->class->destruct(netsock_);
    if (ret < 0) {
        OVAT_LOG(ERR, NETSOCK, "netsock destruct failed, errno %d\n", errno);
    }

    ret = netsock_->class->dealloc(netsock_);
    if (ret < 0) {
        OVAT_LOG(ERR, NETSOCK, "netsock dealloc failed, errno %d\n", errno);
    }

    return ret;
}

OVAT_LOG_REGISTER(netsock_logtype, NETSOCK, INFO);
