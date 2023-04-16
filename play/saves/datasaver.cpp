
#include "datasaver.h"

#include <QFile>

static const QString saveFileName = "savefile.dat";

DataSaver::DataSaver()
{
}

QDataStream & operator<<(QDataStream &out, const GameSave& gameSave) {
    out << gameSave.seedString << gameSave.date;
    return out;
}

QDataStream & operator>>(QDataStream &in, GameSave& gameSave) {
    in >> gameSave.seedString >> gameSave.date;
    return in;
}

QList<GameSave> DataSaver::read() {
    QList<GameSave> result;

    QFile file(saveFileName);
    QDataStream in(&file);
    file.open(QIODevice::ReadOnly);
    in >> result;
    file.close();

    return result;
}

void DataSaver::write(QList<GameSave> &list) {
    qDebug() << "writing...";
    QFile file(saveFileName);
    QDataStream out(&file);
    file.open(QIODevice::WriteOnly);
    out << list;
    file.close();
}

