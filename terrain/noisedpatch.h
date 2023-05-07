#ifndef NOISEDPATCH_H
#define NOISEDPATCH_H

#include "patch.h"

#include "../utils/consts.h"

#define DATA_SIZE (9)

class LayeredPerlinNoise;

class NoisedPatch : public Patch
{
private:
    unsigned int VAO, VBO, EBO;
    float vertices[PATCH_VERTS * PATCH_VERTS * DATA_SIZE];
    float terrainHeight(QVector3D position);
    void calculateBoundingSphere();

    LayeredPerlinNoise *noise;
public:
    NoisedPatch(QMatrix4x4 relativeRotation, float scale, QVector2D relativePosition, LayeredPerlinNoise *noise);

    virtual void render();
};

#endif // NOISEDPATCH_H