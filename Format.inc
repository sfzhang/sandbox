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
 * @file Formating log message
 *
 * @author zhangshengfa
 * @date 2014-01-01
 */

#include "LoggerDefs.h"

#include <cstdlib>
#include <sys/time.h>
#include <time.h>
#include <errno.h>
#include <stdarg.h>

#ifdef HIFU_THREAD
#include <pthread.h>
#endif

namespace hifu { namespace logger {

/** @brief Global variabel */
static HIFU_TLS int         g_init_flag = 0;    /* Whether initialized */
static HIFU_TLS pid_t       g_pid;              /* Process ID */

#ifdef HIFU_THREAD
static HIFU_TLS pthread_t   g_tid;              /* Thread ID */
#endif

static char g_log_level_flag[] = {'D', 'I', 'W', 'E', 'F' };

/**
 * @brief Formating log message
 *
 * @param[out] log_buffer Log buffer saved log message after formated
 * @param[in] buffer_size Size of log_buffer
 * @param[in] level Log level
 * @param[in] file File name
 * @param[in] func Function name
 * @param[in] line Line number
 * @param[in] log_msg Log message
 *
 * @return On success, return 0; if log_msg is truncated, return the expected
 *         size, otherwise return -1
 */
static int format(char *log_buffer, size_t buffer_size, LogLevel level,
                  const char *file, const char *func, int line,
                  const char *log_msg)
{
    if (!g_init_flag) {
        g_pid = getpid();

#ifdef HIFU_THREAD
        g_tid = pthread_self();
#endif
    }

    /* Get current time */
    struct timeval now;
    int ret = gettimeofday(&now, NULL);
    if (ret != 0) {
        ETRACE("gettimeofday() failed: errno[%d]", errno);
        return -1;
    }

    struct tm *t = NULL;
#ifndef HIFU_THREAD
    t = localtime(&now.tv_sec);
#else
    struct tm t2;
    t = localtime_r(&now.tv_sec, &t2);
#endif

    if (!t) {
#ifndef HIFU_THREAD
        ETRACE("localtime() failed: sec[%ld], errno[%d]", now.tv_sec, errno);
#else
        ETRACE("localtime_r() failed: sec[%ld], errno[%d]", now.tv_sec, errno);
#endif
        return -1;
    }

    /* Format */
    ret = snprintf(log_buffer, buffer_size,
#ifndef HIFU_THREAD
                   "[%c][%04d-%02d-%02d %02d:%02d:%02d.%06ld][%d][%s:%s:%d] %s",
#else
                   "[%c][%04d-%02d-%02d %02d:%02d:%02d.%06ld][%d:%#lx][%s:%s:%d] %s",
#endif
                   g_log_level_flag[level],
                   (t->tm_year + 1900),
                   (t->tm_mon + 1),
                   t->tm_mday,
                   t->tm_hour,
                   t->tm_min,
                   t->tm_sec,
                   now.tv_usec,
                   g_pid,
#ifdef HIFU_THREAD
                   g_tid,
#endif
                   file,
                   func,
                   line,
                   log_msg);
    if (ret < 0) {
        ETRACE("snprintf() failed: errno[%d]", errno);
    }

    return ret;
}

} } /* !namespace */
