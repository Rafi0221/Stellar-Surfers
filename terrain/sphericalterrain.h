#ifndef SPHERICALTERRAIN_H
#define SPHERICALTERRAIN_H

#include "terraintype.h"

class QVector3D;

class SphericalTerrain : public TerrainType
{
private:
    float radius;
public:
    SphericalTerrain(float radius = 1.0f);

    virtual float terrainHeight(QVector3D position) override;
};

#endif // SPHERICALTERRAIN_H
