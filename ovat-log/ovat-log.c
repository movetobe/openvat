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

/** The ovat_log structure. */
static struct ovat_logs {
	uint32_t type;  /**< Bitfield with enabled logs. */
	uint32_t level; /**< Log level. */
	FILE *file;     /**< Output file set by ovat_openlog_stream, or NULL. */
    char file_path[PATH_MAX];
} ovat_logs = {
	.type = ~0,
	.level = OVAT_LOG_DEBUG,
};

TAILQ_HEAD(ovat_module_loglevel_list, ovat_module_loglevel);

/** List of valid EAL log level options */
static struct ovat_module_loglevel_list module_loglevel_list =
	TAILQ_HEAD_INITIALIZER(module_loglevel_list);

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
ovat_log_get_level(struct ovat_module_loglevel *module)
{
    return module->level;
}

bool
ovat_log_can_log(struct ovat_module_loglevel *module, uint32_t level)
{
    int log_level;

    if (module == NULL) {
        return false;
    }
    if (level > ovat_log_get_global_level()) {
        return false;
    }

    log_level = ovat_log_get_level(module);
    if (log_level < 0) {
        return false;
    }
    if (level > (uint32_t)log_level) {
        return false;
    }

	return true;
}

int
ovat_log_set_level(struct ovat_module_loglevel *module, uint32_t level)
{
    module->level = level;
    return 0;
}

static struct ovat_module_loglevel *
ovat_log_lookup(const char *name)
{
    struct ovat_module_loglevel *module;

    TAILQ_FOREACH(module, &module_loglevel_list, next) {
        if (!strcmp(module->name, name)) {
            return module;
        }
    }
    return NULL;
}

/* register an extended log type */
void
ovat_log_register(const char *name, uint32_t level)
{
    struct ovat_module_loglevel *new_module;

    if (ovat_log_lookup(name) != NULL) {
        return;
    }

    new_module = calloc(1, sizeof(struct ovat_module_loglevel));
    snprintf(new_module->name, OVAT_LOG_MODULE_NAME, "%s", name);
    new_module->level = level;
    TAILQ_INSERT_HEAD(&module_loglevel_list, new_module, next);
}

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
ovat_vlog(uint32_t level, const char *name,
            const char *func_name, const char *format, va_list ap)
{
	FILE *f = ovat_log_get_stream();
	int ret;
    struct stat st;
    char new_path[PATH_MAX + sizeof(long long int)] = {0};
    struct ovat_module_loglevel *module = ovat_log_lookup(name);

    if (!ovat_log_can_log(module, level)) {
        return 0;
    }
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
    format_log_message(module->name, level, func_name,
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
ovat_log(uint32_t level, const char *module_name,
                const char *func_name, const char *format, ...)
{
    va_list ap;
    int ret;

    va_start(ap, format);
    ret = ovat_vlog(level, module_name, func_name, format, ap);
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

void
ovat_log_uninit(void)
{
    struct ovat_module_loglevel *module = NULL;
    struct ovat_module_loglevel *tmodule = NULL;

    TAILQ_FOREACH_SAFE(module, &module_loglevel_list, next, tmodule) {
        TAILQ_REMOVE(&module_loglevel_list, module, next);
        free(module);
    }
}


