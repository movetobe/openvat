#ifndef OVAT_PTHREAD_H
#define OVAT_PTHREAD_H

#define _GNU_SOURCE
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>

typedef pthread_mutex_t ovat_pthread_mutex_t;

int ovat_pthread_create(pthread_t *thread_id, const char *thread_name, void *(*start_func) (void *), void *arg);
int ovat_pthread_join(pthread_t thread_id, void **retval);

void ovat_pthread_mutex_init(ovat_pthread_mutex_t *mutex);
void ovat_pthread_mutex_lock(ovat_pthread_mutex_t *mutex);
void ovat_pthread_mutex_unlock(ovat_pthread_mutex_t *mutex);
int ovat_pthread_create_detach(pthread_t *thread_id, const char *thread_name,
                                    void *(*start_func) (void *), void *arg);


#endif /* OVAT_PTHREAD_H */

