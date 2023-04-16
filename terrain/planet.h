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
public:
    Planet(float radius = 1.0f);
    void update(QVector3D cameraPosition);
    void render();
    void setPosition(QVector3D position);
    void setRotation(QVector3D rotation);
};

#endif // PLANET_H
