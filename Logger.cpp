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
 * @file Logging core
 *
 * @author zhangshengfa
 * @date 2014-01-01
 */

#include "Logger.h"
#include "Trace.h"

#include <cstdlib>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <cstdio>

#ifdef HIFU_THREAD
#include <pthread.h>
#endif

#include "Format.inc"

namespace hifu { namespace logger {

/** @brief Global variables */
static HIFU_TLS AppendFn    g_append_func = NULL;           /* Append function */
static HIFU_TLS LogLevel    g_log_level = DEBUG;            /* Log level */
static HIFU_TLS char        g_log_buffer[1024 * 6] = {0};   /* Log buffer */

int initLogger(LogLevel level, InitAppenderFn init_func, void *argv)
{
    /* Save log level */
    if ((level < DEBUG) || (level > FATAL)) {
        ETRACE("Argument level[%d] is illegal", level);
        return -1;
    }
    g_log_level = level;

    /* Initialize appender */
    if (!init_func) {
        ETRACE("Argument init_func[%p] is NULL", init_func);
        return -1;
    }

    int ret = (*init_func)(argv, &g_append_func);
    if (ret != 0) {
        ETRACE("Call *init_func() failed: ret[%d]", ret);
        return -1;
    }

    if (!g_append_func) {
        ETRACE("g_append_func[%p] is NULL after call *init_func()",
               g_append_func);
        return -1;
    }

    return 0;
}

void log(LogLevel level, const char *file, const char *func, int line,
         const char *fmt, ...)
{
    /* Check append function */
    if (!g_append_func) {
        ETRACE("g_append_func[%p] is NULL", g_append_func);
        return;
    }

    /* Check arguments */
    if ((!file) || (!func) || (!fmt)) {
        return;
    }

    if ((level < g_log_level) || (level > FATAL)) {
        return;
    }

    /* Generate log message */
    char log_msg[1024 * 5] = {0};
    int ret = 0;

    va_list ap;
    va_start(ap, fmt);
    ret = vsnprintf(log_msg, sizeof(log_msg), fmt, ap);
    va_end(ap);
    
    if (ret < 0) {
        ETRACE("vsnprintf() failed: errno[%d]", errno);
        return;
    }

    g_log_buffer[0] = '\0';
    ret = format(g_log_buffer, sizeof(g_log_buffer), level, file, func, line, log_msg);
    if (ret < 0) {
        ETRACE("format() failed: ret[%d]", ret);
        return;
    }

    /* Append */
    ret = (*g_append_func)(level, 1, g_log_buffer);
    if (ret != 0) {
        ETRACE("*g_append_func() failed: ret[%d]", ret);
    }
}

void dump(LogLevel level, const char *file, const char *func, int line,
          const char *buffer, size_t size)
{
    /* Check append function */
    if (!g_append_func) {
        ETRACE("g_append_func[%p] is NULL", g_append_func);
        return;
    }

    /* Check arguments */
    if ((!file) || (!func) || (!buffer)) {
        ETRACE("Argument(s) illegal: file[%p], func[%p], buffer[%p]",
               file, func, buffer);
        return;
    }

    if ((level < g_log_level) || (level > FATAL)) {
        return;
    }

    /* Format first */
    g_log_buffer[0] = '\0';
    int ret = format(g_log_buffer, sizeof(g_log_buffer), level, file, func, line,
                     "dump buffer as followed:\n");
    if (ret < 0) {
        ETRACE("format() failed: ret[%d]", ret);
        return;
    }

    /* Number of character one line in hex */
    int char_count = 78;

    /* Maximum size to dump is UINT_MAX */
    if (size > UINT_MAX) {
        size = UINT_MAX;
    }

    /* begin to dump */
    size_t i = 0;
    size_t j = 0;
    while (i < size) {
        /* Check enough buffer */
        if ((sizeof(g_log_buffer) - strlen(g_log_buffer)) <= char_count) {
            ret = (*g_append_func)(level, 0, g_log_buffer);
            if (ret != 0) {
                ETRACE("*g_append_func() failed: ret[%d]", ret);
                return;
            }
            g_log_buffer[0] = '\0';
        }

        j = i;

        /* Count */
        ret = snprintf((g_log_buffer + strlen(g_log_buffer)),
                       (sizeof(g_log_buffer) - strlen(g_log_buffer)),
                       "%08x |",
                       static_cast<unsigned int>(i / 16));
        if (ret != 10) {
            ETRACE("snprintf() failed: errno[%d]", errno);
            return;
        }

        /* Hex data */
        while ((i < size) && ((i - j) < 16)) {
            ret = snprintf((g_log_buffer + strlen(g_log_buffer)),
                           (sizeof(g_log_buffer) - strlen(g_log_buffer)),
                           " %02x",
                           static_cast<unsigned char>(buffer[i]));
            if (ret != 3) {
                ETRACE("snprintf() failed: ret[%d], errno[%d]", ret, errno);
                return;
            }

            i++;
        }

        while ((i - j) < 16) {
            ret = snprintf((g_log_buffer + strlen(g_log_buffer)),
                           (sizeof(g_log_buffer) - strlen(g_log_buffer)),
                           "   ");
            if (ret != 3) {
                ETRACE("snprintf() failed: errno[%d]", errno);
                return;
            }

            i++;
        }

        /* Separator */
        ret = snprintf((g_log_buffer + strlen(g_log_buffer)),
                       (sizeof(g_log_buffer) - strlen(g_log_buffer)),
                       " | ");
        if (ret != 3) {
            ETRACE("snprintf() failed: errno[%d]", errno);
            return;
        }

        /* Value */
        i = j;
        unsigned char c = '\0';
        while ((i < size) && ((i - j) < 16)) {
            if (static_cast<unsigned int>(buffer[i]) < 33) {
                c = '.';
            }   
            else if (static_cast<unsigned int>(buffer[i]) > 126) {
                c = '*';
            }   
            else {
                c = buffer[i];
            }

            ret = snprintf((g_log_buffer + strlen(g_log_buffer)),
                           (sizeof(g_log_buffer) - strlen(g_log_buffer)),
                           "%c",
                           c);
            if (ret != 1) {
                ETRACE("snprintf() failed: errno[%d]", errno);
                return;
            }

            i++;
        }

        /* End of line */
        if (i < size) {
            ret = snprintf((g_log_buffer + strlen(g_log_buffer)),
                           (sizeof(g_log_buffer) - strlen(g_log_buffer)),
                           "\n");
            if (ret != 1) {
                ETRACE("snprintf() failed: errno[%d]", errno);
                return;
            }
        }
    }

    /* Dump left log message */
    ret = (*g_append_func)(level, 1, g_log_buffer);
    if (ret != 0) {
        ETRACE("*g_append_func() failed: ret[%d]", ret);
        return;
    }
}

} } /* !namespace */
