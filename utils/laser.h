#ifndef LASER_H
#define LASER_H

#include <QVector3D>

class Camera;
class Space;

class Laser
{
private:
    unsigned int VAO, VBO;
    QVector3D dir, center;
    float speed;
public:
    Laser(QVector3D dir, QVector3D center, float speed);
    ~Laser();

    void update(float deltaTime);
    void render(Camera *camera);
    int checkCollisions(Space *space);

    QVector3D getCenter();
};

#endif // LASER_H
