#include "trianglewindow.h"

#include <QScreen>
#include "../opengl/gl.h"
#include "../skybox/skybox.h"
#include "../terrain/noisedterrain.h"
#include "../terrain/patchindices.h"
#include "../terrain/planet.h"
#include "../terrain/terrainface.h"
#include "../terrain/sphericalterrain.h"
#include "../utils/camera.h"
#include "../utils/shader.h"
#include "../utils/shadermanager.h"

void TriangleWindow::initialize()
{
    camera = new Camera();

//    shader = new Shader("shaders/testShader.vs", "shaders/testShader.fs");
//    GL::terrainShader = new Shader("shaders/terrainShader.vs", "shaders/terrainShader.fs");
//    GL::skyboxShader = new Shader("shaders/skyboxShader.vs", "shaders/skyboxShader.fs");
//    face = new TerrainFace(new SphericalTerrain());
    ShaderManager::initialize();
    PatchIndices::initialize();
//    planet = new Planet(5.0f);
//    planet->setPosition(QVector3D(0,0,15));
    for(int i = 0; i < 100; i++){
        planets[i] = new Planet(3.0f);
        planets[i]->setPosition(QVector3D(rand() % 150 - 75, rand()  % 150 - 75, rand() % 150 - 75));
    }
    skybox = new SkyBox();

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

void TriangleWindow::setSeed(SetupGame::GameSeed value) {
    seed = value;
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

//    camera->setPitch(-45);
//    camera->SetYaw(camera->GetYaw() + 0.6);
//    camera->setPosition(QVector3D(0.0, -15, 0));

//    camera->setRoll(camera->getRoll() + 0.6);
//    camera->move(0.03);

//    camera->updateXYZ(GL::rotation.x() / 3.0, GL::rotation.y() / 3.0, GL::rotation.z() / 3.0);
//    camera->updateCameraVectors();
    camera->addAngles(GL::rotation.x() / 3.0, GL::rotation.z() / 3.0, GL::rotation.y() / 3.0);
    camera->updateSpeed(GL::acceleration);
    camera->move(camera->getSpeed());

    counter++;
    QMatrix4x4 projection;
    projection.perspective(60.0f, 4.0f / 3.0f, 0.1f, 1000.0f);

    QMatrix4x4 view = camera->getViewMatrix();
    QMatrix3x3 view_ = view.normalMatrix();
//    view_.normalMatrix();
    QMatrix4x4 tmp(view_);
    Shader *skyboxShader = ShaderManager::getShader("skyboxShader");
    skyboxShader->use();

    skyboxShader->setMat4("projection", projection);
    skyboxShader->setMat4("view", tmp);
    skyboxShader->setVec3("seed", QVector3D(seed.f1, seed.f2, seed.f3));

    skybox->render();

    GL::funcs.glClear(GL_DEPTH_BUFFER_BIT);

//    Shader *testShader = ShaderManager::getShader("testShader");
//    testShader->use();

//    testShader->setMat4("projection", projection);
//    testShader->setMat4("view", view);

//    QMatrix4x4 model_;
//    model_.translate(0, 0, (float)counter/400);
//    model_.scale(0.05);
//    testShader->setMat4("model", model_);

//    GL::funcs.glBindVertexArray(VAO);
//    GL::funcs.glDrawArrays(GL_TRIANGLES, 0, 36);

    Shader *terrainShader = ShaderManager::getShader("terrainShader");
    terrainShader->use();

    terrainShader->setVec3("dirLight.direction", QVector3D(-0.2f, -1.0f, -0.3f));
    terrainShader->setVec3("dirLight.ambient", QVector3D(0.25f, 0.25f, 0.25f));
    terrainShader->setVec3("dirLight.diffuse", QVector3D(0.3f, 0.3f, 0.3f));
    terrainShader->setVec3("dirLight.specular", QVector3D(0.4f, 0.4f, 0.4f));

    terrainShader->setFloat("shininess", 2.0f);

    terrainShader->setVec3("viewPos", camera->getPosition());

    terrainShader->setMat4("projection", projection);
    terrainShader->setMat4("view", view);

//    planet->setRotation(QVector3D(0,(float)counter/20,0));
//    planet->update(QVector3D(0,0,(float)counter/400));
    for(int i = 0; i < 100; i++){
        planets[i]->update(camera->getPosition());
        planets[i]->render();
    }
//    planet->update(camera->getPosition());
//    planet->render();
}
