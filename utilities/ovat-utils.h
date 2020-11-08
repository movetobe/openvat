#ifndef NETSOCK_UTILS_H
#define NETSOCK_UTILS_H

#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

enum netsock_errno {
    OVAT_EOK, /* normal return */
    OVAT_ENOEXIST, /* something not exist */
    OVAT_EEXIST, /* something exist */
    OVAT_ESYSCALL, /* syscall error */
    OVAT_ECALL, /* call error */
};

#ifndef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif

#ifndef container_of
/**
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:    the pointer to the member.
 * @type:   the type of the container struct this is embedded in.
 * @member: the name of the member within the struct.
 *
 */
#define container_of(ptr, type, member) ({                  \
    const typeof(((type *)0)->member) * __mptr = (ptr);     \
    (type *)((char *)__mptr - offsetof(type, member)); })
#endif

typedef pthread_mutex_t ovat_pthread_mutex_t;

int ovat_pthread_create(pthread_t *thread_id, const char *thread_name, void *(*start_func) (void *), void *arg);
int ovat_pthread_join(pthread_t thread_id, void **retval);

void ovat_pthread_mutex_init(ovat_pthread_mutex_t *mutex);
void ovat_pthread_mutex_lock(ovat_pthread_mutex_t *mutex);
void ovat_pthread_mutex_unlock(ovat_pthread_mutex_t *mutex);

#endif /* NETSOCK_UTILS_H */