#include "terraintype.h"

#include <QVector3D>

TerrainType::TerrainType(float radius)
{
    this->radius = radius;
}

QVector3D TerrainType::cubeToSphere(QVector3D position){
    float x2 = position.x() * position.x();
    float y2 = position.y() * position.y();
    float z2 = position.z() * position.z();

    float x3 = position.x() * sqrt(1 - y2/2.0 - z2/2.0 + z2*y2/3.0);
    float y3 = position.y() * sqrt(1 - z2/2.0 - x2/2.0 + z2*x2/3.0);
    float z3 = position.z() * sqrt(1 - x2/2.0 - y2/2.0 + x2*y2/3.0);

    return QVector3D(x3,y3,z3) * radius;
}
