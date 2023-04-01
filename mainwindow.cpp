#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "connect/connectwindow.h"
#include "test/trianglewindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Stellar Surfers");
    connect(ui->playButton, &QPushButton::clicked, this, &MainWindow::playClicked);
    connect(ui->connectButton, &QPushButton::clicked, this, &MainWindow::connectClicked);
    connect(ui->testButton, &QPushButton::clicked, this, &MainWindow::testClicked);
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

void MainWindow::testClicked()
{
    QSurfaceFormat format;
    format.setSamples(16);
    format.setDepthBufferSize(16);

    TriangleWindow* triangleWindow = new TriangleWindow();
    triangleWindow->setFormat(format);
    triangleWindow->resize(640, 480);
    triangleWindow->show();

    triangleWindow->setAnimating(true);
}
