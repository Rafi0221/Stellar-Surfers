// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef DEVICE_H
#define DEVICE_H

#include <qbluetoothlocaldevice.h>
#include "client.h"

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

public slots:
    void addDevice(const QBluetoothDeviceInfo&);
    void powerClicked(bool clicked);
private slots:
    void startScan();
    void stopScan();
    void scanFinished();
    void itemActivated(QListWidgetItem *item);
    void hostModeStateChanged(QBluetoothLocalDevice::HostMode);

private:
    QBluetoothDeviceDiscoveryAgent *discoveryAgent;
    QBluetoothLocalDevice *localDevice;
    Ui_ConnectWindow *ui;
    Client *client = nullptr;
};

#endif
