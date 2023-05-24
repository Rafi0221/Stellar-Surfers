#ifndef PATCH_H
#define PATCH_H

#include <QVector2D>
#include <QVector3D>
#include <QMatrix4x4>

class PatchIndices;

class Patch
{
protected:
    float scale;
    QVector2D relativePosition;
    QMatrix4x4 relativeRotation;

    PatchIndices *indices;

    QVector3D boundingSphereCenter;
    float boundingSphereRadius;

    QVector3D cubeToSphere(QVector3D position, float r);
public:
    QVector3D getCenter();
    float getRadius();

    virtual void render(QMatrix4x4 model, int dNorth, int dEast, int dSouth, int dWest) = 0;
};

#endif // PATCH_H
