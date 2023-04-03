#include "noisedterrain.h"

#include <QVector3D>

#include "../utils/perlinnoise.h"

NoisedTerrain::NoisedTerrain()
{
    this->noiseFn = new PerlinNoise(2137);
}


float NoisedTerrain::TerrainHeight(QVector3D position){
    float tmp = noiseFn->noise(position.x(), position.y(), position.z());
    tmp = (tmp * 2.0) - 1.0;
    return (1 + tmp * 0.2);
}
