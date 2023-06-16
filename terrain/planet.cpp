#include "planet.h"

#include "noisedpatchfactory.h"
#include "planetlayer.h"
#include "planetproperties.h"
#include "waterpatchfactory.h"
#include "../utils/frustum.h"

Planet::Planet(int seed, float radius)
{
    this->radius = radius;
    properties = new PlanetProperties(seed, radius);
    if(properties->hasTerrain == true){
        solidLayer = new PlanetLayer(new NoisedPatchFactory(seed), properties);
    }
    if(properties->hasWater == true){
        waterLayer = new PlanetLayer(new WaterPatchFactory(seed), properties);
    }
}

Planet::~Planet(){
    if(solidLayer != nullptr)
        delete solidLayer;
    if(waterLayer != nullptr)
        delete waterLayer;
    delete properties;
}

void Planet::update(QVector3D cameraPosition){
    if(solidLayer != nullptr)
        solidLayer->update(cameraPosition);
    if(waterLayer != nullptr)
        waterLayer->update(cameraPosition);
}

void Planet::render(Frustum *frustum){
    if(solidLayer != nullptr)
        solidLayer->render(frustum);
    if(waterLayer != nullptr)
        waterLayer->render(frustum);
}

bool Planet::checkCollision(QVector3D cameraPosition){
    bool ans = false;
    if(solidLayer != nullptr)
        ans |= solidLayer->checkCollision(cameraPosition);
    return ans;
}

void Planet::setPosition(QVector3D position){
    this->position = position;
    if(solidLayer != nullptr)
        solidLayer->setPosition(position);
    if(waterLayer != nullptr)
        waterLayer->setPosition(position);
}

void Planet::setRotation(QVector3D rotation){
    this->rotation = rotation;
    if(solidLayer != nullptr)
        solidLayer->setRotation(rotation);
    if(waterLayer != nullptr)
        waterLayer->setRotation(rotation);
}

QVector3D Planet::getPosition(){
    return position;
}

float Planet::getRadius(){
    return radius;
}

bool Planet::collisionAhead(QVector3D position, QVector3D direction, float distance) {
    if(this->position.distanceToLine(position, direction) > this->radius*0.9)
        return false;
    QVector3D planetCenter = this->position - position;
    float projection = QVector3D::dotProduct(direction, planetCenter);
    return projection >= 0 && projection <= distance;
}

QVector3D Planet::getCollisionPoint(QVector3D position, QVector3D direction, float distance) {
    QVector3D result(1e9, 1e9, 1e9);
    if(this->position.distanceToLine(position, direction) > this->radius*0.9)
        return result;
    QVector3D planetCenter = this->position - position;
    float projection = QVector3D::dotProduct(direction, planetCenter);
    result = direction * projection + position;
    return result;
}
