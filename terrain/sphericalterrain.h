#ifndef SPHERICALTERRAIN_H
#define SPHERICALTERRAIN_H

#include "terraintype.h"

class QVector3D;

class SphericalTerrain : public TerrainType
{
public:
    SphericalTerrain();

    virtual float TerrainHeight(QVector3D position) override;
};

#endif // SPHERICALTERRAIN_H
