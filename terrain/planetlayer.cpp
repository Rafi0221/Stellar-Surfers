#include "planetlayer.h"

#include <QVector3D>
#include <QMatrix4x4>

#include "patchfactory.h"
#include "terrainface.h"
#include "../utils/consts.h"
#include "../utils/shader.h"
#include "../utils/shadermanager.h"

PlanetLayer::PlanetLayer(PatchFactory *factory, PlanetProperties *properties)
{
    this->factory = factory;
    this->properties = properties;
    this->faces[FAR_] = new TerrainFace(this->factory, QMatrix4x4(), properties);
    QMatrix4x4 tmp;
    tmp.rotate(90, 0, 1, 0);
    this->faces[LEFT] = new TerrainFace(this->factory, tmp, properties);
    tmp.rotate(90, 0, 1, 0);
    this->faces[NEAR_] = new TerrainFace(this->factory, tmp, properties);
    tmp.rotate(90, 0, 1, 0);
    this->faces[RIGHT] = new TerrainFace(this->factory, tmp, properties);
    tmp = QMatrix4x4();
    tmp.rotate(90, 1, 0, 0);
    this->faces[DOWN] = new TerrainFace(this->factory, tmp, properties);
    tmp.rotate(180, 1, 0, 0);
    this->faces[UP] = new TerrainFace(this->factory, tmp, properties);
    int neighbors[6][4] = {
        {DOWN, RIGHT, UP, LEFT},  // NEAR_
        {DOWN, NEAR_, UP, FAR_},  // LEFT
        {DOWN, LEFT, UP, RIGHT},  // FAR_
        {DOWN, FAR_, UP, NEAR_},  // RIGHT
        {FAR_, LEFT, NEAR_, RIGHT},  // UP
        {NEAR_, LEFT, FAR_, RIGHT},  // DOWN
    };
    for (int dir = 0; dir < 4; dir++) {
        for (int i = 0; i < 6; i++)
            this->faces[i]->addNeighbor(this->faces[neighbors[i][dir]], dir);
    }
}

void PlanetLayer::update(QVector3D cameraPosition){
    QMatrix4x4 modelMatrix;
    modelMatrix.translate(position);
    modelMatrix.rotate(this->rotation.x(), 1, 0, 0);
    modelMatrix.rotate(this->rotation.y(), 0, 1, 0);
    modelMatrix.rotate(this->rotation.z(), 0, 0, 1);
    for(int i = 0; i < 6; i++){
        faces[i]->update(cameraPosition, modelMatrix);
    }
}

void PlanetLayer::render(){
//    Shader *terrainShader = ShaderManager::getShader("terrainShader");
    QMatrix4x4 model;
    model.translate(this->position);
    model.rotate(this->rotation.x(), 1, 0, 0);
    model.rotate(this->rotation.y(), 0, 1, 0);
    model.rotate(this->rotation.z(), 0, 0, 1);
//    terrainShader->setMat4("model", model);
    for(int i = 0; i < 6; i++){
        faces[i]->render(model);
    }
}

bool PlanetLayer::checkCollision(QVector3D cameraPosition) {
    QVector3D relativePosition  = cameraPosition - position;
    for(int i = 0; i < 6; i++){
        if(faces[i]->checkCollision(relativePosition))
            return true;
    }
    return false;
}

void PlanetLayer::setPosition(QVector3D position){
    this->position = position;
}

QVector3D PlanetLayer::getPosition() {
    return position;
}

void PlanetLayer::setRotation(QVector3D rotation){
    this->rotation = rotation;
}

float PlanetLayer::getRadius() {
    return radius;
}
