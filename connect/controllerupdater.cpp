
#include "controllerupdater.h"

#include <QVector3D>

ControllerUpdater::ControllerUpdater(ConnectManager* connectManager)
{
    this->connectManager = connectManager;
    lastSend = QTime::currentTime();
}


void ControllerUpdater::update(QVector3D position, float speed, int collision) {
//    qDebug() << "got" << position << speed;
    const int msecBreak = 20;
    if(lastSend.msecsTo(QTime::currentTime()) < msecBreak) {
        return;
    }
    lastSend = QTime::currentTime();

    QByteArray buffer;

    float x = position.x();
    float y = position.y();
    float z = position.z();
    float hash = x + y + z + speed + collision;
    buffer.append('\x00');
    buffer.append('\x00');
    buffer.append('\x00');
    buffer.append('\x00');
    buffer.append('\xFF');
    buffer.append('\xFF');
    buffer.append('\xFF');
    buffer.append('\xFF');

    buffer.append(reinterpret_cast<const char*>(&x), sizeof(x));
    buffer.append(reinterpret_cast<const char*>(&y), sizeof(y));
    buffer.append(reinterpret_cast<const char*>(&z), sizeof(z));
    buffer.append(reinterpret_cast<const char*>(&speed), sizeof(speed));
    buffer.append(reinterpret_cast<const char*>(&collision), sizeof(collision));
    buffer.append(reinterpret_cast<const char*>(&hash), sizeof(hash));

    buffer.append('\x00');
    buffer.append('\x00');
    buffer.append('\x00');
    buffer.append('\x00');
    buffer.append('\x00');
    buffer.append('\x00');
    buffer.append('\x00');
    buffer.append('\x00');
    connectManager->send(buffer);
}

