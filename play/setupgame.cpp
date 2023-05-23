#include "setupgame.h"
#include "ui_setupwindow.h"
#include "saves/datasaver.h"
#include "../game/gamewindow.h"
#include "../connect/connectmanager.h"

#include <QDebug>
#include <random>

static const int defaultSeedLength = 10;

SetupGame::SetupGame(ConnectManager* connectManager, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SetupWindow)
{
    this->connectManager = connectManager;

    ui->setupUi(this);
    this->setWindowState(Qt::WindowMaximized);
    this->setWindowTitle("Stellar Surfers");
    this->setWindowIcon(QIcon(":/media/media/ic_launcher.png"));

    QPixmap bkgnd(":/media/media/galaxy.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Window, bkgnd);
    this->setPalette(palette);

    connect(ui->connectButton, &QPushButton::clicked, this, &SetupGame::connectClicked);
    connect(ui->startButton, &QPushButton::clicked, this, &SetupGame::startClicked);
    connect(ui->saveListWidget, &QListWidget::itemActivated, this, &SetupGame::itemActivated);

    connect(this, &SetupGame::startGame, connectManager, [=]() { connectManager->send("Start!"); } );
    connect(connectManager, &ConnectManager::connectionLost, this, &SetupGame::disconnected);
    connect(connectManager, &ConnectManager::deviceConnected, this, &SetupGame::connected);
    if(connectManager->isConnected())
        ui->disconnectedLabel->hide();
    else
        ui->connectedLabel->hide();

    saveList = DataSaver::read();
    updateSaveListWidget();
}

void SetupGame::connectClicked() {
    connectManager->show();
}

void SetupGame::startClicked() {
    std::string seedString = ui->seedField->text().toStdString();

    if(seedString.empty()) {
        std::mt19937 mt(std::chrono::system_clock::now().time_since_epoch().count());
        auto randchar = [&]() -> char
        {
            const char charset[] =
                "0123456789"
                "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                "abcdefghijklmnopqrstuvwxyz";
            const size_t max_index = (sizeof(charset) - 1);
            return charset[mt() % max_index];
        };
        seedString.resize(defaultSeedLength);
        std::generate_n( seedString.begin(), seedString.size(), randchar);
    }

    qDebug() << "seed: " << QString::fromStdString(seedString);

    std::seed_seq seedSeq(seedString.begin(), seedString.end());
    std::mt19937 gen(seedSeq);
    std::uniform_real_distribution<> dis(0.0, 1.0);

    GameSeed gs;
    gs.seed = gen();
    gs.f1 = dis(gen);
    gs.f2 = dis(gen);
    gs.f3 = dis(gen);
    QDateTime date = QDateTime::currentDateTime();

    saveList.push_front(GameSave(QString::fromStdString(seedString), date));
    updateSaveListWidget();
    DataSaver::write(saveList);

    qDebug() << "starting a new game with seed (" << gs.seed << "," << gs.f1 << "," << gs.f2 << "," << gs.f3 << ")";
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);
    GameWindow *gameWindow = new GameWindow();
    gameWindow->setControllerUpdater(new ControllerUpdater(connectManager));
    gameWindow->setSeed(gs);
    gameWindow->resize(640, 480);
    emit startGame();

    gameWindow->showMaximized();

    gameWindow->setAnimating(true);
    this->close();
}

void SetupGame::updateSaveListWidget() {
    ui->saveListWidget->clear();
    for(auto &s : saveList) {
        const QString label = s.seedString + " - " + s.date.toString("HH:mm dd.MM.yyyy");
        QListWidgetItem *item = new QListWidgetItem(label);
        ui->saveListWidget->addItem(item);
    }
}

void SetupGame::itemActivated(QListWidgetItem *item)
{
    const QString text = item->text();
    const auto index = text.indexOf(' ');
    if (index == -1)
        return;
    QString seed(text.left(index));
    ui->seedField->setText(seed);
}

void SetupGame::connected() {
    ui->disconnectedLabel->hide();
    ui->connectedLabel->show();
}

void SetupGame::disconnected() {
    ui->connectedLabel->hide();
    ui->disconnectedLabel->show();
}

void SetupGame::closeEvent(QCloseEvent* event) {
    emit closed();
    event->accept();
}

void SetupGame::resizeEvent(QResizeEvent *event)
{
    QPixmap bkgnd(":/media/media/galaxy.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Window, bkgnd);
    this->setPalette(palette);
    QMainWindow::resizeEvent(event);
}

