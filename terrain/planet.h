#ifndef PLANET_H
#define PLANET_H

#include <QVector3D>

class TerrainFace;
class TerrainType;

class Planet
{
private:
    TerrainType *type;
    TerrainFace *faces[6];

    QVector3D position;
    QVector3D rotation;

    int seed;
public:
    Planet(float radius = 1.0f, int seed = 0);
    void update(QVector3D cameraPosition);
    void render();
    void setPosition(QVector3D position);
    void setRotation(QVector3D rotation);
    QVector3D getPosition();
};

#endif // PLANET_H
