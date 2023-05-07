#ifndef PLANET_H
#define PLANET_H

#include <QVector3D>

class PatchFactory;
class TerrainFace;

class Planet
{
private:
    TerrainFace *faces[6];

    QVector3D position;
    QVector3D rotation;

    float radius;

    PatchFactory *factory;

public:
    Planet(PatchFactory *factory, float radius = 1.0f);
    void update(QVector3D cameraPosition);
    void render();
    bool checkCollision(QVector3D cameraPosition);
    void setPosition(QVector3D position);
    void setRotation(QVector3D rotation);
    QVector3D getPosition();
    float getRadius();
};

#endif // PLANET_H
