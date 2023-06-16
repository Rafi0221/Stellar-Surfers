
#ifndef SPACE_H
#define SPACE_H

#include <QOpenGLShaderProgram>
#include <QVector3D>
#include <QVector>
#include <QHash>
#include "../utils/model.h"
#include "../terrain/asteroidcluster.h"
#include "../terrain/planet.h"

class Frustum;

class Space
{
public:
    Space(int seed);
    void update(QVector3D cameraPosition);
    void initialize();
    void render(Shader *asteroidShader, Frustum *frustum);
    bool checkCollision(QVector3D cameraPosition);
    bool collisionAheadPlanet(QVector3D position, QVector3D direction, float distance);
    bool collisionAheadAsteroid(QVector3D position, QVector3D direction, float distance);
    QVector3D getCollisionPointAsteroid(QVector3D position, QVector3D direction, float distance);
    QVector3D getCollisionPointPlanet(QVector3D position, QVector3D direction, float distance);
    void deleteAsteroid(QVector3D asteroidPosition);

private:
    int seed;
    QVector3D position;
    QVector<Planet*> planets;
    QHash<QVector3D, bool> takenCoords;

    Model *asteroidModel;
    unsigned int buffer;
    QVector<float> models;
    //float *models;
    QVector<AsteroidCluster*> asteroidClusters;

    int hashCoordinates(const QVector3D & coordinates);
    void checkAddObject(const QVector3D & coordinates);
    bool isInRange(const QVector3D & pos, const QVector3D & coordinates);
};

#endif // SPACE_H
