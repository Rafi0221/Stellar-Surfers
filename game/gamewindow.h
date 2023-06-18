#ifndef TRIANGLEWINDOW_H
#define TRIANGLEWINDOW_H

#include <QOpenGLShaderProgram>
#include "../opengl/openglwindow.h"
#include "../play/setupgame.h"
#include "../connect/controllerupdater.h"
#include "space.h"

class Camera;
class CollisionManager;
class ExplosionManager;
class LaserManager;
class Model;
class Planet;
class PlanetLayer;
class Shader;
class TerrainFace;
class SkyBox;

class GameWindow : public OpenGLWindow
{
public:
    using OpenGLWindow::OpenGLWindow;

    void initialize() override;
    void render() override;
    void setSeed(SetupGame::GameSeed);
    void setControllerUpdater(ControllerUpdater* controllerUpdater);

private:
    GLint m_posAttr = 0;
    GLint m_colAttr = 0;
    GLint m_matrixUniform = 0;

    int m_frame = 0;

    ControllerUpdater* controllerUpdater;
    Camera *camera;
    Space *space;

    unsigned int VAO, VBO;

    PlanetLayer *planets[100];
    SkyBox *skybox;
    int counter = 0;
    SetupGame::GameSeed seed;

    //for testing
    Planet *planet;

    int frameCounter = 0;
    float oldTime = 0.0f;

    float cooldown = 0.0f;
    LaserManager *laserManager;
    CollisionManager *collisionManager;
    ExplosionManager *explosionManager;
};
#endif // TRIANGLEWINDOW_H
