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
 * @file Definition for file appender
 *
 * @author zhangshengfa
 * @date 2014-01-02
 */

#ifndef _FILEAPPENDER_H_
#define _FILEAPPENDER_H_

#if (defined(_MSC_VER) && _MSC_VER > 1000)
#pragma once
#endif

#include "LoggerDefs.h"
#include "Logger.h"
#include "Trace.h"

#include <cstdio>

namespace hifu { namespace logger {

/** @brief Structure for file appender */
LOGGER_LIB struct FileAppender {
    char log_file[1024];
    size_t max_log_size;
};

/**
 * @brief Initialized file appender
 *
 * @param[in] argv Argument value
 * @param[out] append_func Append function
 *
 * @retval 0 Initialize appender successfully
 * @retval -1 Failed to initialize
 */
LOGGER_LIB int initFileAppender(void *argv, AppendFn *append_func);

/**
 * @brief Macro to initialize file appender
 *
 * @param[in] level Log level
 * @param[in] log_file Log file to append
 * @param[in] max_log_size The maximum size of log file
 * @param[out] ret Return value
 *
 * @return N/A
 */
#define INIT_FILE_APPENDER(level__, log_file__, max_log_size__, ret__) \
do { \
    struct FileAppender __file_appender; \
    snprintf(__file_appender.log_file, sizeof(__file_appender.log_file), \
             "%s", log_file__); \
    __file_appender.max_log_size = static_cast<size_t>(max_log_size__); \
    ret__ = initLogger(level__, initFileAppender, &__file_appender); \
    if (ret__ != 0) { \
        ETRACE("initLogger() failed: level[%d], log_file[%s], max_log_size[%zu]", \
               level__, log_file__, static_cast<size_t>(max_log_size__)); \
    } \
} while (0)

} } /* !namespace */

#endif /* !_FILEAPPENDER_H_ */
