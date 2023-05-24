#include "waterpatch.h"

#include "../opengl/gl.h"
#include "../utils/layeredperlinnoise.h"
#include "../utils/perlinnoise.h"
#include "../utils/shader.h"
#include "../utils/shadermanager.h"

#include "patchindices.h"

#include <QOpenGLFunctions_3_3_Core>

namespace {
    unsigned int toID(int x, int y){
        return x * PATCH_VERTS + y;
    }
}

WaterPatch::WaterPatch(QMatrix4x4 relativeRotation, float scale, QVector2D relativePosition, float radius, LayeredPerlinNoise *noise, unsigned int wavesBuffer)
{
    this->relativeRotation = relativeRotation;
    this->scale = scale;
    this->relativePosition = relativePosition;
    this->radius = radius;
    this->noise = noise;
    this->wavesBuffer = wavesBuffer;
    for(int x = 0; x < PATCH_VERTS; x++){
        for(int y = 0; y < PATCH_VERTS; y++){
            float xPos = relativePosition.x() + scale / PATCH_QUADS * x;
            float yPos = relativePosition.y() + scale / PATCH_QUADS * y;
            float zPos = 1.0;

            QVector3D pos(xPos, yPos, zPos);
            pos = this->relativeRotation.map(pos);

            QVector3D spherePos = cubeToSphere(pos, radius);
//            QVector3D newPos = cubePos * terrainHeight(cubePos);

            this->vertices[toID(x,y) * DATA_SIZE] = spherePos.x();
            this->vertices[toID(x,y) * DATA_SIZE + 1] = spherePos.y();
            this->vertices[toID(x,y) * DATA_SIZE + 2] = spherePos.z();

            this->vertices[toID(x,y) * DATA_SIZE + 3] = (float)x / (PATCH_QUADS);
            this->vertices[toID(x,y) * DATA_SIZE + 4] = (float)y / (PATCH_QUADS);
        }
    }

    this->indices = PatchIndices::getIndices(0,0,0,0);

    GL::funcs.glGenVertexArrays(1, &VAO);
    GL::funcs.glGenBuffers(1, &VBO);
    EBO = indices->getEBO();

    GL::funcs.glBindVertexArray(VAO);

    GL::funcs.glBindBuffer(GL_ARRAY_BUFFER, VBO);
    GL::funcs.glBufferData(GL_ARRAY_BUFFER, sizeof(this->vertices), this->vertices, GL_STATIC_DRAW);

    GL::funcs.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    GL::funcs.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, DATA_SIZE * sizeof(float), (void*)0);
    GL::funcs.glEnableVertexAttribArray(0);
    GL::funcs.glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, DATA_SIZE * sizeof(float), (void*)(3* sizeof(float)));
    GL::funcs.glEnableVertexAttribArray(1);

    GL::funcs.glBindBuffer(GL_ARRAY_BUFFER, 0);

    GL::funcs.glBindVertexArray(0);

    calculateBoundingSphere();
    generateNormalMap();
}

void WaterPatch::calculateBoundingSphere(){
    boundingSphereCenter = QVector3D(0,0,0);
    for(int i = 0; i < PATCH_VERTS * PATCH_VERTS; i++){
        boundingSphereCenter += QVector3D(vertices[i*DATA_SIZE], vertices[i*DATA_SIZE+1], vertices[i*DATA_SIZE+2]);
    }
    boundingSphereCenter /= (float)(PATCH_VERTS * PATCH_VERTS);

    boundingSphereRadius = 0;
    for(int i = 0; i < PATCH_VERTS * PATCH_VERTS; i++){
        boundingSphereRadius = std::max(boundingSphereRadius, boundingSphereCenter.distanceToPoint(QVector3D(vertices[i*DATA_SIZE], vertices[i*DATA_SIZE+1], vertices[i*DATA_SIZE+2])));
    }
}

