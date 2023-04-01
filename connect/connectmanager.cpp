// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "connectmanager.h"
#include "remoteselector.h"
#include "server.h"
#include "client.h"

#include <QCoreApplication>
#include <QtCore/qdebug.h>

#include <QtBluetooth/qbluetoothdeviceinfo.h>
#include <QtBluetooth/qbluetoothlocaldevice.h>
#include <QtBluetooth/qbluetoothuuid.h>

static const QLatin1String serviceUuid("e8e10f95-1a70-4b27-9ccf-02010264e9c8");
#ifdef Q_OS_ANDROID
static const QLatin1String reverseUuid("c8e96402-0102-cf9c-274b-701a950fe1e8");
#endif

ConnectManager::ConnectManager(QWidget *parent)
    : QDialog(parent), ui(new Ui_ConnectWindow)
{
    //! [Construct UI]
    ui->setupUi(this);

    connect(ui->quitButton, &QPushButton::clicked, this, &ConnectManager::accept);
    connect(ui->connectButton, &QPushButton::clicked, this, &ConnectManager::connectClicked);
    connect(ui->sendButton, &QPushButton::clicked, this, &ConnectManager::sendClicked);
    //! [Construct UI]

    localAdapters = QBluetoothLocalDevice::allDevices();
    if (localAdapters.size() < 2) {
        ui->localAdapterBox->setVisible(false);
    } else {
        //we ignore more than two adapters
        ui->localAdapterBox->setVisible(true);
        ui->firstAdapter->setText(tr("Default (%1)", "%1 = Bluetooth address").
                                  arg(localAdapters.at(0).address().toString()));
        ui->secondAdapter->setText(localAdapters.at(1).address().toString());
        ui->firstAdapter->setChecked(true);
        connect(ui->firstAdapter, &QRadioButton::clicked, this, &ConnectManager::newAdapterSelected);
        connect(ui->secondAdapter, &QRadioButton::clicked, this, &ConnectManager::newAdapterSelected);
    }

    // make discoverable
    if (!localAdapters.isEmpty()) {
        QBluetoothLocalDevice adapter(localAdapters.at(0).address());
        adapter.setHostMode(QBluetoothLocalDevice::HostDiscoverable);
    } else {
        qWarning("Local adapter is not found! The application might work incorrectly.");
#ifdef Q_OS_WIN
        // WinRT implementation does not support adapter information yet. So it
        // will always return an empty list.
        qWarning("If the adapter exists, make sure to pair the devices manually before launching"
                 " the chat.");
#endif
    }

    //! [Create Chat Server]
    server = new Server(this);
    connect(server, QOverload<const QString &>::of(&Server::clientConnected),
            this, &ConnectManager::clientConnected);
    connect(server, QOverload<const QString &>::of(&Server::clientDisconnected),
            this,  QOverload<const QString &>::of(&ConnectManager::clientDisconnected));
    connect(server, &Server::messageReceived,
            this,  &ConnectManager::showMessage);
    connect(this, &ConnectManager::sendMessage, server, &Server::sendMessage);
    server->startServer();
    //! [Create Chat Server]

    //! [Get local device name]
    localName = QBluetoothLocalDevice().name();
    //! [Get local device name]
}

ConnectManager::~ConnectManager()
{
    qDeleteAll(clients);
    delete server;
}

//! [clientConnected clientDisconnected]
void ConnectManager::clientConnected(const QString &name)
{
    ui->chat->insertPlainText(QString::fromLatin1("%1 has joined chat.\n").arg(name));
}

void ConnectManager::clientDisconnected(const QString &name)
{
    ui->chat->insertPlainText(QString::fromLatin1("%1 has left.\n").arg(name));
}
//! [clientConnected clientDisconnected]

//! [connected]
void ConnectManager::connected(const QString &name)
{
    ui->chat->insertPlainText(QString::fromLatin1("Joined chat with %1.\n").arg(name));
}
//! [connected]

void ConnectManager::newAdapterSelected()
{
    const int newAdapterIndex = adapterFromUserSelection();
    if (currentAdapterIndex != newAdapterIndex) {
        server->stopServer();
        currentAdapterIndex = newAdapterIndex;
        const QBluetoothHostInfo info = localAdapters.at(currentAdapterIndex);
        QBluetoothLocalDevice adapter(info.address());
        adapter.setHostMode(QBluetoothLocalDevice::HostDiscoverable);
        server->startServer(info.address());
        localName = info.name();
    }
}

int ConnectManager::adapterFromUserSelection() const
{
    int result = 0;
    QBluetoothAddress newAdapter = localAdapters.at(0).address();

    if (ui->secondAdapter->isChecked()) {
        newAdapter = localAdapters.at(1).address();
        result = 1;
    }
    return result;
}

void ConnectManager::reactOnSocketError(const QString &error)
{
    ui->chat->insertPlainText(QString::fromLatin1("%1\n").arg(error));
}

//! [clientDisconnected]
void ConnectManager::clientDisconnected()
{
    Client *client = qobject_cast<Client *>(sender());
    if (client) {
        clients.removeOne(client);
        client->deleteLater();
    }
}
//! [clientDisconnected]

//! [Connect to remote service]
void ConnectManager::connectClicked()
{
    ui->connectButton->setEnabled(false);

    // scan for services
    const QBluetoothAddress adapter = localAdapters.isEmpty() ?
                                           QBluetoothAddress() :
                                           localAdapters.at(currentAdapterIndex).address();

    RemoteSelector remoteSelector(adapter);
#ifdef Q_OS_ANDROID
    // QTBUG-61392
    Q_UNUSED(serviceUuid);
    remoteSelector.startDiscovery(QBluetoothUuid(reverseUuid));
#else
    remoteSelector.startDiscovery(QBluetoothUuid(serviceUuid));
#endif
    if (remoteSelector.exec() == QDialog::Accepted) {
        QBluetoothServiceInfo service = remoteSelector.service();

        qDebug() << "Connecting to service 2" << service.serviceName()
                 << "on" << service.device().name();

        // Create client
        qDebug() << "Going to create client";
        Client *client = new Client(this);
qDebug() << "Connecting...";

        connect(client, &Client::messageReceived,
                this, &ConnectManager::showMessage);
        connect(client, &Client::disconnected,
                this, QOverload<>::of(&ConnectManager::clientDisconnected));
        connect(client, QOverload<const QString &>::of(&Client::connected),
                this, &ConnectManager::connected);
        connect(client, &Client::socketErrorOccurred,
                this, &ConnectManager::reactOnSocketError);
        connect(this, &ConnectManager::sendMessage, client, &Client::sendMessage);
qDebug() << "Start client";
        client->startClient(service);

        clients.append(client);
    }

    ui->connectButton->setEnabled(true);
}
//! [Connect to remote service]

//! [sendClicked]
void ConnectManager::sendClicked()
{
    ui->sendButton->setEnabled(false);
    ui->sendText->setEnabled(false);

    showMessage(localName, ui->sendText->text());
    emit sendMessage(ui->sendText->text());

    ui->sendText->clear();

    ui->sendText->setEnabled(true);
    ui->sendButton->setEnabled(true);
    ui->sendText->setFocus();
}
//! [sendClicked]

//! [showMessage]
void ConnectManager::showMessage(const QString &sender, const QString &message)
{
    ui->chat->insertPlainText(QString::fromLatin1("%1: %2\n").arg(sender, message));
    ui->chat->ensureCursorVisible();
}
//! [showMessage]
