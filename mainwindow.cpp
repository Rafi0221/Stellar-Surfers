#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "connect/connectmanager.h"
#include "play/setupgame.h"
#include "game/gamewindow.h"

#include <QSurfaceFormat>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Stellar Surfers");
    connect(ui->playButton, &QPushButton::clicked, this, &MainWindow::playClicked);
    connect(ui->connectButton, &QPushButton::clicked, this, &MainWindow::connectClicked);
    connect(ui->testButton, &QPushButton::clicked, this, &MainWindow::testClicked);

    connectManager = new ConnectManager();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete connectManager;
}

void MainWindow::playClicked()
{
    SetupGame *s = new SetupGame();
    //connect(s, &SetupGame::closed, this, &MainWindow::show);
    //this->hide();
    s->show();

}

void MainWindow::connectClicked()
{
    connectManager->show();
}

void MainWindow::testClicked()
{
    QSurfaceFormat format;
//    format.setSamples(16);
    format.setDepthBufferSize(24);
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);

    GameWindow *gameWindow = new GameWindow();
    gameWindow->resize(640, 480);
    gameWindow->showMaximized();

    gameWindow->setAnimating(true);
}
