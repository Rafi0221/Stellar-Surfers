
#ifndef SETUPGAME_H
#define SETUPGAME_H


#include <QObject>
#include <QMainWindow>
#include <QCloseEvent>

#include "saves/gamesave.h"

QT_BEGIN_NAMESPACE
namespace Ui { class SetupWindow; }
QT_END_NAMESPACE

class SetupGame : public QMainWindow
{
    Q_OBJECT
public:
    explicit SetupGame(QWidget *parent = nullptr);
    void startClicked();
    void closeEvent(QCloseEvent* event);

signals:
    void closed();

private:
    void updateSaveListWidget();

    QList<GameSave> saveList;

    Ui::SetupWindow *ui;
};

#endif // SETUPGAME_H
