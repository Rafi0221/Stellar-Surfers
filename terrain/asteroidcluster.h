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

private:
    int seed;
    QVector3D position;
    QVector<QMatrix4x4> rotationMatrices;
};

#endif // ASTEROIDCLUSTER_H