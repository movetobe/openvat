/*
 * Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013, 2015, 2016 Nicira, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef DYNAMIC_STRING_H
#define DYNAMIC_STRING_H

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

/* A "dynamic string", that is, a buffer that can be used to construct a
 * string across a series of operations that extend or modify it.
 *
 * The 'string' member does not always point to a null-terminated string.
 * Initially it is NULL, and even when it is nonnull, some operations do not
 * ensure that it is null-terminated.  Use ds_cstr() to ensure that memory is
 * allocated for the string and that it is null-terminated. */
struct ds {
    char *string;       /* Null-terminated string. */
    size_t length;      /* Bytes used, not including null terminator. */
    size_t allocated;   /* Bytes allocated, not including null terminator. */
};

#define DS_EMPTY_INITIALIZER { NULL, 0, 0 }

void ds_init(struct ds *);
void ds_clear(struct ds *);

void ds_reserve(struct ds *, size_t min_length);
char *ds_put_uninit(struct ds *, size_t n);
static inline void ds_put_char(struct ds *, char);

void ds_put_char_multiple(struct ds *, char, size_t n);
void ds_put_cstr(struct ds *, const char *);
void ds_put_format(struct ds *, const char *, ...); //__ovat_format_printf(2, 3);
void ds_put_format_valist(struct ds *, const char *, va_list);
    //__ovat_format_printf(2, 0);

void ds_put_strftime_msec(struct ds *, const char *format, long long int when,
                          bool utc);
char *ds_cstr(struct ds *);
void ds_destroy(struct ds *);

/* Inline functions. */

void ds_put_char__(struct ds *, char);

static inline void
ds_put_char(struct ds *ds, char c)
{
    if (ds->length < ds->allocated) {
        ds->string[ds->length++] = c;
        ds->string[ds->length] = '\0';
    } else {
        ds_put_char__(ds, c);
    }
}

#endif /* DYNAMIC_STRING_H */
