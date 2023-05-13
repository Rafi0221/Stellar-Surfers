#ifndef QUADTREENODE_H
#define QUADTREENODE_H

#include <QVector2D>
#include <QMatrix4x4>

class Patch;
class PatchFactory;
class TerrainFace;

class QuadTreeNode
{
private:
    TerrainFace *face;
    Patch *patch;

    PatchFactory *factory;

    QuadTreeNode *parent;
    QuadTreeNode *children[4];
    QuadTreeNode *neighbors[4];

    int depth;
    float scale;

    QVector2D relativePosition;

    QMatrix4x4 relativeRotation;

    bool isLeaf();
    void merge();
    void split();

    int counter = 0;
public:
    QuadTreeNode(TerrainFace *face, PatchFactory *factory, QMatrix4x4 relativeRotation, QuadTreeNode *parent = nullptr, QVector2D relativePosition = QVector2D(-1,-1));
    ~QuadTreeNode();

    void update(QVector3D cameraPosition, QMatrix4x4 modelMatrix);
    void render();

    friend class TerrainFace;
};

#endif // QUADTREENODE_H
