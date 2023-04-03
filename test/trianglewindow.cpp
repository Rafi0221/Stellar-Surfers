#include "trianglewindow.h"

#include <QScreen>
#include "../opengl/gl.h"
#include "../terrain/noisedterrain.h"
#include "../terrain/planet.h"
#include "../terrain/terrainface.h"
#include "../terrain/sphericalterrain.h"
#include "../utils/camera.h"
#include "../utils/shader.h"


void TriangleWindow::initialize()
{
    camera = new Camera();

    shader = new Shader("shaders/testShader.vs", "shaders/testShader.fs");
    GL::terrainShader = new Shader("shaders/terrainShader.vs", "shaders/terrainShader.fs");

//    face = new TerrainFace(new SphericalTerrain());
    planet = new Planet(new NoisedTerrain());
    planet->SetPosition(QVector3D(0,0,1.5));

    GLfloat vertices[] = {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f
    };

    GLfloat colors[] = {
        0.5f, 0.5f, 0.0f,
        0.0f, 0.5f, 0.5f,
        0.5f, 0.0f, 0.5f,
        0.5f, 0.5f, 0.0f,
        0.0f, 0.5f, 0.5f,
        0.5f, 0.0f, 0.5f,

        0.5f, 0.5f, 0.0f,
        0.0f, 0.5f, 0.5f,
        0.5f, 0.0f, 0.5f,
        0.5f, 0.5f, 0.0f,
        0.0f, 0.5f, 0.5f,
        0.5f, 0.0f, 0.5f,

        0.5f, 0.5f, 0.0f,
        0.0f, 0.5f, 0.5f,
        0.5f, 0.0f, 0.5f,
        0.5f, 0.5f, 0.0f,
        0.0f, 0.5f, 0.5f,
        0.5f, 0.0f, 0.5f,

        0.5f, 0.5f, 0.0f,
        0.0f, 0.5f, 0.5f,
        0.5f, 0.0f, 0.5f,
        0.5f, 0.5f, 0.0f,
        0.0f, 0.5f, 0.5f,
        0.5f, 0.0f, 0.5f,

        0.5f, 0.5f, 0.0f,
        0.0f, 0.5f, 0.5f,
        0.5f, 0.0f, 0.5f,
        0.5f, 0.5f, 0.0f,
        0.0f, 0.5f, 0.5f,
        0.5f, 0.0f, 0.5f,

        0.5f, 0.5f, 0.0f,
        0.0f, 0.5f, 0.5f,
        0.5f, 0.0f, 0.5f,
        0.5f, 0.5f, 0.0f,
        0.0f, 0.5f, 0.5f,
        0.5f, 0.0f, 0.5f,
    };

    GLfloat data[36*6];
    for(int i = 0; i < 36; i++){
        data[i*6] = vertices[i*3];
        data[i*6+1] = vertices[i*3+1];
        data[i*6+2] = vertices[i*3+2];

        data[i*6+3] = colors[i*3];
        data[i*6+4] = colors[i*3+1];
        data[i*6+5] = colors[i*3+2];
    }

    GL::funcs.glGenVertexArrays(1, &VAO);
    GL::funcs.glGenBuffers(1, &VBO);

    GL::funcs.glBindVertexArray(VAO);

    GL::funcs.glBindBuffer(GL_ARRAY_BUFFER, VBO);
    GL::funcs.glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

    // position attribute
    GL::funcs.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    GL::funcs.glEnableVertexAttribArray(0);
    // color attribute
    GL::funcs.glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    GL::funcs.glEnableVertexAttribArray(1);
}

void TriangleWindow::render()
{
    GL::funcs.glEnable(GL_DEPTH_TEST);
    GL::funcs.glDepthMask(GL_TRUE);

    const qreal retinaScale = devicePixelRatio();
    GL::funcs.glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    GL::funcs.glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    GL::funcs.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GLfloat translations[][3] = {
        {2, 0, 0},
        {-2, 0, 0},
        {0, 2, 0},
        {0, -2, 0},
        {0, 0, 2},
        {0, 0, -2},
    };

    srand(time(NULL));
//    camera->SetPitch(camera->GetPitch() + 20);
//    camera->SetYaw(camera->GetYaw() + 20);
//    camera->SetRoll(camera->GetRoll() + 20);

    camera->SetPitch(45);
    camera->SetPosition(QVector3D(0, 1.5, 0));

    counter++;

    shader->use();
    QMatrix4x4 projection_;
    projection_.perspective(60.0f, 4.0f / 3.0f, 0.1f, 100.0f);
    shader->setMat4("projection", projection_);

    QMatrix4x4 model_;
    model_.translate(0, 0, (float)counter/400);
    model_.scale(0.05);
    shader->setMat4("model", model_);

    QMatrix4x4 view_ = camera->GetViewMatrix();
    shader->setMat4("view", view_);


    GL::funcs.glBindVertexArray(VAO);
    GL::funcs.glDrawArrays(GL_TRIANGLES, 0, 36);

    GL::terrainShader->use();
    QMatrix4x4 projection;
    projection.perspective(60.0f, 4.0f / 3.0f, 0.1f, 100.0f);
    GL::terrainShader->setMat4("projection", projection);

    QMatrix4x4 model;

    model.translate(0,0,1.5);

    GL::terrainShader->setMat4("model", model);

    QMatrix4x4 view = camera->GetViewMatrix();
    GL::terrainShader->setMat4("view", view);

    planet->Update(QVector3D(0,0,(float)counter/400));
    planet->Render();
//    face->Update(camera->GetPosition() + QVector3D(0,0,(float)counter/200),model);
//    face->Render();
}
