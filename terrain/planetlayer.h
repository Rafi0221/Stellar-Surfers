#ifndef PLANET_LAYER_H
#define PLANET_LAYER_H

#include <QVector3D>

class Frustum;
class PatchFactory;
class TerrainFace;
class PlanetProperties;

class PlanetLayer
{
private:
    TerrainFace *faces[6];
    PlanetProperties *properties;

    QVector3D position;
    QVector3D rotation;

    float radius;

    PatchFactory *factory;

public:
    PlanetLayer(PatchFactory *factory, PlanetProperties* properties);
    ~PlanetLayer();
    void update(QVector3D cameraPosition);
    void render(Frustum *frustum);
    bool checkCollision(QVector3D cameraPosition);
    void setPosition(QVector3D position);
    void setRotation(QVector3D rotation);
    QVector3D getPosition();
    float getRadius();
};

#endif // PLANET_LAYER_H
