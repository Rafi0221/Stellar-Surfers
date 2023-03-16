#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "connect/connectwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Stellar Surfers");
    connect(ui->playButton, &QPushButton::clicked, this, &MainWindow::playClicked);
    connect(ui->connectButton, &QPushButton::clicked, this, &MainWindow::connectClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::playClicked()
{
    qDebug() << "xdd";
}

void MainWindow::connectClicked()
{
    qDebug() << "connect";
    ConnectWindow* connectWindow = new ConnectWindow();
    connectWindow->setAttribute(Qt::WA_DeleteOnClose);
    connectWindow->show();
}

