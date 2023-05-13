// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef CLIENT_H
#define CLIENT_H

#include <QtCore/qobject.h>

#include <qbluetoothservicediscoveryagent.h>
#include <QtBluetooth/qbluetoothserviceinfo.h>
#include <QtBluetooth/qbluetoothsocket.h>

QT_FORWARD_DECLARE_CLASS(QBluetoothSocket)

QT_USE_NAMESPACE

class Client : public QObject
{
    Q_OBJECT

public:
    explicit Client(QObject *parent = nullptr);
    ~Client();

    void startClient(const QBluetoothAddress &address);
    void stopClient();

    constexpr static const QLatin1String serviceUuid = QLatin1String("f296bf37-5412-460d-954d-2fcc31b072c0");
    static const int messageSize = 16;

public slots:
    void sendMessage(const QByteArray &message);

signals:
    void messageReceived(const QByteArray &line);
    void deviceConnected(const QString &name);
    void connectionLost();
    void socketErrorOccurred(const QString &errorString);

private slots:
    void readSocket();
    void connected();
    void onSocketErrorOccurred(QBluetoothSocket::SocketError);
    void disconnected();

private:
    QBluetoothServiceDiscoveryAgent *discoveryAgent;
    QBluetoothSocket *socket = nullptr;
};

#endif // CLIENT_H
