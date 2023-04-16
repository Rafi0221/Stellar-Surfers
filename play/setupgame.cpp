
#include "setupgame.h"
#include "ui_setupwindow.h"
#include "saves/datasaver.h"

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

    unsigned int gameSeed = gen();
    QDateTime date = QDateTime::currentDateTime();

    saveList.push_front(GameSave(QString::fromStdString(seedString), date));
    updateSaveListWidget();
    DataSaver::write(saveList);

    qDebug() << "starting a new game with seed " << gameSeed;
}

void SetupGame::updateSaveListWidget() {
    ui->saveListWidget->clear();
    for(auto &s : saveList) {
        const QString label = s.seedString + u' ' + s.date.toString();
        QListWidgetItem *item = new QListWidgetItem(label);
        ui->saveListWidget->addItem(item);
    }
}


void SetupGame::closeEvent(QCloseEvent* event) {
    emit closed();
    event->accept();
}

