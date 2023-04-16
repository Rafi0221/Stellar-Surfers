
#include "gamesave.h"

GameSave::GameSave()
{

}

GameSave::GameSave(QString seedString, QDateTime date)
{
    this->seedString = seedString;
    this->date = date;
}


