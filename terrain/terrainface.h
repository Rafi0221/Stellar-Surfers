#ifndef TERRAINFACE_H
#define TERRAINFACE_H

#include <QVector3D>
#include <QMatrix4x4>

class QuadTreeNode;
class TerrainType;

class TerrainFace
{
private:
    QuadTreeNode *root;
    TerrainType *type;

    QMatrix4x4 relativeRotation;
public:
    TerrainFace(TerrainType *type, QMatrix4x4 relativeRotation);
    ~TerrainFace();

    void Update(QVector3D cameraPosition, QMatrix4x4 modelMatrix);
    void Render();
};

#endif // TERRAINFACE_H
