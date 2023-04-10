#ifndef QUADTREENODE_H
#define QUADTREENODE_H

#include <QVector2D>
#include <QMatrix4x4>

#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3

#define NORTH_WEST 0
#define NORTH_EAST 1
#define SOUTH_EAST 2
#define SOUTH_WEST 3

class TerrainFace;
class TerrainPatch;
class TerrainType;

class QuadTreeNode
{
private:
    TerrainFace *face;
    TerrainPatch *patch;
    TerrainType *type;

    QuadTreeNode *parent;
    QuadTreeNode *children[4];

    int depth;
    float scale;

    QVector2D relativePosition;

    QMatrix4x4 relativeRotation;

    bool isLeaf();
    void merge();
    void split();

    int counter = 0;
public:
    QuadTreeNode(TerrainFace *face, TerrainType *type, QMatrix4x4 relativeRotation, QuadTreeNode *parent = nullptr, QVector2D relativePosition = QVector2D(-1,-1));
    ~QuadTreeNode();

    void update(QVector3D cameraPosition, QMatrix4x4 modelMatrix);
    void render();

};

#endif // QUADTREENODE_H
