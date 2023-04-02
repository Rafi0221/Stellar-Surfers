// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause
#ifndef CONNECTMANAGER_H
#define CONNECTMANAGER_H

#include "ui_connectwindow.h"
#include "remoteselector.h"

#include <QtWidgets/qdialog.h>

#include <QtBluetooth/qbluetoothhostinfo.h>
#include <QBluetoothServiceInfo>

QT_USE_NAMESPACE

class Server;
class Client;

class ConnectManager : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectManager(QWidget *parent = nullptr);
    ~ConnectManager();

signals:
    void sendMessage(const QString &message);

public slots:
    void createClient(QBluetoothServiceInfo service);

private slots:
    void scanClicked();
    void sendClicked();

    void showMessage(const QString &sender, const QString &message);

    void clientConnected(const QString &name);
    void clientDisconnected(const QString &name);
    void clientDisconnected();
    void connected(const QString &name);
    void reactOnSocketError(const QString &error);

    void newAdapterSelected();

private:
    int adapterFromUserSelection() const;
    int currentAdapterIndex = 0;
    Ui_ConnectWindow *ui;

    RemoteSelector *remoteSelector;
    Server *server;
    QList<Client *> clients;
    QList<QBluetoothHostInfo> localAdapters;

    QString localName;
};

#endif // CONNECTMANAGER_H
