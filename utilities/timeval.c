/*
 * Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013, 2014, 2015 Nicira, Inc.
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

#include "timeval.h"
#include <errno.h>
#include <poll.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>

static void
time_timespec__(struct timespec *ts)
{
    clock_gettime(CLOCK_REALTIME, ts);
}

/* Stores the current time into '*ts'. */
void
time_wall_timespec(struct timespec *ts)
{
    time_timespec__(ts);
}

static time_t
time_sec__(void)
{
    struct timespec ts;

    time_timespec__(&ts);
    return ts.tv_sec;
}

/* Returns the current time, in seconds. */
time_t
time_wall(void)
{
    return time_sec__();
}

static long long int
time_msec__(void)
{
    struct timespec ts;

    time_timespec__(&ts);
    return timespec_to_msec(&ts);
}


/* Returns the current time, in ms. */
long long int
time_wall_msec(void)
{
    return time_msec__();
}

static long long int
time_usec__(void)
{
    struct timespec ts;

    time_timespec__(&ts);
    return timespec_to_usec(&ts);
}

/* Returns the current time, in microseconds. */
long long int
time_wall_usec(void)
{
    return time_usec__();
}


long long int
timespec_to_msec(const struct timespec *ts)
{
    return (long long int) ts->tv_sec * 1000 + ts->tv_nsec / (1000 * 1000);
}

long long int
timeval_to_msec(const struct timeval *tv)
{
    return (long long int) tv->tv_sec * 1000 + tv->tv_usec / 1000;
}

long long int
timespec_to_usec(const struct timespec *ts)
{
    return (long long int) ts->tv_sec * 1000 * 1000 + ts->tv_nsec / 1000;
}

long long int
timeval_to_usec(const struct timeval *tv)
{
    return (long long int) tv->tv_sec * 1000 * 1000 + tv->tv_usec;
}

void
nsec_to_timespec(long long int nsec, struct timespec *ts)
{
    if (!nsec) {
        ts->tv_sec = ts->tv_nsec = 0;
        return;
    }
    ts->tv_sec = nsec / (1000 * 1000 * 1000);

    nsec = nsec % (1000 * 1000 * 1000);
    /* This is to handle dates before epoch. */
    if (nsec < 0) {
        nsec += 1000 * 1000 * 1000;
        ts->tv_sec--;
    }

    ts->tv_nsec = nsec;
}

/* strftime() with an extension for high-resolution timestamps.  Any '#'s in
 * 'format' will be replaced by subseconds, e.g. use "%S.###" to obtain results
 * like "01.123".  */
size_t
strftime_msec(char *s, size_t max, const char *format,
              const struct tm_msec *tm)
{
    size_t n;

    /* Visual Studio 2013's behavior is to crash when 0 is passed as second
     * argument to strftime. */
    n = max ? strftime(s, max, format, &tm->tm) : 0;
    if (n) {
        char decimals[4];
        char *p;

        sprintf(decimals, "%03d", tm->msec);
        for (p = strchr(s, '#'); p; p = strchr(p, '#')) {
            char *d = decimals;
            while (*p == '#')  {
                *p++ = *d ? *d++ : '0';
            }
        }
    }

    return n;
}

struct tm_msec *
localtime_msec(long long int now, struct tm_msec *result)
{
  time_t now_sec = now / 1000;
  localtime_r(&now_sec, &result->tm);
  result->msec = now % 1000;
  return result;
}

struct tm_msec *
gmtime_msec(long long int now, struct tm_msec *result)
{
  time_t now_sec = now / 1000;
  gmtime_r(&now_sec, &result->tm);
  result->msec = now % 1000;
  return result;
}


void
ovat_msleep(unsigned int msecs)
{
    unsigned secs = msecs / 1000;
    unsigned usecs = (msecs % 1000) * 1000;

    if (secs > 0) {
        sleep(secs);
    }
    if (usecs > 0) {
        usleep(usecs);
    }
}
