#ifndef TERRAINPATCH_H
#define TERRAINPATCH_H

#include <QVector2D>
#include <QVector3D>
#include <QMatrix4x4>

#include <QOpenGLFunctions>

#include "../utils/consts.h"

#define DATA_SIZE (9)

class TerrainType;
class PatchIndices;

class TerrainPatch
{
private:
    float scale;
    QVector2D relativePosition;
    QMatrix4x4 relativeRotation;

    TerrainType *type;

    PatchIndices *indices;

    QVector3D center;
    float radius;

    unsigned int VAO, VBO, EBO;
    float vertices[PATCH_VERTS * PATCH_VERTS * DATA_SIZE];

    void calculateBoundingSphere();
public:
    TerrainPatch(TerrainType *type, QMatrix4x4 relativeRotation, float scale = 2.0, QVector2D relativePosition = QVector2D(-1,-1));
    ~TerrainPatch();

    QVector3D getCenter();
    float getRadius();

    void render();
};

#endif // TERRAINPATCH_H
