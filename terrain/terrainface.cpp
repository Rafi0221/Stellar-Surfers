#include "terrainface.h"

#include "quadtreenode.h"
#include "terraintype.h"

TerrainFace::TerrainFace(TerrainType *type, QMatrix4x4 relativeRotation)
{
    this->type = type;
    this->relativeRotation = relativeRotation;
    this->root = new QuadTreeNode(this, type, relativeRotation);
}

TerrainFace::~TerrainFace(){
    delete root;
}

void TerrainFace::Update(QVector3D cameraPosition, QMatrix4x4 modelMatrix){
    root->Update(cameraPosition, modelMatrix);
}

void TerrainFace::Render(){
    root->Render();
}
