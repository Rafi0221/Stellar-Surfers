#include "quadtreenode.h"

#include "terrainpatch.h"

#include <QVector3D>
#include <QMatrix4x4>

QuadTreeNode::QuadTreeNode(TerrainFace *face, TerrainType *type, QMatrix4x4 relativeRotation, QuadTreeNode *parent, QVector2D relativePosition)
{
    this->face = face;
    this->type = type;
    this->relativeRotation = relativeRotation;
    this->parent = parent;
    this->relativePosition = relativePosition;

    if(parent != nullptr){
        this->depth = parent->depth + 1;
        this->scale = parent->scale / 2.0;
    }else{
        this->depth = 1;
        this->scale = 2;
    }

    for(int i = 0; i < 4; i++){
        this->children[i] = nullptr;
    }

    this->patch = new TerrainPatch(type, this->relativeRotation, this->scale, this->relativePosition);
}

QuadTreeNode::~QuadTreeNode(){
    for(int i = 0; i < 4; i++){
        delete children[i];
    }
//    delete patch;
}

bool QuadTreeNode::IsLeaf(){
    return (children[0] == nullptr);
}

void QuadTreeNode::Merge(){
    if(IsLeaf())
        return;
    for(int i = 0; i < 4; i++){
        delete children[i];
        children[i] = nullptr;
    }
}

void QuadTreeNode::Split(){
    if(!IsLeaf())
        return;
    children[NORTH_EAST] = new QuadTreeNode(face, this->type, this->relativeRotation, this, relativePosition);
    children[NORTH_WEST] = new QuadTreeNode(face, this->type, this->relativeRotation, this, relativePosition + QVector2D(scale / 2.0 ,0));
    children[SOUTH_WEST] = new QuadTreeNode(face, this->type, this->relativeRotation, this, relativePosition + QVector2D(scale / 2.0, scale / 2.0));
    children[SOUTH_EAST] = new QuadTreeNode(face, this->type, this->relativeRotation, this, relativePosition + QVector2D(0, scale / 2.0));
}

void QuadTreeNode::Update(QVector3D cameraPosition, QMatrix4x4 modelMatrix){
    QVector3D center = patch->GetCenter();
    float distance = modelMatrix.map(center).distanceToPoint(cameraPosition);

    if(distance <= (patch->getRadius() * 1.5) && depth <= 5)
        Split();
    else
        Merge();

    if(!IsLeaf()){
        for(int i = 0; i < 4; i++){
            children[i]->Update(cameraPosition, modelMatrix);
        }
    }
}

void QuadTreeNode::Render(){
    if(IsLeaf()){
        patch->Render();
    }else{
        for(int i = 0; i < 4; i++){
            children[i]->Render();
        }
    }
}
