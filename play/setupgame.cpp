#include "setupgame.h"
#include "ui_setupwindow.h"
#include "saves/datasaver.h"
#include "../game/gamewindow.h"

#include <QDebug>
#include <random>

static const int defaultSeedLength = 10;

SetupGame::SetupGame(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SetupWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Stellar Surfers");

    connect(ui->startButton, &QPushButton::clicked, this, &SetupGame::startClicked);
    connect(ui->saveListWidget, &QListWidget::itemActivated, this, &SetupGame::itemActivated);

    saveList = DataSaver::read();
    updateSaveListWidget();
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
    GameWindow *gameWindow = new GameWindow();
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
        const QString label = s.seedString + u' ' + s.date.toString();
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

void SetupGame::closeEvent(QCloseEvent* event) {
    emit closed();
    event->accept();
}

