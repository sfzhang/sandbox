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
 * @file The definition of thread manager
 *
 * @author zhangshengfa
 * @date 2014-04-26
 */

#ifndef _THREADMANAGER_H_
#define _THREADMANAGER_H_

#include <QString>
#include <QThread>

#include <logger/Logger.h>

/**
 * @class ThreadManager 
 *
 * @brief This class inherited from QThread manage the thread
 */
class ThreadManager: public QThread {

    Q_OBJECT

public:

    /**
     * @brief Default constructor
     *
     * @param[in] parent The parent with NULL default
     *
     * @return N/A
     */
    ThreadManager(QObject *parent = NULL);

    /**
     * @brief destructor
     *
     * @param N/A
     *
     * @return N/A
     */
    ~ThreadManager();

    /**
     * @brief Initialize
     *
     * @param[in] name To specify a name for the thread
     *
     * @retval true Initialize successfully
     * @retval false Failed to initialize
     */
    bool initialize(const QString &name);

protected:

    /**
     * @brief Thread main loop
     *
     * @param N/A
     *
     * @return N/A
     */
    virtual void run();

public:

    QString m_thread_name;              /**< The name of thread */

};

#endif /* !_TCPSOCKETTHREAD_H_ */
