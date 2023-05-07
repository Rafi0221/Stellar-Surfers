#include "noisedpatchfactory.h"

#include "../utils/layeredperlinnoise.h"
#include "noisedpatch.h"

NoisedPatchFactory::NoisedPatchFactory(int seed)
{
    noise = new LayeredPerlinNoise(seed);
}

Patch* NoisedPatchFactory::getPatch(QMatrix4x4 relativeRotation, float scale, QVector2D relativePosition){
    return new NoisedPatch(relativeRotation, scale, relativePosition, noise);
}
