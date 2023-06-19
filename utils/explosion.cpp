#include "explosion.h"

#include "../opengl/gl.h"
#include "camera.h"
#include "consts.h"
#include "shader.h"
#include "shadermanager.h"

#include <qopenglfunctions_3_3_core.h>

#include<random>

#define PARTICLE_SIZE 0.05
#define PARTICLE_SPEED 0.48
#define AMOUNT 5000

float texDelta[][2] = {
    {0,0},
    {0.5, 0},
    {0.0, 0.75},
    {0.75, 0}
};

float sizes[][2] = {
    {0.01, 0.01},
    {0.1, 0.1},
    {0.06, 0.06},
    {0.1, 0.1},
};

float speeds[] = {
    0.3,
    0.2,
    0.24,
    0.44
};

float speedDelta[] = {
    0.03,
    0.01,
    0.02,
    0.03
};

float getRand(){
    return (((rand()%1000) / 1000.0) - 0.5) * 2.0;
}

Explosion::Explosion(QVector3D position)
{
    for(int i = 0; i < AMOUNT; i++){
        velocities[i] = QVector3D(getRand(), getRand(), getRand());
        velocities[i].normalize();
        velocities[i] *= getRand();
        positions[i] = position + velocities[i] * abs(getRand()) * 2;

        angularVelocities[i] = getRand();

        if(i < AMOUNT/4)
            type[i] = 0;
        else if(i < AMOUNT/2){
            type[i] = 1;
        }else if(i < 3 * AMOUNT / 4){
            type[i] = 2;
        }else{
            type[i] = 3;
            angularVelocities[i] = 0.0f;
        }
    }
    GL::funcs.glGenVertexArrays(1, &VAO);
    GL::funcs.glGenBuffers(1, &fVBO);
    GL::funcs.glGenBuffers(1, &iVBO);

    GL::funcs.glBindVertexArray(VAO);

    GL::funcs.glBindBuffer(GL_ARRAY_BUFFER, fVBO);

    GL::funcs.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    GL::funcs.glEnableVertexAttribArray(0);
    GL::funcs.glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    GL::funcs.glEnableVertexAttribArray(1);

    GL::funcs.glBindBuffer(GL_ARRAY_BUFFER, iVBO);

    GL::funcs.glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(float), (void*)0);
    GL::funcs.glEnableVertexAttribArray(2);

    GL::funcs.glBindVertexArray(0);
}

void Explosion::update(Camera *camera, float deltaTime){
    deltaTime /= 1000.0;
    currentTime += deltaTime;
    std::vector<std::pair<float, int>> V;
    for(int i = 0; i < AMOUNT; i++){
        int t = type[i];
        positions[i] += velocities[i] * deltaTime * (speeds[t] - speedDelta[t] * currentTime);
        rotations[i] += angularVelocities[i] * deltaTime * 25;
//        positions[i] = camera->getPosition() + camera->getFront() * 1;
        V.push_back({positions[i].distanceToPoint(camera->getPosition()), i});

    }
    sort(V.begin(), V.end());
    reverse(V.begin(), V.end());
    for(int i = 0; i < AMOUNT; i++){
        int id = V[i].second;
        int t = type[id];

        QMatrix4x4 model;
        model.rotate(rotations[id], camera->getFront());

        QVector3D up = model.map(camera->getUp()).normalized();
        QVector3D right = model.map(camera->getRight()).normalized();

        QVector3D v0, v1, v2, v3;

        if(t == 3){
            QVector3D v = QVector3D::crossProduct(velocities[id], positions[id]-camera->getPosition());
            v0 = positions[id] + velocities[id] * sizes[t][0] - v * sizes[t][1];
            v1 = positions[id] + velocities[id] * sizes[t][0] + v * sizes[t][1];
            v2 = positions[id] - velocities[id] * sizes[t][0] - v * sizes[t][1];
            v3 = positions[id] - velocities[id] * sizes[t][0] + v * sizes[t][1];
        }else{
            v0 = positions[id] + right * sizes[t][0] - up * sizes[t][1];
            v1 = positions[id] + right * sizes[t][0] + up * sizes[t][1];
            v2 = positions[id] - right * sizes[t][0] - up * sizes[t][1];
            v3 = positions[id] - right * sizes[t][0] + up * sizes[t][1];
        }

        float vertices[] = {
            v0.x(), v0.y(), v0.z(),  0,  1,
            v2.x(), v2.y(), v2.z(),  1,  1,
            v3.x(), v3.y(), v3.z(),  1,  0,
            v3.x(), v3.y(), v3.z(),  1,  0,
            v1.x(), v1.y(), v1.z(),  0,  0,
            v0.x(), v0.y(), v0.z(),  0,  1
        };

        for(int j = 0; j < 6; j++){
            vertices[j * 5 + 3] *= 0.25;
            vertices[j * 5 + 3] += texDelta[t][0];
            vertices[j * 5 + 4] *= 0.25;
            vertices[j * 5 + 4] += texDelta[t][1];
        }

        float types[] = {
            (float)t,
            (float)t,
            (float)t,
            (float)t,
            (float)t,
            (float)t
        };

        memcpy(data + i * 30, vertices, 30 * sizeof(float));
        memcpy(data2 + i * 6, types, 6 * sizeof(float));
    }
    GL::funcs.glBindVertexArray(VAO);

    GL::funcs.glBindBuffer(GL_ARRAY_BUFFER, fVBO);
    GL::funcs.glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_DYNAMIC_DRAW);

    GL::funcs.glBindBuffer(GL_ARRAY_BUFFER, iVBO);
    GL::funcs.glBufferData(GL_ARRAY_BUFFER, sizeof(data2), data2, GL_DYNAMIC_DRAW);
}

void Explosion::render(){
    Shader *explosionShader = ShaderManager::getShader("explosionShader");
    explosionShader->use();
    explosionShader->setFloat("fade", 1.0-(currentTime/3.0f));
    GL::funcs.glBindVertexArray(VAO);
    GL::funcs.glDrawArrays(GL_TRIANGLES, 0, 6 * AMOUNT);
}
