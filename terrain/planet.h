#ifndef PLANET_H
#define PLANET_H

class PlanetLayer;
class PlanetProperties;

#include <QVector3D>
#include <QMatrix4x4>

class Planet
{
public:
    Planet(int seed, float radius);
    ~Planet();
    void update(QVector3D cameraPosition);
    void render();
    bool checkCollision(QVector3D cameraPosition);
    void setPosition(QVector3D position);
    void setRotation(QVector3D rotation);
    QVector3D getPosition();
    float getRadius();
private:
    PlanetLayer *solidLayer = nullptr;
    PlanetLayer *waterLayer = nullptr;
    PlanetProperties *properties;

    QVector3D position;
    QVector3D rotation;

    float radius;
};

#endif // PLANET_H