void WaterPatch::render(QMatrix4x4 model){
    Shader *waterShader = ShaderManager::getShader("waterShader");
    waterShader->use();
    waterShader->setMat4("model", model);
    waterShader->setInt("normalMapTexture", 0);
    waterShader->setFloat("time", GL::time/30.0f);
//    waterShader->setFloat("time", 15000.0f);
    waterShader->setFloat("radius", radius);
    waterShader->setInt("waveNumber", 10);
    GL::funcs.glBindBufferBase(GL_UNIFORM_BUFFER, 0, wavesBuffer);

    GL::funcs.glActiveTexture(GL_TEXTURE0);
    GL::funcs.glBindTexture(GL_TEXTURE_2D, normalMapTexture);

    GL::funcs.glBindVertexArray(VAO);
//    GL::funcs.glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    GL::funcs.glDrawElements(GL_TRIANGLES, indices->getSize(), GL_UNSIGNED_INT, 0);
    GL::funcs.glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void WaterPatch::generateNormalMap(){
    Shader *positionShader = ShaderManager::getShader("positionTextureShader");
    positionShader->use();

    positionShader->setFloat("multiplier", noise->getMultiplier());
    positionShader->setInt("permutation", 0);
    positionShader->setInt("octaves", 15);
    positionShader->setFloat("baseFrequency", radius * 10);
    positionShader->setFloat("persistance", 0.5f);
    positionShader->setFloat("lacunarity", 2.0f);

    unsigned int textureID = noise->getPerlinNoise()->getPermutationTexture();
    GL::funcs.glActiveTexture(GL_TEXTURE0);
    GL::funcs.glBindTexture(GL_TEXTURE_1D, textureID);

    float a = 1.0;
    int b = 0;
    QVector3D v0(relativePosition.x() + scale + scale * a / (NORMAL_MAP_SIZE + b),
                 relativePosition.y() + scale + scale * a / (NORMAL_MAP_SIZE + b),
                 1.0);
    QVector3D v1(relativePosition.x() + scale + scale * a / (NORMAL_MAP_SIZE + b),
                 relativePosition.y() - scale * a / (NORMAL_MAP_SIZE + b),
                 1.0);
    QVector3D v2(relativePosition.x() - scale * a / (NORMAL_MAP_SIZE + b),
                 relativePosition.y() - scale * a / (NORMAL_MAP_SIZE + b),
                 1.0);
    QVector3D v3(relativePosition.x() - scale * a / (NORMAL_MAP_SIZE + b),
                 relativePosition.y() + scale + scale * a / (NORMAL_MAP_SIZE + b),
                 1.0);
    v0 = relativeRotation.map(v0);
    v1 = relativeRotation.map(v1);
    v2 = relativeRotation.map(v2);
    v3 = relativeRotation.map(v3);

    float quadVertices[] = {
         1.0f,  1.0f, 0.0f,   v0.x(), v0.y(), v0.z(),
         1.0f, -1.0f, 0.0f,   v1.x(), v1.y(), v1.z(),
        -1.0f, -1.0f, 0.0f,   v2.x(), v2.y(), v2.z(),
        -1.0f,  1.0f, 0.0f,   v3.x(), v3.y(), v3.z()
    };
    unsigned int quadIndices[] = {
        0, 1, 3,
        1, 2, 3
    };

    unsigned int tmpVAO, tmpVBO, tmpEBO;

    GL::funcs.glGenVertexArrays(1, &tmpVAO);
    GL::funcs.glGenBuffers(1, &tmpVBO);
    GL::funcs.glGenBuffers(1, &tmpEBO);

    GL::funcs.glBindVertexArray(tmpVAO);

    GL::funcs.glBindBuffer(GL_ARRAY_BUFFER, tmpVBO);
    GL::funcs.glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    GL::funcs.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tmpEBO);
    GL::funcs.glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);

    GL::funcs.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    GL::funcs.glEnableVertexAttribArray(0);
    GL::funcs.glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    GL::funcs.glEnableVertexAttribArray(1);

    unsigned int fbo;
    GL::funcs.glGenFramebuffers(1, &fbo);
    GL::funcs.glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    unsigned int positionTexture;
    GL::funcs.glGenTextures(1, &positionTexture);
    GL::funcs.glBindTexture(GL_TEXTURE_2D, positionTexture);

    GL::funcs.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, NORMAL_MAP_SIZE + 2, NORMAL_MAP_SIZE + 2, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    GL::funcs.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    GL::funcs.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    GL::funcs.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    GL::funcs.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    GL::funcs.glBindTexture(GL_TEXTURE_2D, 0);

    GL::funcs.glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, positionTexture, 0);

    GL::funcs.glViewport(0,0,NORMAL_MAP_SIZE + 2, NORMAL_MAP_SIZE + 2);
    GL::funcs.glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    float coordDelta = 1.0f / (NORMAL_MAP_SIZE + 2);

    float texVertices[] = {
         1.0f,  1.0f, 0.0f,   1.0f - coordDelta, 1.0f - coordDelta,
         1.0f, -1.0f, 0.0f,   1.0f - coordDelta, coordDelta,
        -1.0f, -1.0f, 0.0f,   coordDelta, coordDelta,
        -1.0f,  1.0f, 0.0f,   coordDelta, 1.0f - coordDelta
    };

    unsigned int texIndices[] = {
        0, 1, 3,
        1, 2, 3
    };

    unsigned int texVAO, texVBO, texEBO;

    GL::funcs.glGenVertexArrays(1, &texVAO);
    GL::funcs.glGenBuffers(1, &texVBO);
    GL::funcs.glGenBuffers(1, &texEBO);

    GL::funcs.glBindVertexArray(texVAO);

    GL::funcs.glBindBuffer(GL_ARRAY_BUFFER, texVBO);
    GL::funcs.glBufferData(GL_ARRAY_BUFFER, sizeof(texVertices), texVertices, GL_STATIC_DRAW);

    GL::funcs.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, texEBO);
    GL::funcs.glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(texIndices), texIndices, GL_STATIC_DRAW);

    GL::funcs.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    GL::funcs.glEnableVertexAttribArray(0);
    GL::funcs.glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3* sizeof(float)));
    GL::funcs.glEnableVertexAttribArray(1);

    GL::funcs.glGenTextures(1, &this->normalMapTexture);
    GL::funcs.glBindTexture(GL_TEXTURE_2D, this->normalMapTexture);

    GL::funcs.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, NORMAL_MAP_SIZE, NORMAL_MAP_SIZE, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    GL::funcs.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    GL::funcs.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    GL::funcs.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    GL::funcs.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    GL::funcs.glBindTexture(GL_TEXTURE_2D, 0);

    GL::funcs.glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->normalMapTexture, 0);

    Shader *normalMapShader = ShaderManager::getShader("normalMapShader");
    normalMapShader->use();
    normalMapShader->setInt("positionTexture", 0);
    normalMapShader->setFloat("coordDelta", coordDelta);
    GL::funcs.glActiveTexture(GL_TEXTURE0);
    GL::funcs.glBindTexture(GL_TEXTURE_2D, positionTexture);

    GL::funcs.glViewport(0,0,NORMAL_MAP_SIZE, NORMAL_MAP_SIZE);
    GL::funcs.glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    GL::funcs.glBindFramebuffer(GL_FRAMEBUFFER, 0);


    //TODO: clear everything
    GL::funcs.glDeleteBuffers(1,&tmpVBO);
    GL::funcs.glDeleteBuffers(1,&tmpEBO);
    GL::funcs.glDeleteBuffers(1,&texVBO);
    GL::funcs.glDeleteBuffers(1,&texEBO);
    GL::funcs.glDeleteVertexArrays(1,&tmpVAO);
    GL::funcs.glDeleteVertexArrays(1,&texVAO);

    GL::funcs.glDeleteTextures(1, &positionTexture);

    GL::funcs.glDeleteFramebuffers(1, &fbo);
}
