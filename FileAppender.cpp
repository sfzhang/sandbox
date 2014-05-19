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

#include "FileAppender.h"
#include "Trace.h"

#include <errno.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/time.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

namespace hifu { namespace logger {

/** @brief Global variables */
static HIFU_TLS char    g_log_file[1024] = {0};     /* Log file to append */
static HIFU_TLS size_t  g_max_log_size = 0;         /* Log maximum size */
static HIFU_TLS FILE    *g_fp = NULL;               /* File pointer */

/**
 * @brief Rename log file
 *
 * @param N/A
 *
 * @return On success, return 0, otherwise return error code
 */
static int renameLogFile()
{
    struct timeval now = {0, 0};
    int ret = gettimeofday(&now, NULL);
    if (ret != 0) {
        ETRACE("gettimeofday() failed: errno[%d]", errno);
        return errno;
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
        return errno; 
    }

    char new_log_file[1024] = {0};
    ret = snprintf(new_log_file, sizeof(new_log_file),
                   "%s.%04d%02d%02d.%02d%02d%02d.%06ld",
                   g_log_file,
                   (t->tm_year + 1900),
                   (t->tm_mon + 1),
                   t->tm_mday,
                   t->tm_hour,
                   t->tm_min,
                   t->tm_sec,
                   now.tv_usec);
    if (ret < 0) {
        ETRACE("snprintf() failed: errno[%d]", errno);
        return errno;
    }

    ret = rename(g_log_file, new_log_file);
    if (ret != 0) {
        ETRACE("rename() failed: g_log_file[%s], new_log_file[%s], errno[%d]",
               g_log_file, new_log_file, errno);
        return errno;
    }

    return 0;
}

/**
 * @brief Append to file
 *
 * @param[in] level Log severity level
 * @param[in] flag Flag for end of log message, zero(0) is NOT terminated
 * @param[in] msg Log message
 *
 * @return On success, return 0, otherwise return error code
 */
static int appendToFile(LogLevel level, int flag, const char *msg)
{
    if ((level < DEBUG) || (level > FATAL) || (!msg)) {
        ETRACE("Argument(s) illegal: level[%d], msg[%p]", level, msg);
        return EINVAL;
    }

    /* Check first */
    if ((0 == strlen(g_log_file)) || (0 == g_max_log_size)) {
        ETRACE("g_log_file[%s] or g_max_log_size[%zu] illegal",
               g_log_file, g_max_log_size);

        return EAGAIN;
    }

    /* Open file to write */
    if (!g_fp) {
        g_fp = fopen(g_log_file, "a+");
        if (!g_fp) {
            ETRACE("fopen() failed: g_log_file[%s], errno[%d]",
                   g_log_file, errno);
            return errno;
        }
    }

    /* Append to file */
    int ret = 0;
    if (flag) {
        ret = fprintf(g_fp, "%s\n", msg);
    }
    else {
        ret = fprintf(g_fp, "%s", msg);
    }

    if (ret < 0) {
        ETRACE("fprintf() failed: g_log_file[%s], errno[%d]", g_log_file, errno);
        ret = errno;

        /* Close file */
        if (fclose(g_fp) != 0) {
            ETRACE("fclose() failed: g_fp[%p], g_log_file[%s], errno[%d]",
                   g_fp, g_log_file, errno);
        }
        g_fp = NULL;

        return ret;
    }

    /* Is it terminated */
    if (flag) {
        if (fclose(g_fp) != 0) {
            ETRACE("fclose() failed: g_fp[%p], g_log_file[%s], errno[%d]",
                   g_fp, g_log_file, errno);
            g_fp = NULL;

            return errno;
        }

        g_fp = NULL;

        /* Check the log file size */
        struct stat st;
        ret = stat(g_log_file, &st);
        if (ret != 0) {
            ETRACE("stat() failed: g_log_file[%s], errno[%d]", g_log_file, errno);
            return errno;
        }

        if (st.st_size >= (long)g_max_log_size) {
            ret = renameLogFile();
            if (ret != 0) {
                ETRACE("renameLogFile() failed");
                return ret;
            }
        }
    }

    return 0;
}

int initFileAppender(void *argv, AppendFn *append_func)
{
    if ((!argv) || (!append_func)) {
        ETRACE("Argument(s) illegal: argv[%p], append_func[%p]", argv, append_func);
        return EINVAL;
    }

    struct FileAppender *file_appender = (struct FileAppender *)argv;
    if (0 == strlen(file_appender->log_file)) {
        ETRACE("log_file[%s] is empty", file_appender->log_file);
        return EINVAL;
    }
    snprintf(g_log_file, sizeof(g_log_file), "%s", file_appender->log_file);

    if ((0 == file_appender->max_log_size) || (file_appender->max_log_size > MAX_LOG_SIZE)) {
        ETRACE("max_log_size[%zu] illegal", file_appender->max_log_size);
        return EINVAL;
    }
    g_max_log_size = file_appender->max_log_size;

    *append_func = appendToFile;

    return 0;
}

} } /* !namespace */
