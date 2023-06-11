#ifndef LASER_H
#define LASER_H

#include <QVector3D>

class Camera;

class Laser
{
private:
    unsigned int VAO, VBO;
    QVector3D dir, center;
public:
    Laser(QVector3D dir, QVector3D center);

    void Render(Camera *camera);
};

#endif // LASER_H
