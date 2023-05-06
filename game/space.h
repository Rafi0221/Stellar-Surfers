
#ifndef SPACE_H
#define SPACE_H

#include <QVector3D>
#include <QVector>
#include <QHash>
#include "../terrain/planet.h"


class Space
{
public:
    Space(int seed);
    void update(QVector3D cameraPosition);
    void initialize();
    void render();

private:
    int seed;
    QVector3D position;
    QVector<Planet*> planets;
    QHash<QVector3D, bool> takenCoords;

    int hashCoordinates(const QVector3D & coordinates);
    void checkAddPlanet(const QVector3D & coordinates);
    bool isInRange(const QVector3D & pos, const QVector3D & coordinates);
};

#endif // SPACE_H
