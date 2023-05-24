#ifndef WATERPATCH_H
#define WATERPATCH_H

#include "patch.h"

#include "../utils/consts.h"

#define DATA_SIZE (5)

class LayeredPerlinNoise;

class WaterPatch : public Patch
{
public:
    WaterPatch(QMatrix4x4 relativeRotation, float scale, QVector2D relativePosition, float radius, LayeredPerlinNoise *noise, unsigned int wavesBuffer);

    virtual void render(QMatrix4x4 model);
private:
    LayeredPerlinNoise *noise;
    float radius;
    unsigned int wavesBuffer;
    unsigned int VAO, VBO, EBO;
    float vertices[PATCH_VERTS * PATCH_VERTS * DATA_SIZE];

    void calculateBoundingSphere();
    unsigned int normalMapTexture;
    void generateNormalMap();
};

#endif // WATERPATCH_H
