// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "client.h"

#include <QtCore/qmetaobject.h>

Client::Client(QObject *parent)
    :   QObject(parent)
{
}

Client::~Client()
{
    stopClient();
}

//! [startClient]
void Client::startClient(const QBluetoothServiceInfo &remoteService)
{
    if (socket)
        return;

    // Connect to service
    socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);
    qDebug() << "Create socket";
    socket->connectToService(remoteService);
    qDebug() << "ConnectToService done";

    connect(socket, &QBluetoothSocket::readyRead, this, &Client::readSocket);
    connect(socket, &QBluetoothSocket::connected, this, QOverload<>::of(&Client::connected));
    connect(socket, &QBluetoothSocket::disconnected, this, &Client::disconnected);
    connect(socket, &QBluetoothSocket::errorOccurred, this, &Client::onSocketErrorOccurred);
}
//! [startClient]

//! [stopClient]
void Client::stopClient()
{
    delete socket;
    socket = nullptr;
}
//! [stopClient]

//! [readSocket]
void Client::readSocket()
{
    if (!socket)
        return;

    while (socket->canReadLine()) {
        QByteArray line = socket->readLine().trimmed();
        emit messageReceived(socket->peerName(),
                             QString::fromUtf8(line.constData(), line.length()));
    }
}
//! [readSocket]

//! [sendMessage]
void Client::sendMessage(const QString &message)
{
    QByteArray text = message.toUtf8() + '\n';
    socket->write(text);
}
//! [sendMessage]

void Client::onSocketErrorOccurred(QBluetoothSocket::SocketError error)
{
    if (error == QBluetoothSocket::SocketError::NoSocketError)
        return;

    QMetaEnum metaEnum = QMetaEnum::fromType<QBluetoothSocket::SocketError>();
    QString errorString = socket->peerName() + QLatin1Char(' ')
            + metaEnum.valueToKey(static_cast<int>(error)) + QLatin1String(" occurred");

    emit socketErrorOccurred(errorString);
}

//! [connected]
void Client::connected()
{
    emit connected(socket->peerName());
}
//! [connected]
