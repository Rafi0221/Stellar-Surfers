
#include "controllerupdater.h"

#include <QVector3D>

ControllerUpdater::ControllerUpdater(ConnectManager* connectManager)
{
    this->connectManager = connectManager;
}


void ControllerUpdater::update(QVector3D position, float speed, int collision) {
//    qDebug() << "got" << position << speed;

    QByteArray buffer;
    float x = position.x();
    float y = position.y();
    float z = position.z();
    buffer.append(reinterpret_cast<const char*>(&x), sizeof(x));
    buffer.append(reinterpret_cast<const char*>(&y), sizeof(y));
    buffer.append(reinterpret_cast<const char*>(&z), sizeof(z));
    buffer.append(reinterpret_cast<const char*>(&speed), sizeof(speed));
    // buffer.append(reinterpret_cast<const char*>(&collision), sizeof(collision));

    connectManager->send(buffer);
}

