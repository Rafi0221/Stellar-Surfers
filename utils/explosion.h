#ifndef EXPLOSION_H
#define EXPLOSION_H

#include <QVector3D>
#include <QMatrix4x4>

class Camera;

class Explosion
{
private:
    float currentTime = 0;

    QVector3D velocities[5000];
    QVector3D positions[5000];
    float angularVelocities[5000];
    float rotations[5000];
    int type[5000];

    float data[5000 * 6 * 5];
    float data2[5000 * 6];

    unsigned int VAO, fVBO, iVBO;

public:
    Explosion(QVector3D position);
    void render();
    void update(Camera *camera, float deltaTime);
};

#endif // EXPLOSION_H
