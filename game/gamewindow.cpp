#include "gamewindow.h"

#include <QScreen>
#include "../opengl/gl.h"
#include "../skybox/skybox.h"
#include "../terrain/noisedpatchfactory.h"
#include "../terrain/waterpatchfactory.h"
#include "../terrain/patchindices.h"
#include "../terrain/planet.h"
#include "../terrain/planetlayer.h"
#include "../terrain/planetproperties.h"
#include "../utils/camera.h"
#include "../utils/shader.h"
#include "../utils/shadermanager.h"
#include "../utils/perlinnoise.h"

void GameWindow::initialize()
{
    camera = new Camera();

    ShaderManager::initialize();
    PatchIndices::initialize();

//    planet = new Planet(2137, 5);
//    planet->setPosition(QVector3D(0,0,13));

    space = new Space(seed.seed);
    space->initialize();

    skybox = new SkyBox();

}

void GameWindow::setSeed(SetupGame::GameSeed value) {
    seed = value;
}

void GameWindow::setControllerUpdater(ControllerUpdater* controllerUpdater) {
    this->controllerUpdater = controllerUpdater;
}

void GameWindow::render()
{
    camera->addAngles(GL::rotation.x() / 3.0, GL::rotation.z() / 3.0, GL::rotation.y() / 3.0);
    camera->updateSpeed(GL::acceleration);
    camera->move(camera->getSpeed());

    space->update(camera->getPosition());

    GL::funcs.glEnable(GL_DEPTH_TEST);
    GL::funcs.glDepthMask(GL_TRUE);

    const qreal retinaScale = devicePixelRatio();
    GL::funcs.glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    GL::funcs.glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    GL::funcs.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    srand(time(NULL));

    counter++;
    GL::time = counter;
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
    PerlinNoise noise(round(seed.f1 * 1000000.0));
    unsigned int textureID = noise.getPermutationTexture();
    skyboxShader->setInt("permutation", 0);
    GL::funcs.glActiveTexture(GL_TEXTURE0);
    GL::funcs.glBindTexture(GL_TEXTURE_1D, textureID);
//    skybox->render();

    GL::funcs.glClear(GL_DEPTH_BUFFER_BIT);

    Shader *terrainShader = ShaderManager::getShader("terrainShader");
    terrainShader->use();
    terrainShader->setVec3("dirLight.direction", QVector3D(-0.2f, -1.0f, -0.3f));
    terrainShader->setVec3("dirLight.ambient", QVector3D(0.5f, 0.5f, 0.5f));
    terrainShader->setVec3("dirLight.diffuse", QVector3D(0.3f, 0.3f, 0.3f));
    terrainShader->setVec3("dirLight.specular", QVector3D(0.4f, 0.4f, 0.4f));
    terrainShader->setFloat("shininess", 8.0f);
    terrainShader->setVec3("viewPos", camera->getPosition());
    terrainShader->setMat4("projection", projection);
    terrainShader->setMat4("view", view);

    Shader *waterShader = ShaderManager::getShader("waterShader");
    waterShader->use();
    waterShader->setVec3("dirLight.direction", QVector3D(-0.2f, -1.0f, -0.3f));
    waterShader->setVec3("dirLight.ambient", QVector3D(0.5f, 0.5f, 0.5f));
    waterShader->setVec3("dirLight.diffuse", QVector3D(0.3f, 0.3f, 0.3f));
    waterShader->setVec3("dirLight.specular", QVector3D(0.4f, 0.4f, 0.4f));
    waterShader->setFloat("shininess", 32.0f);
    waterShader->setVec3("viewPos", camera->getPosition());
    waterShader->setMat4("projection", projection);
    waterShader->setMat4("view", view);

    space->render();
//    planet->setRotation(QVector3D(0, counter/10.0f, 0));
//    planet->update(camera->getPosition());
//    planet->render();
//    planet2->setRotation(QVector3D(0, 55, 0));
//    planet2->update(camera->getPosition());
//    planet2->render();

//    skybox->render();

//    if(space->checkCollision(camera->getPosition())) {
//        qDebug() << "ALARAM!!!";
//    }

//    if(planet->checkCollision(camera->getPosition())) {
//        qDebug() << "ALARAM!!!";
//    }
//    else qDebug() << "no collision";

//    controllerUpdater->update(camera->getPosition(), camera->getSpeed());
}
