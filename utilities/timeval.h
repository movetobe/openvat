/*
 * Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 Nicira, Inc.
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

#ifndef TIMEVAL_H
#define TIMEVAL_H

#include <time.h>
#include "ovat-utils.h"

#define TIME_MAX TYPE_MAXIMUM(time_t)
#define TIME_MIN TYPE_MINIMUM(time_t)

struct tm_msec {
  struct tm tm;
  int msec;
};

time_t time_wall(void);
long long int time_wall_msec(void);
long long int time_wall_usec(void);

void time_wall_timespec(struct timespec *);

long long int timespec_to_msec(const struct timespec *);
long long int timespec_to_usec(const struct timespec *);
long long int timeval_to_msec(const struct timeval *);
long long int timeval_to_usec(const struct timeval *);

struct tm_msec *localtime_msec(long long int now, struct tm_msec *result);
struct tm_msec *gmtime_msec(long long int now, struct tm_msec *result);
size_t strftime_msec(char *s, size_t max, const char *format,
                     const struct tm_msec *);

void nsec_to_timespec(long long int, struct timespec *);

void ovat_msleep(unsigned int msecs);

#endif
