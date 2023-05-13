#include "quadtreenode.h"

#include <QVector3D>
#include <QMatrix4x4>

#include "../utils/consts.h"
#include "patchfactory.h"
#include "patch.h"

QuadTreeNode::QuadTreeNode(TerrainFace *face, PatchFactory *factory, QMatrix4x4 relativeRotation, QuadTreeNode *parent, QVector2D relativePosition)
{
    this->face = face;
    this->factory = factory;
    this->relativeRotation = relativeRotation;
    this->parent = parent;
    this->relativePosition = relativePosition;

    if(parent != nullptr){
        this->depth = parent->depth + 1;
        this->scale = parent->scale / 2.0;
        for (int i = 0; i < 4; i++){
            this->neighbors[i] = parent->neighbors[i];
        }
    }else{
        this->depth = 1;
        this->scale = 2;
        for (int i = 0; i < 4; i++){
            this->neighbors[i] = nullptr;
        }
    }

    for(int i = 0; i < 4; i++){
        this->children[i] = nullptr;
    }
    this->patch = factory->getPatch(this->relativeRotation, this->scale, this->relativePosition);
//    this->patch = new TerrainPatch(type, this->relativeRotation, this->scale, this->relativePosition);
}

QuadTreeNode::~QuadTreeNode(){
    for(int i = 0; i < 4; i++){
        delete children[i];
    }
//    delete patch;
}

bool QuadTreeNode::isLeaf(){
    return (children[0] == nullptr);
}

void QuadTreeNode::merge(){
    if(isLeaf())
        return;
    for(int i = 0; i < 4; i++){
        delete children[i];
        children[i] = nullptr;
    }

}

int combine_dirs(int x, int y) {
    if (x + y < 3)
        return x + y - 1;
    return x + y - 2;
}

int opposite_dir(int dir) {
    return 3 - dir;
}

void QuadTreeNode::split(){
    if(!isLeaf())
        return;
    children[NORTH_EAST] = new QuadTreeNode(face, this->factory, this->relativeRotation, this, relativePosition);
    children[NORTH_WEST] = new QuadTreeNode(face, this->factory, this->relativeRotation, this, relativePosition + QVector2D(scale / 2.0 ,0));
    children[SOUTH_WEST] = new QuadTreeNode(face, this->factory, this->relativeRotation, this, relativePosition + QVector2D(scale / 2.0, scale / 2.0));
    children[SOUTH_EAST] = new QuadTreeNode(face, this->factory, this->relativeRotation, this, relativePosition + QVector2D(0, scale / 2.0));
    children[NORTH_EAST]->neighbors[WEST]=children[NORTH_WEST];
    children[NORTH_WEST]->neighbors[EAST]=children[NORTH_EAST];
    children[SOUTH_EAST]->neighbors[WEST]=children[SOUTH_WEST];
    children[SOUTH_WEST]->neighbors[EAST]=children[SOUTH_EAST];
    children[NORTH_EAST]->neighbors[SOUTH]=children[SOUTH_EAST];
    children[NORTH_WEST]->neighbors[SOUTH]=children[SOUTH_WEST];
    children[SOUTH_EAST]->neighbors[NORTH]=children[NORTH_EAST];
    children[SOUTH_WEST]->neighbors[NORTH]=children[NORTH_WEST];
    for (int dir = 0; dir < 4; dir++){
        if (!this->neighbors[dir]->isLeaf()) {
            QuadTreeNode* nn = this->neighbors[dir];
            for (int d2 = 0; d2 < 4; d2++) {
                if (d2 == dir or d2 == opposite_dir(dir)) continue;
                children[combine_dirs(dir, d2)]->neighbors[d2] = nn->children[combine_dirs(dir, opposite_dir(d2))];
            }
        }
    }
}

void QuadTreeNode::update(QVector3D cameraPosition, QMatrix4x4 modelMatrix){
    QVector3D center = patch->getCenter();
    float distance = modelMatrix.map(center).distanceToPoint(cameraPosition);
//    qDebug() << center;
//    qDebug() << center << " " << modelMatrix.map(center) << " " << distance << " " << patch->getRadius();
//    if(isLeaf() && depth <= 2 && distance <= (patch->getRadius() * 1.5)){
//        qDebug() << modelMatrix.map(center) << " " << distance << " " << (patch->getRadius());
//    }else if(isLeaf() && depth <= 2){
//        qDebug() << ":(((    " << modelMatrix.map(center) << " " << distance << " " << (patch->getRadius());
//    }
    if(distance <= (patch->getRadius() * 2) && depth <= 6)
        split();
    else
        merge();

    if(!isLeaf()){
        for(int i = 0; i < 4; i++){
            children[i]->update(cameraPosition, modelMatrix);
        }
    }
}

void QuadTreeNode::render(){
    if(isLeaf()){
        patch->render();
    }else{
        for(int i = 0; i < 4; i++){
            children[i]->render();
        }
    }
}
