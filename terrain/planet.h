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
    float radius;
public:
    Planet(float radius = 1.0f, int seed = 0);
    void update(QVector3D cameraPosition);
    void render();
    bool checkCollision(QVector3D cameraPosition);
    void setPosition(QVector3D position);
    void setRotation(QVector3D rotation);
    QVector3D getPosition();
    float getRadius();
};

#endif // PLANET_H
