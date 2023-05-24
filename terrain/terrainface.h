#ifndef TERRAINFACE_H
#define TERRAINFACE_H

#include <QVector3D>
#include <QMatrix4x4>

class PatchFactory;
class QuadTreeNode;
class PlanetProperties;

class TerrainFace
{
private:
    QuadTreeNode *root;
    PatchFactory *factory;
    PlanetProperties *properties;
    QMatrix4x4 relativeRotation;
public:
    TerrainFace(PatchFactory *factory, QMatrix4x4 relativeRotation, PlanetProperties *properties);
    ~TerrainFace();

    void update(QVector3D cameraPosition, QMatrix4x4 modelMatrix);
    void render(QMatrix4x4 model);
    bool checkCollision(const QVector3D & relativePosition);
    void addNeighbor(TerrainFace* neighbor, int dir);
};

#endif // TERRAINFACE_H
