#ifndef NOISEDPATCH_H
#define NOISEDPATCH_H

#include "patch.h"

#include "../utils/consts.h"

#define DATA_SIZE (5)

class LayeredPerlinNoise;

class NoisedPatch : public Patch
{
private:
    LayeredPerlinNoise *noise;
    float radius;

    unsigned int VAO, VBO, EBO;
    float vertices[PATCH_VERTS * PATCH_VERTS * DATA_SIZE];
    float terrainHeight(QVector3D position);
    void calculateBoundingSphere();

    unsigned int normalMapTexture;
    void generateNormalMap();

public:
    NoisedPatch(QMatrix4x4 relativeRotation, float scale, QVector2D relativePosition, float radius, LayeredPerlinNoise *noise);

    virtual void render(QMatrix4x4 model, int dNorth, int dEast, int dSouth, int dWest);
};

#endif // NOISEDPATCH_H
