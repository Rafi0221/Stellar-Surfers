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
#include <QMessageBox>
#include <QTimer>

static QColor colorForPairing(QBluetoothLocalDevice::Pairing pairing)
{
    return pairing == QBluetoothLocalDevice::Paired
           || pairing == QBluetoothLocalDevice::AuthorizedPaired
           ? QColor(0, 170, 0) : QColor(232, 0, 0);
}

ConnectManager::ConnectManager(QWidget *parent) :
    QDialog(parent),
    localDevice(new QBluetoothLocalDevice),
    ui(new Ui_ConnectWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/media/media/ic_launcher.png"));
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

    client = new Client();

    glupdater = new GLUpdater();
    connect(client, &Client::connectionLost, glupdater, &GLUpdater::setToDefault);
    connect(client, &Client::connectionLost, this, &ConnectManager::disconnected);
    connect(client, &Client::deviceConnected, this, &ConnectManager::connected);

    parser = new Parser(glupdater);
    connect(client, &Client::messageReceived, parser, &Parser::parse);

    hostModeStateChanged(localDevice->hostMode());
    startScan();
}

ConnectManager::~ConnectManager()
{
    delete discoveryAgent;
    delete client;
    delete parser;
    delete glupdater;
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

    client->startClient(address);
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

void ConnectManager::connected() {
    _isConnected = true;
    emit deviceConnected();
    this->hide();
}

void ConnectManager::disconnected() {
    _isConnected = false;
    emit connectionLost();
    this->show();
}

bool ConnectManager::isConnected() {
    return _isConnected;
}

void ConnectManager::send(const QByteArray &message) {
    client->sendMessage(message);
}
