
#include "controllerupdater.h"
#include "../utils/consts.h"

#include <QVector3D>

ControllerUpdater::ControllerUpdater(ConnectManager* connectManager)
{
    this->connectManager = connectManager;
    lastSend = QTime::currentTime();
}


void ControllerUpdater::update(QVector3D position, float speed, bool collision, bool collisionAheadPlanet, bool collisionAheadAsteroid) {
//    qDebug() << "got" << position << speed;
    const int msecBreak = 20;
    if(lastSend.msecsTo(QTime::currentTime()) < msecBreak) {
        return;
    }
    lastSend = QTime::currentTime();

    int collisionInfo = INFO_SAFE;
    if (collisionAheadPlanet) collisionInfo = INFO_COLLISION_AHEAD_PLANET;
    if (collisionAheadAsteroid) collisionInfo = INFO_COLLISION_AHEAD_ASTEROID;
    if (collision) collisionInfo = INFO_COLLISION;

    QByteArray buffer;

    float x = position.x();
    float y = position.y();
    float z = position.z();
    float hash = x + y + z + speed + collisionInfo;

    if (collisionInfo == INFO_COLLISION)
        qDebug() << "sending collision info\n";

    buffer.append(reinterpret_cast<const char*>(&x), sizeof(x));
    buffer.append(reinterpret_cast<const char*>(&y), sizeof(y));
    buffer.append(reinterpret_cast<const char*>(&z), sizeof(z));
    buffer.append(reinterpret_cast<const char*>(&speed), sizeof(speed));
    buffer.append(reinterpret_cast<const char*>(&collisionInfo), sizeof(collisionInfo));
    buffer.append(reinterpret_cast<const char*>(&hash), sizeof(hash));

    connectManager->send(buffer);
}

