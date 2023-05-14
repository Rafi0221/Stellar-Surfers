#ifndef TRIANGLEWINDOW_H
#define TRIANGLEWINDOW_H

#include <QOpenGLShaderProgram>
#include "../opengl/openglwindow.h"
#include "../play/setupgame.h"
#include "../connect/controllerupdater.h"
#include "space.h"

class Camera;
class Planet;
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

    Planet *planets[100];
    SkyBox *skybox;
    int counter = 0;
    SetupGame::GameSeed seed;

    //for testing
    Planet *planet;
};
#endif // TRIANGLEWINDOW_H
