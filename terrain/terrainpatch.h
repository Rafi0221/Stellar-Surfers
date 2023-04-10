#ifndef TERRAINPATCH_H
#define TERRAINPATCH_H

#include <QVector2D>
#include <QVector3D>
#include <QMatrix4x4>

#include <QOpenGLFunctions>

#define PATCH_SIZE ((1 << 4) + 1)

class TerrainType;

class TerrainPatch
{
private:
    float scale;
    QVector2D relativePosition;
    QMatrix4x4 relativeRotation;

    TerrainType *type;

    QVector3D center;
    float radius;

    unsigned int VAO, VBO, EBO;
    float vertices[PATCH_SIZE * PATCH_SIZE * 6];
    unsigned int indices[(PATCH_SIZE - 1) * (PATCH_SIZE - 1) * 6];

    void calculateBoundingSphere();
public:
    TerrainPatch(TerrainType *type, QMatrix4x4 relativeRotation, float scale = 2.0, QVector2D relativePosition = QVector2D(-1,-1));
    ~TerrainPatch();

    QVector3D getCenter();
    float getRadius();

    void render();
};

#endif // TERRAINPATCH_H
