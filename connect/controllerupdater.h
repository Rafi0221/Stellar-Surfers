
#ifndef CONTROLLERUPDATER_H
#define CONTROLLERUPDATER_H


#include "connectmanager.h"

#include <QObject>
#include <QTime>


class ControllerUpdater : public QObject
{
    Q_OBJECT
public:
    ControllerUpdater(ConnectManager* connectManager);
    void update(QVector3D position, float speed, bool collision, bool collisionAheadPlanet, bool collisionAheadAsteroid);

private:
    ConnectManager* connectManager;
    QTime lastSend;
};

#endif // CONTROLLERUPDATER_H
