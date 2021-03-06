/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) 2010-2014 Intel Corporation
 */

#ifndef OVAT_LOG_H
#define OVAT_LOG_H

#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <sys/queue.h>
#include "ovat-utils.h"

#define OVAT_LOG_MODULE_NAME 32

struct ovat_module_loglevel {
	/** Next list entry */
	TAILQ_ENTRY(ovat_module_loglevel) next;
	/** module name */
	char name[OVAT_LOG_MODULE_NAME];
	/** Log level value obtained from the option */
	uint32_t level;
};

/* Can't use 0, as it gives compiler warnings */
#define OVAT_LOG_EMERG    1U  /**< System is unusable.               */
#define OVAT_LOG_ALERT    2U  /**< Action must be taken immediately. */
#define OVAT_LOG_CRIT     3U  /**< Critical conditions.              */
#define OVAT_LOG_ERR      4U  /**< Error conditions.                 */
#define OVAT_LOG_WARNING  5U  /**< Warning conditions.               */
#define OVAT_LOG_NOTICE   6U  /**< Normal but significant condition. */
#define OVAT_LOG_INFO     7U  /**< Informational.                    */
#define OVAT_LOG_DEBUG    8U  /**< Debug-level messages.             */

/**
 * Change the stream that will be used by the logging system.
 *
 * This can be done at any time. The f argument represents the stream
 * to be used to send the logs. If f is NULL, the default output is
 * used (stderr).
 *
 * @param f
 *   Pointer to the stream.
 * @return
 *   - 0 on success.
 *   - Negative on error.
 */
int ovat_openlog_stream(FILE *f);

/**
 * Retrieve the stream used by the logging system (see ovat_openlog_stream()
 * to change it).
 *
 * @return
 *   Pointer to the stream.
 */
FILE *ovat_log_get_stream(void);

/**
 * Set the global log level.
 *
 * After this call, logs with a level lower or equal than the level
 * passed as argument will be displayed.
 *
 * @param level
 *   Log level. A value between OVAT_LOG_EMERG (1) and OVAT_LOG_DEBUG (8).
 */
void ovat_log_set_global_level(uint32_t level);

/**
 * Get the global log level.
 *
 * @return
 *   The current global log level.
 */
uint32_t ovat_log_get_global_level(void);

/**
 * Get the log level for a given type.
 *
 * @param logtype
 *   The log type identifier.
 * @return
 *   0 on success, a negative value if logtype is invalid.
 */
int ovat_log_get_level(struct ovat_module_loglevel *module);

/**
 * For a given `logtype`, check if a log with `loglevel` can be printed.
 *
 * @param logtype
 *   The log type identifier
 * @param loglevel
 *   Log level. A value between OVAT_LOG_EMERG (1) and OVAT_LOG_DEBUG (8).
 * @return
 * Returns 'true' if log can be printed and 'false' if it can't.
 */
bool ovat_log_can_log(struct ovat_module_loglevel *module, uint32_t loglevel);

/**
 * Set the log level for a given type.
 *
 * @param logtype
 *   The log type identifier.
 * @param level
 *   The level to be set.
 * @return
 *   0 on success, a negative value if logtype or level is invalid.
 */
int ovat_log_set_level(struct ovat_module_loglevel *module, uint32_t level);

/**
 * Register a dynamic log type
 *
 * If a log is already registered with the same type, the returned value
 * is the same than the previous one.
 *
 * @param name
 *   The string identifying the log type.
 * @return
 *   - >0: success, the returned value is the log type identifier.
 *   - (-ENOMEM): cannot allocate memory.
 */
void ovat_log_register(const char *name, uint32_t level);

/**
 * Generates a log message.
 *
 * The message will be sent in the stream defined by the previous call
 * to ovat_openlog_stream().
 *
 * The level argument determines if the log should be displayed or
 * not, depending on the loglevel settings.
 *
 * The preferred alternative is the OVAT_LOG() because it adds the
 * level and type in the logged string.
 *
 * @param level
 *   Log level. A value between OVAT_LOG_EMERG (1) and OVAT_LOG_DEBUG (8).
 * @param logtype
 *   The log type, for example, OVAT_LOGTYPE_EAL.
 * @param format
 *   The format string, as in printf(3), followed by the variable arguments
 *   required by the format.
 * @return
 *   - 0: Success.
 *   - Negative on error.
 */
