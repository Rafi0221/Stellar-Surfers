#include "noisedpatchfactory.h"

#include "../utils/layeredperlinnoise.h"
#include "noisedpatch.h"
#include "planetproperties.h"

NoisedPatchFactory::NoisedPatchFactory(int seed)
{
    noise = new LayeredPerlinNoise(seed);
}

NoisedPatchFactory::~NoisedPatchFactory(){
    delete noise;
}

Patch* NoisedPatchFactory::getPatch(QMatrix4x4 relativeRotation, float scale, QVector2D relativePosition,  PlanetProperties *properties){
    return new NoisedPatch(relativeRotation, scale, relativePosition, properties->radius, noise, properties->colorMapTexture);
}
