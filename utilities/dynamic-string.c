/*
 * Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013, 2016 Nicira, Inc.
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


#include "dynamic-string.h"
#include "ovat-utils.h"
#include "timeval.h"
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Initializes 'ds' as an empty string buffer. */
void
ds_init(struct ds *ds)
{
    ds->string = NULL;
    ds->length = 0;
    ds->allocated = 0;
}

/* Sets 'ds''s length to 0, effectively clearing any existing content.  Does
 * not free any memory. */
void
ds_clear(struct ds *ds)
{
    ds->length = 0;
}

/* Ensures that at least 'min_length + 1' bytes (including space for a null
 * terminator) are allocated for ds->string, allocating or reallocating memory
 * as necessary. */
void
ds_reserve(struct ds *ds, size_t min_length)
{
    if (min_length > ds->allocated || !ds->string) {
        ds->allocated += MAX(min_length, ds->allocated);
        ds->allocated = MAX(8, ds->allocated);
        ds->string = realloc(ds->string, ds->allocated + 1);
    }
}

/* Appends space for 'n' bytes to the end of 'ds->string', increasing
 * 'ds->length' by the same amount, and returns the first appended byte.  The
 * caller should fill in all 'n' bytes starting at the return value. */
char *
ds_put_uninit(struct ds *ds, size_t n)
{
    ds_reserve(ds, ds->length + n);
    ds->length += n;
    ds->string[ds->length] = '\0';
    return &ds->string[ds->length - n];
}

void
ds_put_char__(struct ds *ds, char c)
{
    *ds_put_uninit(ds, 1) = c;
}

void
ds_put_char_multiple(struct ds *ds, char c, size_t n)
{
    memset(ds_put_uninit(ds, n), c, n);
}

void
ds_put_cstr(struct ds *ds, const char *s)
{
    size_t s_len = strlen(s);
    memcpy(ds_put_uninit(ds, s_len), s, s_len);
}

void
ds_put_format(struct ds *ds, const char *format, ...)
{
    va_list args;

    va_start(args, format);
    ds_put_format_valist(ds, format, args);
    va_end(args);
}

void
ds_put_format_valist(struct ds *ds, const char *format, va_list args_)
{
    va_list args;
    size_t available;
    int needed;

    va_copy(args, args_);
    available = ds->string ? ds->allocated - ds->length + 1 : 0;
    needed = vsnprintf(&ds->string[ds->length], available, format, args);
    va_end(args);

    if (needed < available) {
        ds->length += needed;
    } else {
        ds_reserve(ds, ds->length + needed);

        va_copy(args, args_);
        available = ds->allocated - ds->length + 1;
        needed = vsnprintf(&ds->string[ds->length], available, format, args);
        va_end(args);

        ds->length += needed;
    }
}

/* Writes the current time with optional millisecond resolution to 'string'
 * based on 'template'.
 * The current time is either localtime or UTC based on 'utc'. */
void
ds_put_strftime_msec(struct ds *ds, const char *template, long long int when,
                     bool utc)
{
    struct tm_msec tm;
    if (utc) {
        gmtime_msec(when, &tm);
    } else {
        localtime_msec(when, &tm);
    }

    for (;;) {
        size_t avail = ds->string ? ds->allocated - ds->length + 1 : 0;
        size_t used = strftime_msec(&ds->string[ds->length], avail, template,
                                    &tm);
        if (used) {
            ds->length += used;
            return;
        }
        ds_reserve(ds, ds->length + (avail < 32 ? 64 : 2 * avail));
    }
}

char *
ds_cstr(struct ds *ds)
{
    if (!ds->string) {
        ds_reserve(ds, 0);
    }
    ds->string[ds->length] = '\0';
    return ds->string;
}


void
ds_destroy(struct ds *ds)
{
    free(ds->string);
}

