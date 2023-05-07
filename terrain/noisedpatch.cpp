#include "noisedpatch.h"

#include "../opengl/gl.h"
#include "../utils/layeredperlinnoise.h"
#include "../utils/shader.h"
#include "../utils/shadermanager.h"

#include "patchindices.h"

#include <QOpenGLFunctions_3_3_Core>

namespace {
    unsigned int toID(int x, int y){
        return x * PATCH_VERTS + y;
    }
}

NoisedPatch::NoisedPatch(QMatrix4x4 relativeRotation, float scale, QVector2D relativePosition, LayeredPerlinNoise *noise)
{
    this->relativeRotation = relativeRotation;
    this->scale = scale;
    this->relativePosition = relativePosition;
    this->noise = noise;
    for(int x = 0; x < PATCH_VERTS; x++){
        for(int y = 0; y < PATCH_VERTS; y++){
            float xPos = relativePosition.x() + scale / PATCH_QUADS * x;
            float yPos = relativePosition.y() + scale / PATCH_QUADS * y;
            float zPos = 1.0;

            QVector3D pos(xPos, yPos, zPos);
            pos = this->relativeRotation.map(pos);

            QVector3D cubePos = cubeToSphere(pos, 3);
            QVector3D newPos = cubePos * terrainHeight(cubePos);

            this->vertices[toID(x,y) * DATA_SIZE] = newPos.x();
            this->vertices[toID(x,y) * DATA_SIZE + 1] = newPos.y();
            this->vertices[toID(x,y) * DATA_SIZE + 2] = newPos.z();

//            this->vertices[(toID(x,y) * DATA_SIZE + 3)] = (float)x/(PATCH_QUADS);
//            this->vertices[(toID(x,y) * DATA_SIZE + 4)] = (float)y/(PATCH_QUADS);
            this->vertices[toID(x,y) * DATA_SIZE + 3] = cubePos.x();
            this->vertices[toID(x,y) * DATA_SIZE + 4] = cubePos.y();
            this->vertices[toID(x,y) * DATA_SIZE + 5] = cubePos.z();

//            this->vertices[toID(x,y) * DATA_SIZE + 6] = 0.1 + 2 * (terrainHeight(pos) - 0.8);
//            this->vertices[toID(x,y) * DATA_SIZE + 7] = 0.2;
//            this->vertices[toID(x,y) * DATA_SIZE + 8] = 0.9 - 2 * (terrainHeight(pos) - 0.8);
//            qDebug() << this->vertices[ToID(x,y) * 6 + 5];

            this->vertices[toID(x,y) * DATA_SIZE + 6] = 0.8 - (terrainHeight(cubePos) - 0.8) * 2;
            this->vertices[toID(x,y) * DATA_SIZE + 7] = 0.8 - (terrainHeight(cubePos) - 0.8) * 2;
            this->vertices[toID(x,y) * DATA_SIZE + 8] = 0.8 - (terrainHeight(cubePos) - 0.8) * 2;
        }
    }

    this->indices = PatchIndices::getIndices(0,0,0,0);

    GL::funcs.glGenVertexArrays(1, &VAO);
    GL::funcs.glGenBuffers(1, &VBO);
    EBO = indices->getEBO();
//    GL::funcs.glGenBuffers(1, &EBO);

    GL::funcs.glBindVertexArray(VAO);

    GL::funcs.glBindBuffer(GL_ARRAY_BUFFER, VBO);
    GL::funcs.glBufferData(GL_ARRAY_BUFFER, sizeof(this->vertices), this->vertices, GL_STATIC_DRAW);

    GL::funcs.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//    GL::funcs.glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(this->indices), this->indices, GL_STATIC_DRAW);

    GL::funcs.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, DATA_SIZE * sizeof(float), (void*)0);
    GL::funcs.glEnableVertexAttribArray(0);
    GL::funcs.glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, DATA_SIZE * sizeof(float), (void*)(3* sizeof(float)));
    GL::funcs.glEnableVertexAttribArray(1);
    GL::funcs.glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, DATA_SIZE * sizeof(float), (void*)(6* sizeof(float)));
    GL::funcs.glEnableVertexAttribArray(2);

    GL::funcs.glBindBuffer(GL_ARRAY_BUFFER, 0);

    GL::funcs.glBindVertexArray(0);

    calculateBoundingSphere();
}

float NoisedPatch::terrainHeight(QVector3D position){
    float tmp = noise->getValue(position.x(), position.y(), position.z());
    tmp = (tmp * 2.0) - 1.0;
    return (1 + tmp * 0.2);
}

void NoisedPatch::calculateBoundingSphere(){
    center = QVector3D(0,0,0);
    for(int i = 0; i < PATCH_VERTS * PATCH_VERTS; i++){
        center += QVector3D(vertices[i*DATA_SIZE], vertices[i*DATA_SIZE+1], vertices[i*DATA_SIZE+2]);
    }
    center /= (float)(PATCH_VERTS * PATCH_VERTS);

    radius = 0;
    for(int i = 0; i < PATCH_VERTS * PATCH_VERTS; i++){
        radius = std::max(radius, center.distanceToPoint(QVector3D(vertices[i*DATA_SIZE], vertices[i*DATA_SIZE+1], vertices[i*DATA_SIZE+2])));
    }
}

void NoisedPatch::render(){
    GL::funcs.glBindVertexArray(VAO);
//    GL::funcs.glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    GL::funcs.glDrawElements(GL_TRIANGLES, indices->getSize(), GL_UNSIGNED_INT, 0);
    GL::funcs.glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
