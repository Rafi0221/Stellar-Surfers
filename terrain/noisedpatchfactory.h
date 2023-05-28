#ifndef NOISEDPATCHFACTORY_H
#define NOISEDPATCHFACTORY_H

#include "patchfactory.h"

#include <QVector2D>
#include <QMatrix4x4>

class LayeredPerlinNoise;
class PlanetProperties;

class NoisedPatchFactory : public PatchFactory
{
private:
    int seed;
    LayeredPerlinNoise *noise;
public:
    NoisedPatchFactory(int seed);
    ~NoisedPatchFactory();
    virtual Patch* getPatch(QMatrix4x4 relativeRotation, float scale, QVector2D relativePosition, PlanetProperties *properties) override;
};

#endif // NOISEDPATCHFACTORY_H
