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
    explicit Client(const QString &name, const QBluetoothAddress &address, QObject *parent = nullptr);
    ~Client();

    void startClient(const QBluetoothServiceInfo &remoteService);
    void stopClient();

public slots:
    void serviceDiscovered(const QBluetoothServiceInfo&);
    void sendMessage(const QString &message);

signals:
    void messageReceived(const QString &sender, const QString &message);
    void connected(const QString &name);
    void disconnected();
    void socketErrorOccurred(const QString &errorString);

private slots:
    void readSocket();
    void connected();
    void onSocketErrorOccurred(QBluetoothSocket::SocketError);

private:
    QBluetoothServiceDiscoveryAgent *discoveryAgent;
    QBluetoothSocket *socket = nullptr;
};

#endif // CLIENT_H
