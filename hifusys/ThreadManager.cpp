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
 * @file The implementation of thread manager
 *
 * @author zhangshengfa
 * @date 2014-04-26
 */

#include "ThreadManager.h"

#include <logger/Logger.h>
#include <logger/Trace.h>
#include <logger/FileAppender.h>
#include <config/IEnvConfig.h>

#include <arpa/inet.h>
#include <locale.h>
#include <errno.h>
#include <cstring>
#include <cstdlib>

using namespace hifu::logger;

ThreadManager::ThreadManager(QObject *parent):
    QThread(parent)
{
}

ThreadManager::~ThreadManager()
{
    /* do nothing */
}

bool ThreadManager::initialize(const QString &name)
{
    if (0 == name.length()) {
        ELOG("name[%s] is empty", TO_PCHAR(name));
        return false;
    }

    m_thread_name = name;
    return true;
}

void ThreadManager::run()
{
    QString log_file = QString(IEnvConfig::instance()->getRootPath().c_str()) +
                       "/log/thread_" + m_thread_name + ".log";

    /* Initialize logger */
    int ret = 0;
    INIT_FILE_APPENDER(IEnvConfig::instance()->getLogLevel(),
                       TO_PCHAR(log_file),
                       IEnvConfig::instance()->getLogSize(),
                       ret);
    if (ret != 0) {
        ETRACE("INIT_FILE_APPENDER(() failed: ret[%d]", ret);
        exit(1);
    }
    ILOG("thread[%s][%lp] started,", TO_PCHAR(m_thread_name), QThread::currentThread());

    exec();
}
