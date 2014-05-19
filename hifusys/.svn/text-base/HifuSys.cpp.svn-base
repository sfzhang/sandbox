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
 * @file HifuSys main
 *
 * @author zhangshengfa
 * @date 2014-01-19
 */

#include <QApplication>
#include <QDesktopWidget>
#include <QMenuBar>
#include <QFile>
#include <locale.h>
#include <errno.h>
#include <cstring>
#include <cstdlib>

#include <logger/Logger.h>
#include <logger/FileAppender.h>
#include <logger/Trace.h>
#include <config/IEnvConfig.h>

#include "HifuSysDefs.h"
#include "MainWindow.h"

using namespace hifu::logger;

int main(int argc, char *argv[])
{
    /* Set locale first */
    char *lc = setlocale(LC_ALL, "");
    if (!lc) {
        ETRACE("setlocale() failed: errno[%d]", errno);
        return -1;
    }

    /* Load environment variable */
    if (!IEnvConfig::instance()->load()) {
        ETRACE("load() failed");
        return -1;
    }

    /* Initialize logger */
    char log_file[1024] = {0};
    snprintf(log_file, sizeof(log_file), "%s/log/%s",
             IEnvConfig::instance()->getRootPath().c_str(),
             HIFUSYS_LOG_FILE_NAME);

    int ret = 0;
    INIT_FILE_APPENDER(IEnvConfig::instance()->getLogLevel(),
                       log_file,
                       IEnvConfig::instance()->getLogSize(),
                       ret);
    if (ret != 0) {
        ETRACE("initLogger() failed: ret[%d]", ret);
        return -1; 
    }
    DLOG("Thread[hifusys][%lp] start...", QThread::currentThread());

    QApplication app(argc, argv);
    QFile qss(":/resource/stylesheet/stylesheet.qss");
    qss.open(QFile::ReadOnly);
    app.setStyleSheet(qss.readAll());
    qss.close();
    // app.setStyleSheet("* { background: #3B3B3B; color: #FAFAFA; font-size: 14px}");

    MainWindow hifusys;
    if (!hifusys.initialize()) {
        ELOG("initialize() failed");
        return -1;
    }

    // hifusys.showFullScreen();
    hifusys.showMaximized();
    DLOG("MainWindow w[%d], h[%d]", hifusys.width(), hifusys.height());

    QDesktopWidget *desk = QApplication::desktop();
    DLOG("desktop: w[%d], h[%d]", desk->screenGeometry().width(), desk->screenGeometry().height());
    DLOG("menubar: w[%d], h[%d]", hifusys.menuBar()->width(), hifusys.menuBar()->height());

    DLOG("centralwidth: w[%d], h[%d]", hifusys.centralWidget()->geometry().width(), hifusys.centralWidget()->geometry().height());

     return app.exec();
}
