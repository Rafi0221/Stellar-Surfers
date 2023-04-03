#include "planet.h"

#include <QVector3D>
#include <QMatrix4x4>

#include "terraintype.h"
#include "terrainface.h"

Planet::Planet(TerrainType *type)
{
    this->type = type;
    this->faces[FAR] = new TerrainFace(type, QMatrix4x4());
    QMatrix4x4 tmp;
    tmp.rotate(90, 0, 1, 0);
    this->faces[LEFT] = new TerrainFace(type, tmp);
    tmp.rotate(90, 0, 1, 0);
    this->faces[NEAR] = new TerrainFace(type, tmp);
    tmp.rotate(90, 0, 1, 0);
    this->faces[RIGHT] = new TerrainFace(type, tmp);
    tmp = QMatrix4x4();
    tmp.rotate(90, 1, 0, 0);
    this->faces[DOWN] = new TerrainFace(type, tmp);
    tmp.rotate(180, 1, 0, 0);
    this->faces[UP] = new TerrainFace(type, tmp);
}

void Planet::Update(QVector3D cameraPosition){
    QMatrix4x4 modelMatrix;
    modelMatrix.translate(position);
    for(int i = 0; i < 6; i++){
        faces[i]->Update(cameraPosition, modelMatrix);
    }
}

void Planet::Render(){
    for(int i = 0; i < 6; i++){
        faces[i]->Render();
    }
}

void Planet::SetPosition(QVector3D position){
    this->position = position;
}
