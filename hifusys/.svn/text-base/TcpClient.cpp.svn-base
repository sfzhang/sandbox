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
 * @file The implementation of TCP client socket
 *
 * @author zhangshengfa
 * @date 2014-04-26
 */

#include "TcpClient.h"

#include <logger/Logger.h>
#include <logger/Trace.h>
#include <logger/FileAppender.h>

#include <arpa/inet.h>
#include <locale.h>
#include <errno.h>
#include <cstring>
#include <cstdlib>

using namespace hifu::logger;

TcpClient::TcpClient(QObject *parent):
    QObject(parent),
    m_ip("127.0.0.1"),
    m_port(0),
    m_next_size(-1)
{
    m_socket = new QTcpSocket(this);
    /* Register QAbstractSocket::SocketError for queued connections if need */
    // qRegisterMetaType(QAbstractSocket::SocketError);
}

TcpClient::~TcpClient()
{
    delete m_socket;
}

bool TcpClient::initialize(const QString &ip, quint16 port)
{
    /* Chech the IP address port */
    if (!isLegalAddress(ip)) {
        ELOG("ip[%s] is NOT legal address", TO_PCHAR(ip));
        return false;
    }

    m_ip = ip;
    m_port = port;

    /* Connect the signal */
    if (!QObject::connect(m_socket, SIGNAL(connected()), this, SIGNAL(connected()))) {
        ELOG("connect() failed: SIGNAL[connected()], SIGNAL[connected()]");
        return false;
    }

    if (!QObject::connect(m_socket, SIGNAL(disconnected()), this, SIGNAL(disconnected()))) {
        ELOG("connect() failed: SIGNAL[disconnected()], SIGNAL[disconnected()]");
        return false;
    }

    if (!QObject::connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)),
                          this, SLOT(socketError(QAbstractSocket::SocketError)))) { 
        ELOG("connect() failed: SIGNLA[error()], SLOT[socketError()]");
        return false;
    }

    if (!QObject::connect(m_socket, SIGNAL(readyRead()), this, SLOT(receive()))) {
        ELOG("connect() failed: SIGNAL[readyRead()], SLOT[receive()]");
        return false;
    }

   return true;
}

bool TcpClient::readn(char *buffer, size_t size)
{
    QString err_msg;
    int left = size;
    int ret = 0;

    while (left > 0) {
        ret = m_socket->read((buffer + size - left), left);
        if (ret < 0) {
            err_msg = QString("read() failed: ret[%1], err_msg[%2], size[%3], left[%4]")
                        .arg(ret)
                        .arg(m_socket->errorString())
                        .arg(size)
                        .arg(left);
            ELOG("%s", TO_PCHAR(err_msg));
            return false;
        }
        else {
            left -= ret;
        }
    }

    DLOG("read() success: size[%d]", size);
    DDUMP(buffer, size);
    return true;
}

void TcpClient::connect()
{
    QString err_msg;

    /* Check IP first */
    if (!isLegalAddress(m_ip)) {
        err_msg = QString("IP[%1] is NOT legal address").arg(m_ip);
        ELOG("%s", TO_PCHAR(err_msg));
        emit error(m_socket->state(), err_msg);
        return;
    }

    /* Check the socket is unconnected */
    QAbstractSocket::SocketState socket_state = m_socket->state();
    if (socket_state != QAbstractSocket::UnconnectedState) {
        err_msg = QString("The socket state[%1] is NOT in UnconnectedState")
                    .arg(static_cast<int>(socket_state));
        ELOG("%s", TO_PCHAR(err_msg));
        emit error(socket_state, err_msg);
        return;
    }

    /* Connect to server */
    DLOG("Begin to connect to ip[%s], port[%u]...", TO_PCHAR(m_ip), m_port);
    m_socket->connectToHost(m_ip, m_port);
}

void TcpClient::connect(QString ip, quint16 port)
{
    QString err_msg;

    /* Check IP first */
    if (!isLegalAddress(ip)) {
        err_msg = QString("IP[%1] is NOT legal address").arg(ip);
        ELOG("%s", TO_PCHAR(err_msg));
        emit error(m_socket->state(), err_msg);
        return;
    }

    /* Check the socket is unconnected */
    QAbstractSocket::SocketState socket_state = m_socket->state();
    if (socket_state != QAbstractSocket::UnconnectedState) {
        err_msg = QString("The socket state[%1] is NOT in UnconnectedState")
                    .arg(static_cast<int>(socket_state));
        ELOG("%s", TO_PCHAR(err_msg));
        emit error(socket_state, err_msg);
        return;
    }

    /* Connect to server */
    m_ip = ip;
    m_port = port;
    DLOG("Begin to connect to ip[%s], port[%u]...", TO_PCHAR(m_ip), m_port);
    m_socket->connectToHost(m_ip, m_port);
}

