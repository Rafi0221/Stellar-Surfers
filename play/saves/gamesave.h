
#ifndef GAMESAVE_H
#define GAMESAVE_H


#include <QObject>
#include <QDateTime>

class GameSave
{
public:
    GameSave();
    GameSave(QString seedString, QDateTime date);

    QString seedString;
    QDateTime date;
};




#endif // GAMESAVE_H
