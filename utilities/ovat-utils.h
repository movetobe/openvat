/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) 2010-2014 Intel Corporation
 */
/* SPDX-License-Identifier: GPL-2.0 */

#ifndef OVAT_UTILS_H
#define OVAT_UTILS_H

#define _GNU_SOURCE
#include <pthread.h>
#include <sys/types.h>
#include <sys/queue.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include "ovat-pthread.h"

enum ovat_errno {
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

#ifndef MIN
#define MIN(X, Y) ((X) < (Y) ? (X) : (Y))
#endif

#ifndef MAX
#define MAX(X, Y) ((X) > (Y) ? (X) : (Y))
#endif

/* Returns X rounded down to the nearest multiple of Y. */
#define ROUND_DOWN(X, Y) ((X) / (Y) * (Y))

/* This is a void expression that issues a compiler error if POINTER cannot be
 * compared for equality with the given pointer TYPE.  This generally means
 * that POINTER is a qualified or unqualified TYPE.  However,
 * BUILD_ASSERT_TYPE(POINTER, void *) will accept any pointer to object type,
 * because any pointer to object can be compared for equality with "void *".
 *
 * POINTER can be any expression.  The use of "sizeof" ensures that the
 * expression is not actually evaluated, so that any side effects of the
 * expression do not occur.
 *
 * The cast to int is present only to suppress an "expression using sizeof
 * bool" warning from "sparse" (see
 * http://permalink.gmane.org/gmane.comp.parsers.sparse/2967). */
#define BUILD_ASSERT_TYPE(POINTER, TYPE) \
    ((void) sizeof ((int) ((POINTER) == (TYPE) (POINTER))))

/* Casts 'pointer' to 'type' and issues a compiler warning if the cast changes
 * anything other than an outermost "const" or "volatile" qualifier. */
#define CONST_CAST(TYPE, POINTER)                               \
    (BUILD_ASSERT_TYPE(POINTER, TYPE),                          \
     (TYPE) (POINTER))

/*#define __ovat_format_printf(format_index, first_arg) \
	__attribute__((format(gnu_printf, format_index, first_arg)))*/

    /** Number of elements in the array. */
#define	OVAT_DIM(a)	(sizeof (a) / sizeof ((a)[0]))

/* The minimum or maximum value of a signed or unsigned integer TYPE. */
#define TYPE_MINIMUM(TYPE) (TYPE_IS_SIGNED(TYPE) ? -TYPE_MAXIMUM(TYPE) - 1 : 0)
#define TYPE_MAXIMUM(TYPE) \
    ((((TYPE)1 << (TYPE_VALUE_BITS(TYPE) - 1)) - 1) * 2 + 1)
#define OVAT_STR(x) #x

/* This macro permits both remove and free var within the loop safely.*/
#ifndef TAILQ_FOREACH_SAFE
#define TAILQ_FOREACH_SAFE(var, head, field, tvar)		\
	for ((var) = TAILQ_FIRST((head));			\
	    (var) && ((tvar) = TAILQ_NEXT((var), field), 1);	\
	    (var) = (tvar))
#endif

#endif /* OVAT_UTILS_H */
