// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "client.h"

#include "../opengl/gl.h"

#include <QtCore/qmetaobject.h>
#include <qbluetoothaddress.h>
#include <qbluetoothservicediscoveryagent.h>
#include <qbluetoothserviceinfo.h>
#include <qbluetoothlocaldevice.h>
#include <qbluetoothuuid.h>
#include <QtBluetooth/qbluetoothserviceinfo.h>
#include <QtBluetooth/qbluetoothsocket.h>

static const QString serviceName("Broadcast Service");
static const QLatin1String serviceUuid("f296bf37-5412-460d-954d-2fcc31b072c0");

Client::Client(const QString &name, const QBluetoothAddress &address, QObject *parent)
    :   QObject(parent)
{
    QBluetoothLocalDevice localDevice;
    QBluetoothAddress adapterAddress = localDevice.address();

    startClient(address);

//    discoveryAgent = new QBluetoothServiceDiscoveryAgent(adapterAddress);

//    discoveryAgent->setRemoteAddress(address);

//    connect(discoveryAgent, &QBluetoothServiceDiscoveryAgent::serviceDiscovered,
//            this, &Client::serviceDiscovered);

//    discoveryAgent->start();
}

Client::~Client()
{
    stopClient();
    delete discoveryAgent;
    delete socket;
}

void Client::serviceDiscovered(const QBluetoothServiceInfo &info)
{
#if 1
    qDebug() << "Discovered service on"
             << info.device().name() << info.device().address().toString();
    qDebug() << "\tService name:" << info.serviceName();
    qDebug() << "\tDescription:"
             << info.attribute(QBluetoothServiceInfo::ServiceDescription).toString();
    qDebug() << "\tProvider:"
             << info.attribute(QBluetoothServiceInfo::ServiceProvider).toString();
    qDebug() << "\tL2CAP protocol service multiplexer:"
             << info.protocolServiceMultiplexer();
    qDebug() << "\tRFCOMM server channel:" << info.serverChannel();
#endif
    if (info.serviceName().isEmpty())
        return;

    if(info.serviceName() == serviceName){
        startClient(info);
    }
}

void Client::startClient(const QBluetoothAddress &address)
{
    if (socket)
        return;

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

void Client::startClient(const QBluetoothServiceInfo &remoteService)
{
    if (socket)
        return;

    socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);
    qDebug() << "Create socket";
    socket->connectToService(remoteService);
    qDebug() << "ConnectToService done";

    connect(socket, &QBluetoothSocket::readyRead, this, &Client::readSocket);
    connect(socket, &QBluetoothSocket::connected, this, QOverload<>::of(&Client::connected));
    connect(socket, &QBluetoothSocket::disconnected, this, &Client::disconnected);
    connect(socket, &QBluetoothSocket::errorOccurred, this, &Client::onSocketErrorOccurred);
}

void Client::stopClient()
{
    delete socket;
    socket = nullptr;
}

void Client::readSocket()
{
    if (!socket)
        return;

    while (socket->canReadLine()) {
        QByteArray line = socket->readLine();
        if(line.size() >= 13){
            line = line.last(13);

            const float* ptrFloat = reinterpret_cast<const float*>(line.constData());

            GL::gyro.setX(*ptrFloat);
            GL::gyro.setY(-(*(ptrFloat+1)));
            GL::gyro.setZ(*(ptrFloat+2));
            if(abs(GL::gyro.x()) > 1.0f){
                GL::gyro.setX(0.0f);
            }
            if(abs(GL::gyro.y()) > 1.0f){
                GL::gyro.setY(0.0f);
            }
            if(abs(GL::gyro.z()) > 1.0f){
                GL::gyro.setZ(0.0f);
            }
//            qDebug() << *ptrFloat << " " << *(ptrFloat+1) << " " << *(ptrFloat+2);
        }

//        qDebug() << "received " << QString::fromUtf8(line.constData(), line.length());
        emit messageReceived(socket->peerName(),
                             QString::fromUtf8(line.constData(), line.length()));
    }
}

void Client::sendMessage(const QString &message)
{
    QByteArray text = message.toUtf8() + '\n';
    socket->write(text);
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
    emit connected(socket->peerName());
}
