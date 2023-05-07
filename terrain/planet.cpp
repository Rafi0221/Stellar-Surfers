#include "planet.h"

#include <QVector3D>
#include <QMatrix4x4>

#include "noisedterrain.h"
#include "terraintype.h"
#include "terrainface.h"
#include "../utils/consts.h"
#include "../utils/shader.h"
#include "../utils/shadermanager.h"

Planet::Planet(float radius, int seed)
{
    this->radius = radius;
    this->seed = seed;

    this->type = new NoisedTerrain(radius);
    this->faces[2] = new TerrainFace(type, QMatrix4x4());
    QMatrix4x4 tmp;
    tmp.rotate(90, 0, 1, 0);
    this->faces[LEFT] = new TerrainFace(type, tmp);
    tmp.rotate(90, 0, 1, 0);
    this->faces[0] = new TerrainFace(type, tmp);
    tmp.rotate(90, 0, 1, 0);
    this->faces[RIGHT] = new TerrainFace(type, tmp);
    tmp = QMatrix4x4();
    tmp.rotate(90, 1, 0, 0);
    this->faces[DOWN] = new TerrainFace(type, tmp);
    tmp.rotate(180, 1, 0, 0);
    this->faces[UP] = new TerrainFace(type, tmp);
}

void Planet::update(QVector3D cameraPosition){
    QMatrix4x4 modelMatrix;
    modelMatrix.translate(position);
    modelMatrix.rotate(this->rotation.x(), 1, 0, 0);
    modelMatrix.rotate(this->rotation.y(), 0, 1, 0);
    modelMatrix.rotate(this->rotation.z(), 0, 0, 1);
    for(int i = 0; i < 6; i++){
        faces[i]->update(cameraPosition, modelMatrix);
    }
}

void Planet::render(){
    Shader *terrainShader = ShaderManager::getShader("terrainShader");
    QMatrix4x4 model;
    model.translate(this->position);
    model.rotate(this->rotation.x(), 1, 0, 0);
    model.rotate(this->rotation.y(), 0, 1, 0);
    model.rotate(this->rotation.z(), 0, 0, 1);
    terrainShader->setMat4("model", model);
    for(int i = 0; i < 6; i++){
        faces[i]->render();
    }
}

bool Planet::checkCollision(QVector3D cameraPosition) {
    QVector3D relativePosition  = cameraPosition - position;
    for(int i = 0; i < 6; i++){
        if(faces[i]->chechCollision(relativePosition))
            return true;
    }
    return false;
}

void Planet::setPosition(QVector3D position){
    this->position = position;
}

QVector3D Planet::getPosition() {
    return position;
}

void Planet::setRotation(QVector3D rotation){
    this->rotation = rotation;
}

float Planet::getRadius() {
    return radius;
}
