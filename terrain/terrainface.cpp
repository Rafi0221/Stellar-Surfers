#include "terrainface.h"

#include "patchfactory.h"
#include "planetproperties.h"
#include "quadtreenode.h"

TerrainFace::TerrainFace(PatchFactory *factory, QMatrix4x4 relativeRotation, PlanetProperties *properties)
{
    this->factory = factory;
    this->relativeRotation = relativeRotation;
    this->properties = properties;
    this->root = new QuadTreeNode(this, factory, relativeRotation, properties);
}

TerrainFace::~TerrainFace(){
    delete root;
}

void TerrainFace::update(QVector3D cameraPosition, QMatrix4x4 modelMatrix){
    root->update(cameraPosition, modelMatrix);
}

void TerrainFace::render(QMatrix4x4 model){
    root->render(model);
}

/* called only upon construction */
void TerrainFace::addNeighbor(TerrainFace* neighborFace, int dir) {
    this->root->neighbors[dir] = neighborFace->root;
}

bool TerrainFace::checkCollision(const QVector3D & relativePosition) {
    return root->checkCollision(relativePosition);
}
