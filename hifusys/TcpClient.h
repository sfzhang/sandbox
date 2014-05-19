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
 * @file The definition of TCP socket worker
 *
 * @author zhangshengfa
 * @date 2014-04-26
 */

#ifndef _TCPWORKER_H_
#define _TCPWORKER_H_

#include "HifuSysDefs.h"

#include <QTcpSocket>
#include <QHostAddress>
#include <QString>
#include <QThread>

/**
 * @class TcpClient
 *
 * @brief This class provide a worker to handle the TCP client socket
 */
class TcpClient: public QObject {

    Q_OBJECT

public:

    /**
     * @brief Default constructor
     *
     * @param[in] parent The parent object
     *
     * @return N/A
     */
    TcpClient(QObject *parent = NULL);

    /**
     * @brief destructor
     *
     * @param N/A
     *
     * @return N/A
     */
    ~TcpClient();

    /**
     * @brief Initialize
     *
     * @param[in] ip The server IP address
     * @param[in] port The server listen port
     *
     * @retval true Initialize successfully
     * @retval false Failed to initialize
     */
    bool initialize(const QString &ip, quint16 port);

    /**
     * @brief Static function, Check IP is illegal
     *
     * @param[in] ip The IP address
     *
     * @retval true The ip is illegal
     * @retval false The ip is NOT illegal
     */
    static bool isLegalAddress(const QString &ip)
    {
        QHostAddress host_addr;
        if (!host_addr.setAddress(ip)) {
            return false;
        }
        return true;
    }

protected:

    /**
     * @brief Read the expected size from socket
     *
     * @param[out] buffer The buffer recevied data
     * @param[in] size The size of buffer
     *
     * @retval ture Read successfully
     * @retval false Read failed
     */
    bool readn(char *buffer, size_t size);

signals:

    /**
     * @brief Success to connect to server
     *
     * @param N/A
     *
     * @return N/A
     */
    void connected();

    /**
     * @brief Disconnect from server
     *
     * @param N/A
     *
     * @return N/A
     */
    void disconnected();

    /**
     * @brief Socket error
     *
     * @param[in] state The socket state 
     * @param[in] err_msg The socket error message
     *
     * @return N/A
     */
    void error(QAbstractSocket::SocketState state, QString err_msg);

    /**
     * @brief Receive data
     *
     * @param[in] data The received data
     *
     * @return N/A
     */
    void received(QByteArray data);

    /**
     * @brief Send data
     *
     * @param N/A
     *
     * @return N/A
     */
    void sent(QByteArray data);

public slots:

    /**
     * @brief Connect to server
     *
     * @param N/A
     *
     * @return N/A
     */
    void connect();

    /**
     * @brief Overried, connect to server with the given address
     *
     * @param[in] ip Server ip address
     * @param[in] port Server port
     *
     * @return N/A
     */
    void connect(QString ip, quint16 port);

    /**
     * @brief Disconnect from server
     *
     * @param N/A
     *
     * @return N/A
     */
    void disconnect();

    /**
     * @brief Send data
     *
     * @param[in] data Data to be sent
     *
     * @return N/A
     */
    void send(QByteArray data);

    /**
     * @brief Close socket and exit
     *
     * @param N/A
     *
     * @return N/A
     */
    void close();

protected slots:

    /**
     * @brief Socket error occur
     *
     * @param[in] socket_error The socket error
     *
     * @return N/A
     */
    void socketError(QAbstractSocket::SocketError socket_error);

    /**
     * @brief Receive data
     *
     * @param N/A
     *
     * @return N/A
     */
    void receive();

public:

    QTcpSocket *m_socket;   /**< TCP socket */
    QString m_ip;           /**< Server IP address */
    quint16 m_port;         /**< Server port */

    qint64 m_next_size;     /**< The next received size */
    QByteArray m_data;      /**< The receive buffer */
};

/** @brief Register QAbstractSocket::SocketState for queued connections if need */
Q_DECLARE_METATYPE(QAbstractSocket::SocketState)

#endif /* !_TCPWORKER_H_ */
