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

/* SDK log type */
#define OVAT_LOGTYPE_CORE       0 /**< Log related to ovat-core. */
#define OVAT_LOGTYPE_APPCTL     1 /**< Log related to ovat-appctl. */
#define OVAT_LOGTYPE_NM         2 /**< Log related to ovat-nm. */
#define OVAT_LOGTYPE_CANNM      3 /**< Log related to ovat-cannm. */


/** First identifier for extended logs */
#define OVAT_LOGTYPE_FIRST_EXT_ID 32

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
int ovat_log_get_level(uint32_t logtype);

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
bool ovat_log_can_log(uint32_t logtype, uint32_t loglevel);

/**
 * Set the log level for a given type based on globbing pattern.
 *
 * @param pattern
 *   The globbing pattern identifying the log type.
 * @param level
 *   The level to be set.
 * @return
 *   0 on success, a negative value if level is invalid.
 */
int ovat_log_set_level_pattern(const char *pattern, uint32_t level);

/**
 * Set the log level for a given type based on regular expression.
 *
 * @param regex
 *   The regular expression identifying the log type.
 * @param level
 *   The level to be set.
 * @return
 *   0 on success, a negative value if level is invalid.
 */
int ovat_log_set_level_regexp(const char *regex, uint32_t level);

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
int ovat_log_set_level(uint32_t logtype, uint32_t level);

/**
 * Get the current loglevel for the message being processed.
 *
 * Before calling the user-defined stream for logging, the log
 * subsystem sets a per-lcore variable containing the loglevel and the
 * logtype of the message being processed. This information can be
 * accessed by the user-defined log output function through this
 * function.
 *
 * @return
 *   The loglevel of the message being processed.
 */
int ovat_log_cur_msg_loglevel(void);

/**
 * Get the current logtype for the message being processed.
 *
 * Before calling the user-defined stream for logging, the log
 * subsystem sets a per-lcore variable containing the loglevel and the
 * logtype of the message being processed. This information can be
 * accessed by the user-defined log output function through this
 * function.
 *
 * @return
 *   The logtype of the message being processed.
 */
int ovat_log_cur_msg_logtype(void);

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
int ovat_log_register(const char *name);

/**
 * @warning
 * @b EXPERIMENTAL: this API may change without prior notice
 *
 * Register a dynamic log type and try to pick its level from EAL options
 *
 * ovat_log_register() is called inside. If successful, the function tries
 * to search for matching regexp in the list of EAL log level options and
 * pick the level from the last matching entry. If nothing can be applied
 * from the list, the level will be set to the user-defined default value.
 *
 * @param name
 *    Name for the log type to be registered
 * @param level_def
 *    Fallback level to be set if the global list has no matching options
 * @return
 *    - >=0: the newly registered log type
 *    - <0: ovat_log_register() error value
 */
int ovat_log_register_type_and_pick_level(const char *name, uint32_t level_def);

/**
 * Dump log information.
 *
 * Dump the global level and the registered log types.
 *
 * @param f
 *   The output stream where the dump should be sent.
 */
void ovat_log_dump(FILE *f);

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
int ovat_log(uint32_t level, uint32_t logtype, const char *format, ...);
//	__ovat_format_printf(3, 4);

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
int ovat_vlog(uint32_t level, uint32_t logtype, const char *format, va_list ap);
//	__ovat_format_printf(3, 0);

int ovat_log_init(const char *id, int facility);

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
		 OVAT_LOGTYPE_ ## t, # t ": " __VA_ARGS__)

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
int type;								\
OVAT_INIT(__##type)							\
{									\
	type = ovat_log_register_type_and_pick_level(OVAT_STR(name),	\
						    OVAT_LOG_##level);	\
}

#endif /*  OVAT_LOG_H */
