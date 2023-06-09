#include "quadtreenode.h"

#include <QVector3D>
#include <QMatrix4x4>

#include "../utils/consts.h"
#include "../utils/frustum.h"
#include "planetproperties.h"
#include "patchfactory.h"
#include "patch.h"
#include <cassert>

QuadTreeNode::QuadTreeNode(TerrainFace *face, PatchFactory *factory, QMatrix4x4 relativeRotation, PlanetProperties *properties, QuadTreeNode *parent, QVector2D relativePosition)
{
    this->face = face;
    this->factory = factory;
    this->relativeRotation = relativeRotation;
    this->properties = properties;
    this->parent = parent;
    this->relativePosition = relativePosition;

    for (int i = 0; i < 4; i++){
        this->neighbors[i] = nullptr;
    }

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
    this->patch = factory->getPatch(this->relativeRotation, this->scale, this->relativePosition, this->properties);
//    this->patch = new TerrainPatch(type, this->relativeRotation, this->scale, this->relativePosition);
}

QuadTreeNode::~QuadTreeNode(){
    for(int i = 0; i < 4; i++){
        delete children[i];
    }
    delete patch;
}

bool QuadTreeNode::isLeaf(){
    return (children[0] == nullptr);
}

void QuadTreeNode::merge(){
    if(isLeaf())
        return;
    for (int dir = 0; dir < 4; dir++)
        if (!(neighbors[dir]->isLeaf())) {
            QuadTreeNode* nb = neighbors[dir];
            int their_dir = -1;
            for (int d = 0; d < 4; d++)
                if (nb->neighbors[d] == this)
                    their_dir = d;
            assert(their_dir >= 0);
            nb->children[combine_dirs(their_dir, 0)]->
                announceDeletedNeighbor(this, their_dir);
            nb->children[combine_dirs(their_dir, 1)]->
                announceDeletedNeighbor(this, their_dir);
        }
    for(int i = 0; i < 4; i++){
        delete children[i];
        children[i] = nullptr;
    }
}

void QuadTreeNode::split(){
    if(!isLeaf())
        return;
    children[NORTH_WEST] = new QuadTreeNode(face, this->factory, this->relativeRotation, this->properties, this, relativePosition);
    children[NORTH_EAST] = new QuadTreeNode(face, this->factory, this->relativeRotation, this->properties, this, relativePosition + QVector2D(scale / 2.0 ,0));
    children[SOUTH_EAST] = new QuadTreeNode(face, this->factory, this->relativeRotation, this->properties, this, relativePosition + QVector2D(scale / 2.0, scale / 2.0));
    children[SOUTH_WEST] = new QuadTreeNode(face, this->factory, this->relativeRotation, this->properties, this, relativePosition + QVector2D(0, scale / 2.0));
    for (int dir = 0; dir < 4; dir++) {
        int d0 = combine_dirs(dir, 1);
        int d1 = opposite_dir(d0);
        QuadTreeNode* ch0 = children[combine_dirs(dir, 0)];
        QuadTreeNode* ch1 = children[combine_dirs(dir, 1)];
        ch0->neighbors[d0]=ch1;
        ch1->neighbors[d1]=ch0;
    }
    for (int dir = 0; dir < 4; dir++){
        QuadTreeNode* nb = this->neighbors[dir];
        QuadTreeNode* ch0 = this->children[combine_dirs(dir, 0)];
        QuadTreeNode* ch1 = this->children[combine_dirs(dir, 1)];
        if (!(nb->isLeaf())) {
            int their_dir = -1;
            for (int d = 0; d < 4; d++)
                if (nb->neighbors[d]==this)
                    their_dir = d;
            assert(their_dir >= 0);
            QuadTreeNode* nb0 = nb->children[combine_dirs(their_dir, 0)];
            QuadTreeNode* nb1 = nb->children[combine_dirs(their_dir, 1)];
            ch0->neighbors[dir] = nb1;
            ch1->neighbors[dir] = nb0;
            nb0->announceNewNeighbor(ch1, their_dir);
            nb1->announceNewNeighbor(ch0, their_dir);
        } else {
            ch0->neighbors[dir]=nb;
            ch1->neighbors[dir]=nb;
        }
    }

    for (int c = 0; c < 4; c++) for (int d = 0; d < 4; d++)
        assert(children[c]->neighbors[d] != nullptr);
}

void QuadTreeNode::announceNewNeighbor(QuadTreeNode* added, int dir) {
    neighbors[dir] = added;
    if (!isLeaf()) {
        children[combine_dirs(dir, 0)]->announceNewNeighbor(added, dir);
        children[combine_dirs(dir, 1)]->announceNewNeighbor(added, dir);
    }
}

/* this == toRemove->neighbor[dir_their]
 * toRemove == neighbor[dir_mine]
 * not necessarily dir_their == opposite_dir(dir_mine)! */
void QuadTreeNode::announceDeletedNeighbor(QuadTreeNode* parent, int dir) {
    neighbors[dir] = parent;
    if (!isLeaf()) {
        children[combine_dirs(dir, 0)]->announceDeletedNeighbor(parent, dir);
        children[combine_dirs(dir, 1)]->announceDeletedNeighbor(parent, dir);
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
    if(distance <= (patch->getRadius() * 6) && depth <= 6)
        split();
    else
        merge();

    if(!isLeaf()){
        for(int i = 0; i < 4; i++){
            children[i]->update(cameraPosition, modelMatrix);
        }
    }
}

void QuadTreeNode::render(QMatrix4x4 model, Frustum *frustum){
    if(isLeaf()){
//        if(!frustum->isSphereInside(model.map(patch->getCenter()), patch->getRadius()))
//            return;
//        if(frustum->isSphereInside(model.map(patch->getCenter()), patch->getRadius()))
//            GL::drawCount++;
        int dNorth = std::min(MAX_LOD_DIFFERENCE, std::max(0, depth - (neighbors[NORTH]->depth)));
        int dEast = std::min(MAX_LOD_DIFFERENCE, std::max(0, depth - (neighbors[EAST]->depth)));
        int dSouth = std::min(MAX_LOD_DIFFERENCE, std::max(0, depth - (neighbors[SOUTH]->depth)));
        int dWest = std::min(MAX_LOD_DIFFERENCE, std::max(0, depth - (neighbors[WEST]->depth)));
        patch->render(model, dNorth, dEast, dSouth, dWest);
    }else{
        for(int i = 0; i < 4; i++){
            children[i]->render(model, frustum);
        }
    }
}

bool QuadTreeNode::checkCollision(const QVector3D & relativePosition) {
    if(isLeaf()) {
//        QVector3D planetCenter = QVector3D(0,0,0);
//        float distPosCenter = planetCenter.distanceToPoint(relativePosition);
//        float distPosPatch = relativePosition.distanceToPoint(patch->getCenter());
//        float distPatchCenter = planetCenter.distanceToPoint(patch->getCenter());
//        return distPosPatch < distPatchCenter && distPosCenter < distPatchCenter;
        return patch->checkCollision(relativePosition);
    }

    int closestChildIndex = -1;
    float smallestDist = std::numeric_limits<float>::max();
    for(int i = 0; i < 4; i++){
        float distance = relativePosition.distanceToPoint(children[i]->patch->getCenter());
        if(distance < smallestDist) {
            smallestDist = distance;
            closestChildIndex = i;
        }
    }
    return children[closestChildIndex]->checkCollision(relativePosition);
}
