#ifndef WATERPATCHFACTORY_H
#define WATERPATCHFACTORY_H

#include "patchfactory.h"

#include <QVector2D>
#include <QMatrix4x4>

class LayeredPerlinNoise;
class PlanetProperties;

class WaterPatchFactory : public PatchFactory
{
private:
    int seed;
    LayeredPerlinNoise *noise;
public:
    WaterPatchFactory(int seed);

    virtual Patch* getPatch(QMatrix4x4 relativeRotation, float scale, QVector2D relativePosition, PlanetProperties *properties) override;
};

#endif // WATERPATCHFACTORY_H
