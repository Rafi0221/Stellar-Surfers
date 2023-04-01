// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "ui_connectwindow.h"

#include <QtWidgets/qdialog.h>

#include <QtBluetooth/qbluetoothhostinfo.h>

QT_USE_NAMESPACE

class Server;
class Client;

//! [declaration]
class ConnectManager : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectManager(QWidget *parent = nullptr);
    ~ConnectManager();

signals:
    void sendMessage(const QString &message);

private slots:
    void connectClicked();
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

    Server *server;
    QList<Client *> clients;
    QList<QBluetoothHostInfo> localAdapters;

    QString localName;
};
//! [declaration]
