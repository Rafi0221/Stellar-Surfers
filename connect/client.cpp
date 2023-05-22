// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "client.h"

#include <QtCore/qmetaobject.h>
#include <qbluetoothaddress.h>
#include <qbluetoothservicediscoveryagent.h>
#include <qbluetoothserviceinfo.h>
#include <qbluetoothlocaldevice.h>
#include <qbluetoothuuid.h>
#include <QtBluetooth/qbluetoothserviceinfo.h>
#include <QtBluetooth/qbluetoothsocket.h>


Client::Client(QObject *parent)
    :   QObject(parent)
{
}

Client::~Client()
{
    stopClient();
    delete discoveryAgent;
}

void Client::startClient(const QBluetoothAddress &address)
{
    if (socket)
        stopClient();

    QBluetoothUuid uuid = QBluetoothUuid(serviceUuid);
    socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);
    qDebug() << "Create socket";
    socket->connectToService(address, uuid);
    qDebug() << "ConnectToService done";

    connect(socket, &QBluetoothSocket::readyRead, this, &Client::readSocket);
    connect(socket, &QBluetoothSocket::connected, this, QOverload<>::of(&Client::connected));
    connect(socket, &QBluetoothSocket::disconnected, this, &Client::disconnected);
    connect(socket, &QBluetoothSocket::errorOccurred, this, &Client::onSocketErrorOccurred);
}

void Client::stopClient()
{
    if(socket != nullptr) {
        delete socket;
        socket = nullptr;
    }
}

void Client::readSocket()
{
    if (!socket)
        return;

    while (socket->bytesAvailable() >= messageSize) {
        QByteArray line = socket->read(messageSize);

        emit messageReceived(line);
    }
}

void Client::sendMessage(const QByteArray &message)
{
    //qDebug() << "sending" << message;
    if(socket->state() == QBluetoothSocket::SocketState::ConnectedState) {
        socket->write(message);
    }

}

void Client::onSocketErrorOccurred(QBluetoothSocket::SocketError error)
{
    if (error == QBluetoothSocket::SocketError::NoSocketError)
        return;

    QMetaEnum metaEnum = QMetaEnum::fromType<QBluetoothSocket::SocketError>();
    QString errorString = socket->peerName() + QLatin1Char(' ')
            + metaEnum.valueToKey(static_cast<int>(error)) + QLatin1String(" occurred");

    qDebug() << "socket error " << errorString;
    emit socketErrorOccurred(errorString);
}

void Client::connected()
{
    qDebug() << socket->peerName() << " connected";
    emit deviceConnected(socket->peerName());
}

void Client::disconnected()
{
    //stopClient();
    qDebug() << socket->peerName() << " disconnected";
    emit connectionLost();
}
