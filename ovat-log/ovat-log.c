/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) 2010-2014 Intel Corporation
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <regex.h>
#include <fnmatch.h>
#include <ctype.h>
#include <linux/limits.h>

#include <sys/types.h>
#include <syslog.h>
#include <sys/queue.h>
#include <sys/stat.h>

#include "ovat-log.h"
#include "ovat-utils.h"
#include "dynamic-string.h"
#include "timeval.h"

#define OVAT_LOG_FILE_MAX 2000000 /* 2MB */
struct ovat_log_dynamic_type {
	const char *name;
	uint32_t loglevel;
};

/** The ovat_log structure. */
static struct ovat_logs {
	uint32_t type;  /**< Bitfield with enabled logs. */
	uint32_t level; /**< Log level. */
	FILE *file;     /**< Output file set by ovat_openlog_stream, or NULL. */
    char file_path[PATH_MAX];
	size_t dynamic_types_len;
	struct ovat_log_dynamic_type *dynamic_types;
} ovat_logs = {
	.type = ~0,
	.level = OVAT_LOG_DEBUG,
};

struct ovat_opt_loglevel {
	/** Next list entry */
	TAILQ_ENTRY(ovat_opt_loglevel) next;
	/** Compiled regular expression obtained from the option */
	regex_t re_match;
	/** Globbing pattern option */
	char *pattern;
	/** Log level value obtained from the option */
	uint32_t level;
};

TAILQ_HEAD(ovat_opt_loglevel_list, ovat_opt_loglevel);

/** List of valid EAL log level options */
static struct ovat_opt_loglevel_list opt_loglevel_list =
	TAILQ_HEAD_INITIALIZER(opt_loglevel_list);

/* Stream to use for logging if ovat_logs.file is NULL */
static FILE *default_log_stream;

/* Change the stream that will be used by logging system */
int
ovat_openlog_stream(FILE *f)
{
	ovat_logs.file = f;
	return 0;
}

FILE *
ovat_log_get_stream(void)
{
	FILE *f = ovat_logs.file;

	if (f == NULL) {
		/*
		 * Grab the current value of stderr here, rather than
		 * just initializing default_log_stream to stderr. This
		 * ensures that we will always use the current value
		 * of stderr, even if the application closes and
		 * reopens it.
		 */
		return default_log_stream ? : stderr;
	}
	return f;
}

/* Set global log level */
void
ovat_log_set_global_level(uint32_t level)
{
	ovat_logs.level = (uint32_t)level;
}

/* Get global log level */
uint32_t
ovat_log_get_global_level(void)
{
	return ovat_logs.level;
}

int
ovat_log_get_level(uint32_t type)
{
	if (type >= ovat_logs.dynamic_types_len)
		return -1;

	return ovat_logs.dynamic_types[type].loglevel;
}

bool
ovat_log_can_log(uint32_t logtype, uint32_t level)
{
	int log_level;

	if (level > ovat_log_get_global_level())
		return false;

	log_level = ovat_log_get_level(logtype);
	if (log_level < 0)
		return false;

	if (level > (uint32_t)log_level)
		return false;

	return true;
}

int
ovat_log_set_level(uint32_t type, uint32_t level)
{
	if (type >= ovat_logs.dynamic_types_len)
		return -1;
	if (level > OVAT_LOG_DEBUG)
		return -1;

	ovat_logs.dynamic_types[type].loglevel = level;

	return 0;
}

/* set log level by regular expression */
int
ovat_log_set_level_regexp(const char *regex, uint32_t level)
{
	regex_t r;
	size_t i;

	if (level > OVAT_LOG_DEBUG)
		return -1;

	if (regcomp(&r, regex, 0) != 0)
		return -1;

	for (i = 0; i < ovat_logs.dynamic_types_len; i++) {
		if (ovat_logs.dynamic_types[i].name == NULL)
			continue;
		if (regexec(&r, ovat_logs.dynamic_types[i].name, 0,
				NULL, 0) == 0)
			ovat_logs.dynamic_types[i].loglevel = level;
	}

	regfree(&r);

	return 0;
}

