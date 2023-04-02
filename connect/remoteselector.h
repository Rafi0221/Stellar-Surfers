// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef REMOTESELECTOR_H
#define REMOTESELECTOR_H

#include <QtWidgets/qdialog.h>
#include "ui_connectwindow.h"

#include <QtBluetooth/qbluetoothaddress.h>
#include <QtBluetooth/qbluetoothserviceinfo.h>
#include <QtBluetooth/qbluetoothuuid.h>

QT_FORWARD_DECLARE_CLASS(QBluetoothServiceDiscoveryAgent)
QT_FORWARD_DECLARE_CLASS(QListWidgetItem)

QT_USE_NAMESPACE


class RemoteSelector : public QObject
{
    Q_OBJECT

public:
    explicit RemoteSelector(const QBluetoothAddress &localAdapter, Ui_ConnectWindow* _ui);
    ~RemoteSelector();

    void startDiscovery(const QBluetoothUuid &uuid);
    void stopDiscovery();

private:
    Ui_ConnectWindow* ui;

    QBluetoothServiceDiscoveryAgent *m_discoveryAgent;
    QBluetoothServiceInfo m_service;
    QMap<QListWidgetItem *, QBluetoothServiceInfo> m_discoveredServices;

signals:
    void createConnection(QBluetoothServiceInfo service);

private slots:
    void serviceDiscovered(const QBluetoothServiceInfo &serviceInfo);
    void discoveryFinished();
    void deviceItemActivated(QListWidgetItem *item);
    void deviceItemClicked(QListWidgetItem *item);
    void cancelClicked();
    void connectClicked();
};

#endif // REMOTESELECTOR_H
