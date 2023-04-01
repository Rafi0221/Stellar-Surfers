#include "trianglewindow.h"

#include <QScreen>

void TriangleWindow::initialize()
{
    camera = new Camera();

    shader = new Shader("shaders/testShader.vs", "shaders/testShader.fs");
}

void TriangleWindow::render()
{
    QOpenGLFunctions *glFuncs = QOpenGLContext::currentContext()->functions();
    glFuncs->glEnable(GL_DEPTH_TEST);
    glFuncs->glDepthMask(GL_TRUE);

    const qreal retinaScale = devicePixelRatio();
    glFuncs->glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glFuncs->glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glFuncs->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader->use();

    static const GLfloat vertices[] = {
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

    static const GLfloat colors[] = {
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

    static const GLfloat translations[][3] = {
        {2, 0, 0},
        {-2, 0, 0},
        {0, 2, 0},
        {0, -2, 0},
        {0, 0, 2},
        {0, 0, -2},
    };
    srand(time(NULL));
//    camera->SetPitch(camera->GetPitch() + rand()%20);
//    camera->SetYaw(camera->GetYaw() + rand()%20);
    camera->SetRoll(camera->GetRoll() + rand()%20);

    for(int i = 0; i < 6 ; i++){
        QMatrix4x4 projection;
        projection.perspective(60.0f, 4.0f / 3.0f, 0.1f, 100.0f);
        shader->setMat4("projection", projection);

        QMatrix4x4 model;
        model.translate(translations[i][0], translations[i][1], translations[i][2]);
        shader->setMat4("model", model);

        QMatrix4x4 view = camera->GetViewMatrix();
        shader->setMat4("view", view);

        glFuncs->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
        glFuncs->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, colors);

        glFuncs->glEnableVertexAttribArray(0);
        glFuncs->glEnableVertexAttribArray(1);

        glFuncs->glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glFuncs->glDisableVertexAttribArray(0);
    glFuncs->glDisableVertexAttribArray(1);

    ++m_frame;
}
