#ifndef WATERPATCH_H
#define WATERPATCH_H

#include "patch.h"

#include "../utils/consts.h"

#define DATA_SIZE (5)

class LayeredPerlinNoise;

class WaterPatch : public Patch
{
public:
    WaterPatch(QMatrix4x4 relativeRotation, float scale, QVector2D relativePosition, float radius, LayeredPerlinNoise *noise, unsigned int wavesBuffer, float oceanLevel);
    ~WaterPatch();
    virtual void render(QMatrix4x4 model, int dNorth, int dEast, int dSouth, int dWest);
    virtual bool checkCollision(const QVector3D & relativePosition);
private:
    LayeredPerlinNoise *noise;
    float radius;
    unsigned int wavesBuffer;
    float oceanLevel;
    unsigned int VAO, VBO, EBO;
    float vertices[PATCH_VERTS * PATCH_VERTS * DATA_SIZE];

    void calculateBoundingSphere();
    unsigned int normalMapTexture;
    void generateNormalMap();
};

#endif // WATERPATCH_H
