#ifndef NOISEDTERRAIN_H
#define NOISEDTERRAIN_H


#include "terraintype.h"

class QVector3D;
class PerlinNoise;

class NoisedTerrain : public TerrainType
{
private:
    PerlinNoise *noiseFn;
public:
    NoisedTerrain();
    virtual float TerrainHeight(QVector3D position) override;
};

#endif // NOISEDTERRAIN_H
