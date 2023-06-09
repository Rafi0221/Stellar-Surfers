#include "waterpatchfactory.h"

#include "../utils/layeredperlinnoise.h"
#include "planetproperties.h"
#include "waterpatch.h"

WaterPatchFactory::WaterPatchFactory(int seed)
{
    noise = new LayeredPerlinNoise(seed);
}

WaterPatchFactory::~WaterPatchFactory(){
    delete noise;
}

Patch* WaterPatchFactory::getPatch(QMatrix4x4 relativeRotation, float scale, QVector2D relativePosition, PlanetProperties *properties){
    return new WaterPatch(relativeRotation, scale, relativePosition, properties->radius, noise, properties->wavesBuffer, properties->oceanLevel);
}