/*
 * Save the type string and the loglevel for later dynamic
 * logtypes which may register later.
 */
static int ovat_log_save_level(int priority,
			      const char *regex, const char *pattern)
{
	struct ovat_opt_loglevel *opt_ll = NULL;

	opt_ll = malloc(sizeof(*opt_ll));
	if (opt_ll == NULL)
		goto fail;

	opt_ll->level = priority;

	if (regex) {
		opt_ll->pattern = NULL;
		if (regcomp(&opt_ll->re_match, regex, 0) != 0)
			goto fail;
	} else if (pattern) {
		opt_ll->pattern = strdup(pattern);
		if (opt_ll->pattern == NULL)
			goto fail;
	} else
		goto fail;

	TAILQ_INSERT_HEAD(&opt_loglevel_list, opt_ll, next);
	return 0;
fail:
	free(opt_ll);
	return -1;
}

int ovat_log_save_regexp(const char *regex, int tmp)
{
	return ovat_log_save_level(tmp, regex, NULL);
}

/* set log level based on globbing pattern */
int
ovat_log_set_level_pattern(const char *pattern, uint32_t level)
{
	size_t i;

	if (level > OVAT_LOG_DEBUG)
		return -1;

	for (i = 0; i < ovat_logs.dynamic_types_len; i++) {
		if (ovat_logs.dynamic_types[i].name == NULL)
			continue;

		if (fnmatch(pattern, ovat_logs.dynamic_types[i].name, 0) == 0)
			ovat_logs.dynamic_types[i].loglevel = level;
	}

	return 0;
}

int ovat_log_save_pattern(const char *pattern, int priority)
{
	return ovat_log_save_level(priority, NULL, pattern);
}

static int
ovat_log_lookup(const char *name)
{
	size_t i;

	for (i = 0; i < ovat_logs.dynamic_types_len; i++) {
		if (ovat_logs.dynamic_types[i].name == NULL)
			continue;
		if (strcmp(name, ovat_logs.dynamic_types[i].name) == 0)
			return i;
	}

	return -1;
}

/* register an extended log type, assuming table is large enough, and id
 * is not yet registered.
 */
static int
__ovat_log_register(const char *name, int id)
{
	char *dup_name = strdup(name);

	if (dup_name == NULL)
		return -ENOMEM;

	ovat_logs.dynamic_types[id].name = dup_name;
	ovat_logs.dynamic_types[id].loglevel = OVAT_LOG_INFO;

	return id;
}

/* register an extended log type */
int
ovat_log_register(const char *name)
{
	struct ovat_log_dynamic_type *new_dynamic_types;
	int id, ret;

	id = ovat_log_lookup(name);
	if (id >= 0)
		return id;

	new_dynamic_types = realloc(ovat_logs.dynamic_types,
		sizeof(struct ovat_log_dynamic_type) *
		(ovat_logs.dynamic_types_len + 1));
	if (new_dynamic_types == NULL)
		return -ENOMEM;
	ovat_logs.dynamic_types = new_dynamic_types;

	ret = __ovat_log_register(name, ovat_logs.dynamic_types_len);
	if (ret < 0)
		return ret;

	ovat_logs.dynamic_types_len++;

	return ret;
}

/* Register an extended log type and try to pick its level from EAL options */
int
ovat_log_register_type_and_pick_level(const char *name, uint32_t level_def)
{
	struct ovat_opt_loglevel *opt_ll;
	uint32_t level = level_def;
	int type;

	type = ovat_log_register(name);
	if (type < 0)
		return type;

	TAILQ_FOREACH(opt_ll, &opt_loglevel_list, next) {
		if (opt_ll->level > OVAT_LOG_DEBUG)
			continue;

		if (opt_ll->pattern) {
			if (fnmatch(opt_ll->pattern, name, 0) == 0)
				level = opt_ll->level;
		} else {
			if (regexec(&opt_ll->re_match, name, 0, NULL, 0) == 0)
				level = opt_ll->level;
		}
	}

	ovat_logs.dynamic_types[type].loglevel = level;

	return type;
}

