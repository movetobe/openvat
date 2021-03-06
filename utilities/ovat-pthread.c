#include "ovat-utils.h"
#include "ovat-pthread.h"
#include "ovat-log.h"
#include <unistd.h>

int
ovat_pthread_create(pthread_t *thread_id, const char *thread_name,
                            void *(*start_func) (void *), void *arg)
{
    pthread_attr_t attr;
    int ret = OVAT_EOK;

    ret = pthread_attr_init(&attr);
    if (ret < 0) {
        OVAT_LOG(ERR, NETSOCK, "pthread_attr_init failed, errno %d\n", errno);
        goto err;
    }

    ret = pthread_create(thread_id, &attr, start_func, arg);
    if (ret < 0) {
        OVAT_LOG(ERR, NETSOCK, "pthread_create failed, errno %d\n", errno);
        goto err;
    }

    ret = pthread_setname_np(*thread_id, thread_name);
    if (ret < 0) {
        OVAT_LOG(ERR, NETSOCK, "pthread set name failed, errno %d\n", errno);
        goto err;
    }

err:
    ret = pthread_attr_destroy(&attr);
    return ret;
}

int
ovat_pthread_create_detach(pthread_t *thread_id, const char *thread_name,
                                    void *(*start_func) (void *), void *arg)
{
    pthread_attr_t attr;
    int ret = OVAT_EOK;

    ret = pthread_attr_init(&attr);
    if (ret < 0) {
        OVAT_LOG(ERR, NETSOCK, "pthread_attr_init failed, errno %d\n", errno);
        goto err;
    }

    ret = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if (ret < 0) {
        OVAT_LOG(ERR, NETSOCK, "pthread set detachstate failed, errno%d\n", errno);
        goto err;
    }
    ret = pthread_create(thread_id, &attr, start_func, arg);
    if (ret < 0) {
        OVAT_LOG(ERR, NETSOCK, "pthread_create failed, errno %d\n", errno);
        goto err;
    }
    ret = pthread_setname_np(*thread_id, thread_name);
    if (ret < 0) {
        OVAT_LOG(ERR, NETSOCK, "pthread set name failed, errno %d\n", errno);
        goto err;
    }

err:
    ret = pthread_attr_destroy(&attr);
    return ret;
}


int
ovat_pthread_join(pthread_t thread_id, void **retval)
{
    int ret = OVAT_EOK;

    ret = pthread_join(thread_id, retval);
    if (ret < 0) {
        OVAT_LOG(ERR, NETSOCK, "pthread join failed, errno %d\n", errno);
        goto err;
    }

err:
    return ret;
}

void
ovat_pthread_mutex_init(ovat_pthread_mutex_t *mutex)
{
    pthread_mutex_init(mutex, NULL);
}

void
ovat_pthread_mutex_lock(ovat_pthread_mutex_t *mutex)
{
    pthread_mutex_lock(mutex);
}

void
ovat_pthread_mutex_unlock(ovat_pthread_mutex_t *mutex)
{
    pthread_mutex_unlock(mutex);
}


