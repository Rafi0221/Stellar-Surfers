
#ifndef SETUPGAME_H
#define SETUPGAME_H


#include <QObject>
#include <QMainWindow>
#include <QCloseEvent>

#include "saves/gamesave.h"

QT_BEGIN_NAMESPACE
namespace Ui { class SetupWindow; }
QT_END_NAMESPACE

QT_FORWARD_DECLARE_CLASS(QListWidgetItem)

class SetupGame : public QMainWindow
{
    Q_OBJECT
public:
    explicit SetupGame(QWidget *parent = nullptr);
    void startClicked();
    void closeEvent(QCloseEvent* event);

    struct GameSeed {
        int seed;
        float f1;
        float f2;
        float f3;
    };

signals:
    void closed();
    void startGame();

private slots:
    void itemActivated(QListWidgetItem *item);

private:
    void updateSaveListWidget();

    QList<GameSave> saveList;

    Ui::SetupWindow *ui;
};

#endif // SETUPGAME_H
