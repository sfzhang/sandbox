/*
 *  Copyright (c) 2013, BLK Co., Ltd.  All right reserved.
 *
 *  This is unpublished proprietary information of BLK CO., Ltd and is
 *  protected as an unpublished work under applicable Copyright laws.
 *  The contents of this file may not be disclosed to third parties,
 *  copied or duplicated in any form, in whole or in part, without the
 *  prior written permission of BLK Co., Ltd.
 *
 *  $Id$
 *
 */

/**
 * @file Definition for logging
 *
 * @author zhangshengfa
 * @date 2013-12-11
 */

#ifndef _LOGGER_H_
#define _LOGGER_H_

#if (defined(_MSC_VER) && _MSC_VER > 1000)
#pragma once
#endif

#include "LoggerDefs.h"

#include <sys/types.h>

namespace hifu { namespace logger {

/** @brief Severity level of log */
enum LOGGER_LIB LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    FATAL
};

/**
 * @brief Type definition for log appending
 *
 * @param[in] level Log severity level
 * @param[in] flag Flag for end of log message, zero(0) is NOT terminated
 * @param[in] msg Log message
 *
 * @return On success, return 0, otherwise return error code
 */
typedef int (*AppendFn)(LogLevel level, int flag, const char *msg);

/**
 * @brief Type definition for the funtion that initialized appender
 *
 * @param[in] argv Argument value
 * @param[out] append_func Append function
 *
 * @retval 0 Initialize appender successfully
 * @retval -1 Failed to initialize
 */
typedef int (*InitAppenderFn)(void *argv, AppendFn *append_func);

/**
 * @brief Initialize logger
 *
 * @param[in] level Log severity level to append
 * @param[in] init_func Appender initialize function
 * @param[in] argv Argument for init_func
 *
 * @retval 0 Initialized successfully
 * @retval -1 Failed to initialize
 */
LOGGER_LIB int initLogger(LogLevel level, InitAppenderFn init_func, void *argv);

/**
 * @brief Logging
 *
 * @param[in] level Log severity level
 * @param[in] file File name
 * @param[in] func Function name
 * @param[in] line Line number
 * @param[in] fmt Log format string
 *
 * @return N/A
 */
LOGGER_LIB void log(LogLevel level, const char *file, const char *func,
                    int line, const char *fmt, ...);

/**
 * @brief Dump buffer in hex
 *
 * @param[in] level Log severity level
 * @param[in] file File name
 * @param[in] func Function name
 * @param[in] line Line number
 * @param[in] buffer Buffer to dump
 * @param[in] size Size of buffer
 *
 * @return N/A
 */
LOGGER_LIB void dump(LogLevel level, const char *file, const char *func,
                     int line, const char *buffer, size_t size);

} } /* !namespace */

/** @brief Macro for logging shorthand */
#define LOG(level, ...) \
    hifu::logger::log(level, __FILE__, __FN__, __LINE__, __VA_ARGS__)
#define DLOG(...) LOG(hifu::logger::DEBUG, __VA_ARGS__)
#define ILOG(...) LOG(hifu::logger::INFO, __VA_ARGS__)
#define WLOG(...) LOG(hifu::logger::WARNING, __VA_ARGS__)
#define ELOG(...) LOG(hifu::logger::ERROR, __VA_ARGS__)
#define FLOG(...) LOG(hifu::logger::FATAL, __VA_ARGS__)

#define DUMP(level, buffer, size) \
    hifu::logger::dump(level, __FILE__, __FN__, __LINE__, buffer, size)
#define DDUMP(buffer, size) DUMP(hifu::logger::DEBUG, buffer, size)
#define IDUMP(buffer, size) DUMP(hifu::logger::INFO, buffer, size)
#define WDUMP(buffer, size) DUMP(hifu::logger::WARNING, buffer, size)
#define EDUMP(buffer, size) DUMP(hifu::logger::ERROR, buffer, size)
#define FDUMP(buffer, size) DUMP(hifu::logger::FATAL, buffer, size)

#endif /* !_LOGGER_H_ */
