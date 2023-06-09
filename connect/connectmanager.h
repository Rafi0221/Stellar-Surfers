// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef DEVICE_H
#define DEVICE_H

#include <qbluetoothlocaldevice.h>
#include "client.h"
#include "parser.h"
#include "glupdater.h"

#include <QDialog>

QT_FORWARD_DECLARE_CLASS(QBluetoothDeviceDiscoveryAgent)
QT_FORWARD_DECLARE_CLASS(QBluetoothDeviceInfo)
QT_FORWARD_DECLARE_CLASS(QListWidgetItem)

QT_FORWARD_DECLARE_CLASS(Ui_ConnectWindow)

QT_USE_NAMESPACE
    
    class ConnectManager : public QDialog
{
    Q_OBJECT

public:
    ConnectManager(QWidget *parent = nullptr);
    ~ConnectManager();

    bool isConnected();

public slots:
    void addDevice(const QBluetoothDeviceInfo&);
    void powerClicked(bool clicked);
    void connected();
    void disconnected();
    void send(const QByteArray &message);
private slots:
    void startScan();
    void stopScan();
    void scanFinished();
    void itemActivated(QListWidgetItem *item);
    void hostModeStateChanged(QBluetoothLocalDevice::HostMode);

signals:
    void deviceConnected();
    void connectionLost();

private:
    bool _isConnected = false;
    QBluetoothDeviceDiscoveryAgent *discoveryAgent;
    QBluetoothLocalDevice *localDevice;
    Ui_ConnectWindow *ui;
    Client *client = nullptr;
    Parser *parser;
    GLUpdater *glupdater;
};

#endif
