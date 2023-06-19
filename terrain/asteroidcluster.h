#ifndef ASTEROIDCLUSTER_H
#define ASTEROIDCLUSTER_H

#include <QVector>
#include <QVector3D>
#include <QMatrix4x4>

class AsteroidCluster
{
public:
    AsteroidCluster(int seed, QVector3D coordinates);

    QVector3D getPosition();
    int getNumberOfAsteroids();
    QVector<QMatrix4x4> getRotationMatrices();
    bool checkCollision(QVector3D cameraPosition);
    void update();
    bool collisionAhead(QVector3D position, QVector3D direction, float distance, float width);
    QVector3D getCollisionPoint(QVector3D position, QVector3D direction, float distance);
    void deleteAsteroid(QVector3D asteroidPosition);
    QVector3D getAsteroid(QVector3D asteroidPosition);
private:
    int seed;
    int numOfDeleted = 0;
    QVector3D position;
    QVector<QMatrix4x4> rotationMatrices;
    QVector<float> scales;
    QVector<QVector3D> movementVectors;
    QVector<float> speeds;
    QVector<bool> destroyed;
};

#endif // ASTEROIDCLUSTER_H
