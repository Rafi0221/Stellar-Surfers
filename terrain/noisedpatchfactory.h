#ifndef NOISEDPATCHFACTORY_H
#define NOISEDPATCHFACTORY_H

#include "patchfactory.h"

#include <QVector2D>
#include <QMatrix4x4>

class LayeredPerlinNoise;

class NoisedPatchFactory : public PatchFactory
{
private:
    int seed;
    LayeredPerlinNoise *noise;
public:
    NoisedPatchFactory(int seed);

    virtual Patch* getPatch(QMatrix4x4 relativeRotation, float scale, QVector2D relativePosition) override;
};

#endif // NOISEDPATCHFACTORY_H
