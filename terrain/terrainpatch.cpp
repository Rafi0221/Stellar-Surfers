#include "terrainpatch.h"

#include "terraintype.h"
#include "../opengl/gl.h"
#include "../utils/shader.h"

#include <QOpenGLFunctions_3_3_Core>

unsigned int ToID(int x, int y){
    return x * PATCH_SIZE + y;
}

TerrainPatch::TerrainPatch(TerrainType *type, QMatrix4x4 relativeRotation, float scale, QVector2D relativePosition)
{
    this->type = type;
    this->relativeRotation = relativeRotation;
    this->scale = scale;
    this->relativePosition = relativePosition;
    for(int x = 0; x < PATCH_SIZE; x++){
        for(int y = 0; y < PATCH_SIZE; y++){
            float xPos = relativePosition.x() + scale / (PATCH_SIZE - 1) * x;
            float yPos = relativePosition.y() + scale / (PATCH_SIZE - 1) * y;
            float zPos = 1.0;

            QVector3D pos(xPos, yPos, zPos);
            pos = this->relativeRotation.map(pos);
            pos = type->CubeToSphere(pos) * type->TerrainHeight(pos);

            this->vertices[ToID(x,y) * 6] = pos.x();
            this->vertices[ToID(x,y) * 6 + 1] = pos.y();
            this->vertices[ToID(x,y) * 6 + 2] = pos.z();

            this->vertices[ToID(x,y) * 6 + 3] = 0.1 + 2 * (type->TerrainHeight(pos) - 0.8);
            this->vertices[ToID(x,y) * 6 + 4] = 0.2;
            this->vertices[ToID(x,y) * 6 + 5] = 0.9 - 2 * (type->TerrainHeight(pos) - 0.8);
//            qDebug() << this->vertices[ToID(x,y) * 6 + 5];
        }
    }

    int tmp = 0;
    for(int x = 0; x < PATCH_SIZE - 1; x++){
        for(int y = 0; y < PATCH_SIZE - 1; y++){
            this->indices[tmp++] = ToID(x,y);
            this->indices[tmp++] = ToID(x,y+1);
            this->indices[tmp++] = ToID(x+1,y+1);

            this->indices[tmp++] = ToID(x,y);
            this->indices[tmp++] = ToID(x+1,y+1);
            this->indices[tmp++] = ToID(x+1,y);
        }
    }

    GL::funcs.glGenVertexArrays(1, &VAO);
    GL::funcs.glGenBuffers(1, &VBO);
    GL::funcs.glGenBuffers(1, &EBO);

    GL::funcs.glBindVertexArray(VAO);

    GL::funcs.glBindBuffer(GL_ARRAY_BUFFER, VBO);
    GL::funcs.glBufferData(GL_ARRAY_BUFFER, sizeof(this->vertices), this->vertices, GL_STATIC_DRAW);

    GL::funcs.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    GL::funcs.glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(this->indices), this->indices, GL_STATIC_DRAW);

    GL::funcs.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    GL::funcs.glEnableVertexAttribArray(0);
    GL::funcs.glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    GL::funcs.glEnableVertexAttribArray(1);

    GL::funcs.glBindBuffer(GL_ARRAY_BUFFER, 0);

    GL::funcs.glBindVertexArray(0);

    CalculateBoundingSphere();
}


void TerrainPatch::Render(){
    GL::terrainShader->use();
    GL::funcs.glBindVertexArray(VAO);
//    GL::funcs.glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//    GL::funcs.glDrawElements(GL_TRIANGLES, (PATCH_SIZE - 1) * (PATCH_SIZE - 1) * 6, GL_UNSIGNED_INT, 0);
    GL::funcs.glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    GL::funcs.glDrawElements(GL_TRIANGLES, (PATCH_SIZE - 1) * (PATCH_SIZE - 1) * 6, GL_UNSIGNED_INT, 0);
    GL::funcs.glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

QVector3D TerrainPatch::GetCenter(){
    return center;
}

float TerrainPatch::getRadius(){
    return radius;
}

void TerrainPatch::CalculateBoundingSphere(){
    center = QVector3D(0,0,0);
    for(int i = 0; i < PATCH_SIZE * PATCH_SIZE; i++){
        center += QVector3D(vertices[i*3], vertices[i*3+1], vertices[i*3+2]);
    }
    center /= (float)(PATCH_SIZE * PATCH_SIZE);

    radius = 0;
    for(int i = 0; i < PATCH_SIZE * PATCH_SIZE; i++){
        radius = std::max(radius, center.distanceToPoint(QVector3D(vertices[i*3], vertices[i*3+1], vertices[i*3+2])));
    }
}
