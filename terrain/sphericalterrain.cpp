#include "sphericalterrain.h"

#include <QVector3D>

SphericalTerrain::SphericalTerrain(float radius)
{
    this->radius = radius;
}


float SphericalTerrain::terrainHeight(QVector3D position){
    return radius;
}
