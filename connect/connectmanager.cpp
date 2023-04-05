// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "connectmanager.h"
#include "ui_connectwindow.h"
#include "client.h"

#include <qbluetoothaddress.h>
#include <qbluetoothdevicediscoveryagent.h>
#include <qbluetoothlocaldevice.h>

#include <QMenu>
#include <QDebug>

static QColor colorForPairing(QBluetoothLocalDevice::Pairing pairing)
{
    return pairing == QBluetoothLocalDevice::Paired
           || pairing == QBluetoothLocalDevice::AuthorizedPaired
           ? QColor(Qt::green) : QColor(Qt::red);
}

ConnectManager::ConnectManager(QWidget *parent) :
    QDialog(parent),
    localDevice(new QBluetoothLocalDevice),
    ui(new Ui_ConnectWindow)
{
    ui->setupUi(this);
    ui->stopScan->setVisible(false);

    discoveryAgent = new QBluetoothDeviceDiscoveryAgent();

    connect(ui->scan, &QAbstractButton::clicked, this, &ConnectManager::startScan);
    connect(ui->stopScan, &QAbstractButton::clicked, this, &ConnectManager::stopScan);
    connect(ui->power, &QPushButton::clicked, this, &ConnectManager::powerClicked);

    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this, &ConnectManager::addDevice);
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished,
            this, &ConnectManager::scanFinished);

    connect(ui->list, &QListWidget::itemActivated,
            this, &ConnectManager::itemActivated);

    connect(localDevice, &QBluetoothLocalDevice::hostModeStateChanged,
            this, &ConnectManager::hostModeStateChanged);

    hostModeStateChanged(localDevice->hostMode());
}

ConnectManager::~ConnectManager()
{
    delete discoveryAgent;
}

void ConnectManager::addDevice(const QBluetoothDeviceInfo &info)
{
    const QString label = info.address().toString() + u' ' + info.name();
    const auto items = ui->list->findItems(label, Qt::MatchExactly);
    if (items.isEmpty()) {
        QListWidgetItem *item = new QListWidgetItem(label);
        QBluetoothLocalDevice::Pairing pairingStatus = localDevice->pairingStatus(info.address());
        item->setForeground(colorForPairing(pairingStatus));
        ui->list->addItem(item);
    }
}

void ConnectManager::startScan()
{
    discoveryAgent->start();
    ui->scan->setVisible(false);
    ui->stopScan->setVisible(true);
}

void ConnectManager::stopScan()
{
    discoveryAgent->stop();
    scanFinished();
}

void ConnectManager::scanFinished()
{
    ui->scan->setVisible(true);
    ui->stopScan->setVisible(false);
}

void ConnectManager::itemActivated(QListWidgetItem *item)
{
    const QString text = item->text();
    const auto index = text.indexOf(' ');
    if (index == -1)
        return;

    QBluetoothAddress address(text.left(index));
    QString name(text.mid(index + 1));

    if (client != nullptr)
        delete client;
    client = new Client(name, address);
}

void ConnectManager::powerClicked(bool clicked)
{
    if (clicked)
        localDevice->powerOn();
    else
        localDevice->setHostMode(QBluetoothLocalDevice::HostPoweredOff);
}

void ConnectManager::hostModeStateChanged(QBluetoothLocalDevice::HostMode mode)
{
    ui->power->setChecked(mode != QBluetoothLocalDevice::HostPoweredOff);;

    const bool on = mode != QBluetoothLocalDevice::HostPoweredOff;
    ui->scan->setEnabled(on);
}