int ovat_log(uint32_t level, const char *name,
            const char *func_name, const char *format, ...);

/**
 * Generates a log message.
 *
 * The message will be sent in the stream defined by the previous call
 * to ovat_openlog_stream().
 *
 * The level argument determines if the log should be displayed or
 * not, depending on the loglevel settings. A trailing
 * newline may be added if needed.
 *
 * The preferred alternative is the OVAT_LOG() because it adds the
 * level and type in the logged string.
 *
 * @param level
 *   Log level. A value between OVAT_LOG_EMERG (1) and OVAT_LOG_DEBUG (8).
 * @param logtype
 *   The log type, for example, OVAT_LOGTYPE_EAL.
 * @param format
 *   The format string, as in printf(3), followed by the variable arguments
 *   required by the format.
 * @param ap
 *   The va_list of the variable arguments required by the format.
 * @return
 *   - 0: Success.
 *   - Negative on error.
 */
int ovat_vlog(uint32_t level, const char *name,
                const char *func_name, const char *format, va_list ap);

void ovat_log_init(const char *path);
void ovat_log_uninit(void);
/**
 * Generates a log message.
 *
 * The OVAT_LOG() is a helper that prefixes the string with the log level
 * and type, and call ovat_log().
 *
 * @param l
 *   Log level. A value between EMERG (1) and DEBUG (8). The short name is
 *   expanded by the macro, so it cannot be an integer value.
 * @param t
 *   The log type, for example, EAL. The short name is expanded by the
 *   macro, so it cannot be an integer value.
 * @param ...
 *   The fmt string, as in printf(3), followed by the variable arguments
 *   required by the format.
 * @return
 *   - 0: Success.
 *   - Negative on error.
 */
#define OVAT_LOG(l, t, ...)					\
	 ovat_log(OVAT_LOG_ ## l,					\
		# t, __func__, ": " __VA_ARGS__)

/**
 * Generates a log message for data path.
 *
 * Similar to OVAT_LOG(), except that it is removed at compilation time
 * if the OVAT_LOG_DP_LEVEL configuration option is lower than the log
 * level argument.
 *
 * @param l
 *   Log level. A value between EMERG (1) and DEBUG (8). The short name is
 *   expanded by the macro, so it cannot be an integer value.
 * @param t
 *   The log type, for example, EAL. The short name is expanded by the
 *   macro, so it cannot be an integer value.
 * @param ...
 *   The fmt string, as in printf(3), followed by the variable arguments
 *   required by the format.
 * @return
 *   - 0: Success.
 *   - Negative on error.
 */
#define OVAT_LOG_DP(l, t, ...)					\
	(void)((OVAT_LOG_ ## l <= OVAT_LOG_DP_LEVEL) ?		\
	 ovat_log(OVAT_LOG_ ## l,					\
		 OVAT_LOGTYPE_ ## t, # t ": " __VA_ARGS__) :	\
	 0)

#define OVAT_PRIORITY_LAST 65535

#define OVAT_PRIO(prio) \
	OVAT_PRIORITY_ ## prio

/**
 * Run function before main() with high priority.
 *
 * @param func
 *   Constructor function.
 * @param prio
 *   Priority number must be above 100.
 *   Lowest number is the first to run.
 */
#define OVAT_INIT_PRIO(func, prio) \
static void __attribute__((constructor(OVAT_PRIO(prio)), used)) func(void)

/**
 * Run function before main() with low priority.
 *
 * The constructor will be run after prioritized constructors.
 *
 * @param func
 *   Constructor function.
 */
#define OVAT_INIT(func) \
	OVAT_INIT_PRIO(func, LAST)

/**
 * @warning
 * @b EXPERIMENTAL: this API may change without prior notice
 *
 * Register a dynamic log type in constructor context with its name and level.
 *
 * It is a wrapper macro for declaring the logtype, register the log and
 * sets it's level in the constructor context.
 *
 * @param type
 *   The log type identifier
 * @param name
 *    Name for the log type to be registered
 * @param level
 *   Log level. A value between EMERG (1) and DEBUG (8).
 */
#define OVAT_LOG_REGISTER(type, name, level)				\
OVAT_INIT(__##type)							\
{									\
	ovat_log_register(OVAT_STR(name),	\
						    OVAT_LOG_##level);	\
}

#endif /*  OVAT_LOG_H */