struct logtype {
	uint32_t log_id;
	const char *logtype;
};

static const struct logtype logtype_strings[] = {
    {OVAT_LOGTYPE_CORE, "ovat.core"},
    {OVAT_LOGTYPE_NETSOCK, "ovat.netsock"},
    {OVAT_LOGTYPE_IF, "ovat.if"},
    {OVAT_LOGTYPE_NMIF, "ovat.nmif"},
    {OVAT_LOGTYPE_CANNMIF, "ovat.cannmif"},
    {OVAT_LOGTYPE_NMSTUB, "ovat.nmstub"},
    {OVAT_LOGTYPE_CANNMSTUB, "ovat.cannmstub"},
    {OVAT_LOGTYPE_BSWMSTUB, "ovat.bswmstub"},
    {OVAT_LOGTYPE_CANIFSTUB, "ovat.canifstub"},
    {OVAT_LOGTYPE_CANSMSTUB, "ovat.cansmstub"},
    {OVAT_LOGTYPE_COMSTUB, "ovat.comstub"},
    {OVAT_LOGTYPE_COMMSTUB, "ovat.commstub"},
    {OVAT_LOGTYPE_DETSTUB, "ovat.detstub"},
    {OVAT_LOGTYPE_PDURSTUB, "ovat.pdurstub"},
};

static const char *
loglevel_to_string(uint32_t level)
{
	switch (level) {
	    case 0:
            return "disabled";
	    case OVAT_LOG_EMERG:
            return "emerg";
	    case OVAT_LOG_ALERT:
            return "alert";
	    case OVAT_LOG_CRIT:
            return "critical";
	    case OVAT_LOG_ERR:
            return "error";
	    case OVAT_LOG_WARNING:
            return "warning";
	    case OVAT_LOG_NOTICE:
            return "notice";
	    case OVAT_LOG_INFO:
            return "info";
	    case OVAT_LOG_DEBUG:
            return "debug";
	    default:
	        return "unknown";
	}
}

/* dump global level and registered log types */
void
ovat_log_dump(FILE *f)
{
	size_t i;

	fprintf(f, "global log level is %s\n",
		loglevel_to_string(ovat_log_get_global_level()));

	for (i = 0; i < ovat_logs.dynamic_types_len; i++) {
		if (ovat_logs.dynamic_types[i].name == NULL)
			continue;
		fprintf(f, "id %zu: %s, level is %s\n",
			i, ovat_logs.dynamic_types[i].name,
			loglevel_to_string(ovat_logs.dynamic_types[i].loglevel));
	}
}

const char *ovat_log_pattern = "%D{%Y-%m-%dT%H:%M:%S.###Z}|%P|%c|%p|%f%m";

/* Similar to strlcpy() from OpenBSD, but it never reads more than 'size - 1'
 * bytes from 'src' and doesn't return anything. */
static void
ovat_strlcpy(char *dst, const char *src, size_t size)
{
    if (size > 0) {
        size_t len = strnlen(src, size - 1);
        memcpy(dst, src, len);
        dst[len] = '\0';
    }
}


static const char *
fetch_braces(const char *p, const char *def, char *out, size_t out_size)
{
    if (*p == '{') {
        size_t n = strcspn(p + 1, "}");
        size_t n_copy = MIN(n, out_size - 1);
        memcpy(out, p + 1, n_copy);
        out[n_copy] = '\0';
        p += n + 2;
    } else {
        ovat_strlcpy(out, def, out_size);
    }
    return p;
}

