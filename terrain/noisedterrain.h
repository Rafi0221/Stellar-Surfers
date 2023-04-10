#ifndef NOISEDTERRAIN_H
#define NOISEDTERRAIN_H


#include "terraintype.h"

class QVector3D;
class Noise;

class NoisedTerrain : public TerrainType
{
private:
    Noise *noise;
    float radius;
public:
    NoisedTerrain(float radius = 1.0f);
    virtual float terrainHeight(QVector3D position) override;
};

#endif // NOISEDTERRAIN_H