void TcpClient::disconnect()
{
    QString err_msg;

    /* Check current socket state */
    QAbstractSocket::SocketState socket_state = m_socket->state();
    if (QAbstractSocket::UnconnectedState == socket_state) {
        err_msg = QString("The socket is in [UnconnectedState]");
        ELOG("%s", TO_PCHAR(err_msg));
        emit error(socket_state, err_msg);
        return;
    }

    /* Disconnect now */
    DLOG("Begin to disconnect to ip[%s], port[%u]...", TO_PCHAR(m_ip), m_port);
    m_socket->disconnectFromHost();
}

void TcpClient::send(QByteArray data)
{
    QString err_msg;
    int size = data.size();
    const char *d = data.data();

    /* Check first */
    if (size <= 0) {
        err_msg = QString("data illegal: size[%1]").arg(size);
        ELOG("%s", TO_PCHAR(err_msg));

        emit error(m_socket->state(), err_msg);
        return;
    }

    /* Send data */
    int left = size;
    qint64 ret = 0;
    while (left > 0) {
        ret = m_socket->write((d + size - left), left);
        if (ret < 0) {
            err_msg = QString("write() failed: ret[%1], size[%2], left[%3]")
                        .arg(ret)
                        .arg(size)
                        .arg(left);
            ELOG("%s", TO_PCHAR(err_msg));
            EDUMP(d, size);

            emit error(m_socket->state(), err_msg);
            return;
        }

        left -= ret;
    }

    DLOG("write() data success: size[%d]", size);
    DDUMP(d, size);
    emit send(data);
}

void TcpClient::close()
{
    /* Check current socket state */
    QAbstractSocket::SocketState socket_state = m_socket->state();
    if (socket_state != QAbstractSocket::UnconnectedState) {
        DLOG("Begin to disconnect to ip[%s], port[%u]...", TO_PCHAR(m_ip), m_port);
        m_socket->disconnectFromHost();

        /* Wait until disconnected */
        if (!m_socket->waitForDisconnected(-1)) {
            ELOG("waitForDisconnected() failed: error[%d]", static_cast<int>(m_socket->error()));
        }
        else {
            WLOG("waitForDisconnected() success");
        }
    }

    /* Exit from thread */
    WLOG("Begin to exit from current thread...");
    QThread *thread = QThread::currentThread();
    thread->exit();
}

void TcpClient::socketError(QAbstractSocket::SocketError socket_error)
{
    QString err_msg = QString("Socket error: error[%1], err_msg[%2], ip[%3], port[%4]")
                        .arg(static_cast<int>(socket_error))
                        .arg(m_socket->errorString())
                        .arg(m_ip)
                        .arg(m_port);
    ELOG("%s", TO_PCHAR(err_msg));

    emit error(m_socket->state(), err_msg);
}

void TcpClient::receive()
{
    QString err_msg;
    qint64 ret = 0;
    qint64 size = 0;
    qint64 left = 0;
    int length = 0;
    char *d = NULL;

    while (1) {
        ret = m_socket->bytesAvailable();
        if (ret < 0) {
            err_msg = QString("bytesAvailable() failed: ret[%1], err_msg[%2]")
                        .arg(ret)
                        .arg(m_socket->errorString());
            ELOG("%s", TO_PCHAR(err_msg));

            emit error(m_socket->state(), err_msg);
            break;
        }
        else if (0 == ret) {
            WLOG("bytesAvailable() success, no data to read: ret[%ld]", ret);
            break;
        }
        DLOG("bytesAvailable() success: ret[%ld]", ret);

        /* Protocol: 4byte(length) + data(length) */
        left = ret;
        while (left > 0) {
            if (-1 == m_next_size) {
                if (left < static_cast<int>(sizeof(int))) {
                    WLOG("not enough data: left[%d], expected[%u]", ret, sizeof(int));
                    break;
                }
            
                /* Read size */
                if (!readn(reinterpret_cast<char *>(&length), sizeof(int))) {
                    err_msg = QString("read() failed: error[%1], err_msg[%2]")
                                .arg(static_cast<int>(m_socket->error()))
                                .arg(m_socket->errorString());
                    ELOG("%s", TO_PCHAR(err_msg));
                    emit error(m_socket->state(), err_msg);
                    return;
                }

                length = ntohl(length);
                if (length < 0) {
                    err_msg = QString("read() success, but length[%1] illegal")
                                .arg(length);
                    ELOG("%s", TO_PCHAR(err_msg));
                    emit error(m_socket->state(), err_msg);
                    return;
                }

                m_next_size = length;
                m_data.clear();
                m_data.resize(m_next_size);

                left -= sizeof(int);
            }
            else {
                /* Read length */
                size = m_data.size();
                d = m_data.data();

                /* The size to read */
                length = (left < m_next_size) ? left : m_next_size;

                /* Read */
                if (!readn((d + size - m_next_size), length)) {
                    err_msg = QString("read() failed: error[%1], err_msg[%2]")
                                .arg(static_cast<int>(m_socket->error()))
                                .arg(m_socket->errorString());
                    ELOG("%s", TO_PCHAR(err_msg));
                    emit error(m_socket->state(), err_msg);
                    return;
                }

                /* Check is a complete block */
                if (length == m_next_size) {
                    emit received(m_data);

                    m_next_size = -1;
                }

                left -= length;
            }
        }
    }
}