static void
format_log_message(const char *module, uint32_t level,
                    const char *func_name, const char *pattern,
                    const char *message, va_list args_, struct ds *s)
{
    char tmp[128];
    va_list args;
    const char *p;

    ds_clear(s);
    for (p = pattern; *p != '\0'; ) {
        enum { LEFT, RIGHT } justify = RIGHT;
        int pad = ' ';
        size_t length, field, used;

        if (*p != '%') {
            ds_put_char(s, *p++);
            continue;
        }

        p++;
        if (*p == '-') {
            justify = LEFT;
            p++;
        }
        if (*p == '0') {
            pad = '0';
            p++;
        }
        field = 0;
        while (isdigit((unsigned char)*p)) {
            field = (field * 10) + (*p - '0');
            p++;
        }

        length = s->length;
        switch (*p++) {
        case 'c':
            p = fetch_braces(p, "", tmp, sizeof tmp);
            ds_put_cstr(s, module);
            break;
        case 'd':
            p = fetch_braces(p, "%Y-%m-%d %H:%M:%S.###", tmp, sizeof tmp);
            ds_put_strftime_msec(s, tmp, time_wall_msec(), false);
            break;
        case 'D':
            p = fetch_braces(p, "%Y-%m-%d %H:%M:%S.###", tmp, sizeof tmp);
            ds_put_strftime_msec(s, tmp, time_wall_msec(), true);
            break;
        case 'E':
            gethostname(tmp, sizeof tmp);
            tmp[sizeof tmp - 1] = '\0';
            ds_put_cstr(s, tmp);
            break;
        case 'f':
            ds_put_format(s, "%s", func_name);
            break;
        case 'm':
            /* Format user-supplied log message and trim trailing new-lines. */
            length = s->length;
            va_copy(args, args_);
            ds_put_format_valist(s, message, args);
            va_end(args);
            while (s->length > length && s->string[s->length - 1] == '\n') {
                s->length--;
            }
            break;
        case 'n':
            ds_put_char(s, '\n');
            break;
        case 'p':
            ds_put_cstr(s, loglevel_to_string(level));
            break;
        case 'P':
            ds_put_format(s, "%ld", (long int) getpid());
            break;
        default:
            ds_put_char(s, p[-1]);
            break;
        }
        used = s->length - length;
        if (used < field) {
            size_t n_pad = field - used;
            if (justify == RIGHT) {
                ds_put_uninit(s, n_pad);
                memmove(&s->string[length + n_pad], &s->string[length], used);
                memset(&s->string[length], pad, n_pad);
            } else {
                ds_put_char_multiple(s, pad, n_pad);
            }
        }
    }
    ds_put_char(s, '\n');
}


/*
 * Generates a log message The message will be sent in the stream
 * defined by the previous call to ovat_openlog_stream().
 */
int
ovat_vlog(uint32_t level, uint32_t logtype,
            const char *func_name, const char *format, va_list ap)
{
	FILE *f = ovat_log_get_stream();
	int ret;
    struct stat st;
    char new_path[PATH_MAX + sizeof(long long int)] = {0};

	if (logtype >= ovat_logs.dynamic_types_len)
		return -1;
	if (!ovat_log_can_log(logtype, level))
		return 0;
    stat(ovat_logs.file_path, &st);
    snprintf(new_path, PATH_MAX + sizeof(long long int), "%s.%lld", ovat_logs.file_path, time_wall_msec());
    if (st.st_size > OVAT_LOG_FILE_MAX) {
        /* log file overflow, store/close old one and open a new one */
        (void)rename(ovat_logs.file_path, new_path);
        fclose(f);
        f = fopen(ovat_logs.file_path, "w+");
        ovat_openlog_stream(f);
    }

    struct ds s;
    ds_init(&s);
    format_log_message(logtype_strings[logtype].logtype, level, func_name,
        ovat_log_pattern, format, ap, &s);

	ret = vfprintf(f, s.string, ap);
	fflush(f);

    ds_destroy(&s);
	return ret;
}

/*
 * Generates a log message The message will be sent in the stream
 * defined by the previous call to ovat_openlog_stream().
 * No need to check level here, done by ovat_vlog().
 */
int
ovat_log(uint32_t level, uint32_t logtype, const char *func_name, const char *format, ...)
{
	va_list ap;
	int ret;

	va_start(ap, format);
	ret = ovat_vlog(level, logtype, func_name, format, ap);
	va_end(ap);
	return ret;
}

void
ovat_log_init(const char *path)
{
    FILE *fp;

    snprintf(ovat_logs.file_path, PATH_MAX, "%s", path);
    fp = fopen(ovat_logs.file_path, "w+");
    ovat_openlog_stream(fp);
}

