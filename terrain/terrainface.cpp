#include "terrainface.h"

#include "patchfactory.h"
#include "quadtreenode.h"

TerrainFace::TerrainFace(PatchFactory *factory, QMatrix4x4 relativeRotation)
{
    this->factory = factory;
    this->relativeRotation = relativeRotation;
    this->root = new QuadTreeNode(this, factory, relativeRotation);
}

TerrainFace::~TerrainFace(){
    delete root;
}

void TerrainFace::update(QVector3D cameraPosition, QMatrix4x4 modelMatrix){
    root->update(cameraPosition, modelMatrix);
}

void TerrainFace::render(){
    root->render();
}

bool TerrainFace::chechCollision(const QVector3D & relativePosition) {
    return root->chechCollision(relativePosition);
}
