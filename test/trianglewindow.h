#ifndef TRIANGLEWINDOW_H
#define TRIANGLEWINDOW_H

#include <QOpenGLShaderProgram>
#include "../opengl/openglwindow.h"

class Camera;
class Planet;
class Shader;
class TerrainFace;
class SkyBox;

class TriangleWindow : public OpenGLWindow
{
public:
    using OpenGLWindow::OpenGLWindow;

    void initialize() override;
    void render() override;

private:
    GLint m_posAttr = 0;
    GLint m_colAttr = 0;
    GLint m_matrixUniform = 0;

    int m_frame = 0;

    Camera *camera;

    unsigned int VAO, VBO;

    Planet *planets[100];
    SkyBox *skybox;
    int counter = 0;
};
#endif // TRIANGLEWINDOW_H
