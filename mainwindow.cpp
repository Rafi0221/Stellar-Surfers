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
    this->setWindowState(Qt::WindowMaximized);
    this->setWindowTitle("Stellar Surfers");
    this->setWindowIcon(QIcon(":/media/media/ic_launcher.png"));

    QPixmap bkgnd(":/media/media/galaxy.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Window, bkgnd);
    this->setPalette(palette);

    connect(ui->playButton, &QPushButton::clicked, this, &MainWindow::playClicked);
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
    SetupGame *s = new SetupGame(connectManager);
    //connect(s, &SetupGame::closed, this, &MainWindow::show);
    //this->hide();
    s->show();

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

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QPixmap bkgnd(":/media/media/galaxy.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Window, bkgnd);
    this->setPalette(palette);
    QMainWindow::resizeEvent(event);
}
