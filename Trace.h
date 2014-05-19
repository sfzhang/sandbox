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
 * @file This file define some macro for dump message to stdout or stderr
 *
 * @author zhangshengfa
 * @date 2013-12-22
 */

#ifndef _TRACE_H_
#define _TRACE_H_

#include "LoggerDefs.h"

#include <cstdio>
#include <cstring>
#include <errno.h>

namespace hifu { namespace logger {

/** @brief Define errmsg, NOT thread-safe */
#ifndef errmsg
#   define errmsg strerror(errno)
#endif

/** @brief Dump debug to stream */
#define _TRACE(stream, level, format, ...) \
    fprintf(stream, "["level"][%s:%s:%d] "format"\n", __FILE__, __FN__, __LINE__, \
            ##__VA_ARGS__)

/** @brief dump debug message */
#define DTRACE(format, ...) \
    _TRACE(stdout, "D", format, ##__VA_ARGS__)

/** @brief dump info message */
#define ITRACE(format, ...) \
    _TRACE(stdout, "I", format, ##__VA_ARGS__)

/** @brief dump warning message */
#define WTRACE(format, ...) \
    _TRACE(stdout, "W", format, ##__VA_ARGS__)

/** @brief dump error message*/
#define ETRACE(format, ...) \
    _TRACE(stderr, "E", format, ##__VA_ARGS__)

/** @brief dump error message*/
#define FTRACE(format, ...) \
    _TRACE(stderr, "F", format, ##__VA_ARGS__)

} } /* !namespace */

#endif /* !_TRACE_H_ */
