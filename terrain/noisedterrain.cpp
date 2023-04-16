#include "noisedterrain.h"

#include <QVector3D>

#include "../utils/layeredperlinnoise.h"

NoisedTerrain::NoisedTerrain(float radius) : TerrainType(radius)
{
    this->noise = new LayeredPerlinNoise();
}


float NoisedTerrain::terrainHeight(QVector3D position){
    float tmp = noise->getValue(position.x(), position.y(), position.z());
    tmp = (tmp * 2.0) - 1.0;
    return (1 + tmp * 0.2);
}
