#ifndef PATCHFACTORY_H
#define PATCHFACTORY_H

#include <QVector2D>
#include <QMatrix4x4>

#include "patch.h"
#include "noisedpatch.h"

class Patch;

class PatchFactory
{
public:
    PatchFactory();

    virtual Patch* getPatch(QMatrix4x4 relativeRotation, float scale, QVector2D relativePosition) = 0;
};

#endif // PATCHFACTORY_H
