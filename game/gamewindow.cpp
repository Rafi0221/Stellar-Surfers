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
#include "../utils/collisionmanager.h"
#include "../utils/explosionmanager.h"
#include "../utils/frustum.h"
#include "../utils/laser.h"
#include "../utils/lasermanager.h"
#include "../utils/model.h"
#include "../utils/obj_loader.h"
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

    skybox = new SkyBox(new PerlinNoise(round(seed.f1 * 1000000.0)));

    laserManager = new LaserManager();
    explosionManager = new ExplosionManager();
//    explosionManager->addExplosion(QVector3D(0,0,1.5));
    collisionManager = new CollisionManager(camera, space, laserManager, explosionManager);
}

void GameWindow::setSeed(SetupGame::GameSeed value) {
    seed = value;

}

void GameWindow::setControllerUpdater(ControllerUpdater* controllerUpdater) {
    this->controllerUpdater = controllerUpdater;
}

void GameWindow::render()
{
    GL::updatesLeft = MAX_UPDATES_PER_FRAME;
    float deltaTime = clock() - oldTime;
    if(frameCounter == 0)
        deltaTime = 0;
    frameCounter++;
    if(frameCounter == 60){
        frameCounter = 0;
        qDebug() << (1.0 / deltaTime) * 1000.0;
    }
    oldTime = clock();

    camera->addAngles(GL::rotation.x() / 3.0, GL::rotation.z() / 3.0, GL::rotation.y() / 3.0);
    camera->updateSpeed(GL::acceleration);
    camera->move(camera->getSpeed() * deltaTime);

    space->update(camera->getPosition());

    controllerUpdater->update(
            camera->getPosition(),
            camera->getSpeed(),
            space->checkCollision(camera->getPosition()),
            space->collisionAheadPlanet(camera->getPosition(), camera->getFront(), 200, 0),
            space->collisionAheadAsteroid(camera->getPosition(), camera->getFront(), 100, 7)
    );

    collisionManager->update();

    cooldown -= deltaTime;
//    GL::shoot = true;
    if(GL::shoot){
        if(cooldown <= 0.0){
            cooldown = LASER_COOLDOWN;
            qDebug() << "shooting";
            QVector3D startPointRight = camera->getPosition() - camera->getUp() * 0.5 - camera->getRight() * 0.5;
            QVector3D startPointLeft = camera->getPosition() - camera->getUp() * 0.5 + camera->getRight() * 0.5;
            laserManager->addLaser(startPointRight, camera->getPosition() + 150 * camera->getFront() - startPointRight,
                                   camera->getSpeed());
            laserManager->addLaser(startPointLeft, camera->getPosition() + 150 * camera->getFront() - startPointLeft,
                                   camera->getSpeed());
        }
        GL::shoot = false;
    }

    laserManager->update(deltaTime);

    explosionManager->update(camera, deltaTime);


    //RENDERING


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
    Shader *textureShader = ShaderManager::getShader("textureShader");
    textureShader->use();
    textureShader->setMat4("projection", projection);
    textureShader->setMat4("view", tmp);

    skybox->render();

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

    Shader *asteroidShader = ShaderManager::getShader("asteroidShader");
    asteroidShader->use();
    asteroidShader->setVec3("dirLight.direction", QVector3D(-0.2f, -1.0f, -0.3f));
    asteroidShader->setVec3("dirLight.ambient", QVector3D(0.5f, 0.5f, 0.5f));
    asteroidShader->setVec3("dirLight.diffuse", QVector3D(0.3f, 0.3f, 0.3f));
    asteroidShader->setVec3("dirLight.specular", QVector3D(0.4f, 0.4f, 0.4f));
    asteroidShader->setFloat("shininess", 16.0f);
    asteroidShader->setVec3("viewPos", camera->getPosition());
    asteroidShader->setMat4("projection", projection);
    asteroidShader->setMat4("view", view);

    Frustum frustum(camera, 4.0f / 3.0f, 1.04719755f, 0.1f, 1000.0f);

    space->render(asteroidShader, &frustum);

    GL::funcs.glEnable(GL_BLEND);
    GL::funcs.glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    Shader *laserShader = ShaderManager::getShader("laserShader");
    laserShader->use();
    laserShader->setMat4("view", view);
    laserShader->setMat4("projection", projection);

    laserManager->render(camera);

    Shader *explosionShader = ShaderManager::getShader("explosionShader");

    GL::funcs.glActiveTexture(GL_TEXTURE0);
    GL::funcs.glBindTexture(GL_TEXTURE_2D, load_texture("media/explosions2.png"));

    explosionShader->use();

    explosionShader->setInt("texture1", 0);
    explosionShader->setMat4("view", view);
    explosionShader->setMat4("projection", projection);

    explosionManager->render();

    GL::funcs.glDisable(GL_BLEND);





}
