
#ifndef DATASAVER_H
#define DATASAVER_H


#include <QList>

#include "gamesave.h"

class DataSaver
{
public:
    DataSaver();

    static QList<GameSave> read();
    static void write(QList<GameSave> & list);
};

#endif // DATASAVER_H
