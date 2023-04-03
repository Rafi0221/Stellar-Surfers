#ifndef PLANET_H
#define PLANET_H

#include <QVector3D>

#define NEAR 0
#define LEFT 1
#define FAR 2
#define RIGHT 3
#define UP 4
#define DOWN 5

class TerrainFace;
class TerrainType;

class Planet
{
private:
    TerrainType *type;
    TerrainFace *faces[6];

    QVector3D position;
public:
    Planet(TerrainType *type);
    void Update(QVector3D cameraPosition);
    void Render();
    void SetPosition(QVector3D position);
};

#endif // PLANET_H
