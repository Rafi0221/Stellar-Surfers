#include "laser.h"

#include "../opengl/gl.h"
#include "camera.h"
#include "consts.h"
#include "shader.h"
#include "shadermanager.h"

#include <QVector3D>
#include <QOpenGLFunctions_3_3_Core>

Laser::Laser(QVector3D dir, QVector3D center)
{
    this->dir = dir.normalized();
    this->center = center;

    GL::funcs.glGenVertexArrays(1, &VAO);
    GL::funcs.glGenBuffers(1, &VBO);

    GL::funcs.glBindVertexArray(VAO);

    GL::funcs.glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    GL::funcs.glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), NULL, GL_DYNAMIC_DRAW);

    GL::funcs.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    GL::funcs.glEnableVertexAttribArray(0);
    GL::funcs.glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(3 * sizeof(float)));
    GL::funcs.glEnableVertexAttribArray(1);

    GL::funcs.glBindVertexArray(0);
}

void Laser::Render(Camera *camera){
    QVector3D v0,v1,v2,v3;
    QVector3D v = QVector3D::crossProduct(dir, center-camera->getPosition());
    v = v.normalized();

    v0 = center + dir * LASER_LENGTH - v * LASER_WIDTH;
    v1 = center + dir * LASER_LENGTH + v * LASER_WIDTH;
    v2 = center - dir * LASER_LENGTH - v * LASER_WIDTH;
    v3 = center - dir * LASER_LENGTH + v * LASER_WIDTH;

    float vertices[] = {
        v0.x(), v0.y(), v0.z(), -1,
        v2.x(), v2.y(), v2.z(), -1,
        v3.x(), v3.y(), v3.z(), 1,
        v3.x(), v3.y(), v3.z(), 1,
        v1.x(), v1.y(), v1.z(), 1,
        v0.x(), v0.y(), v0.z(), -1
    };

    GL::funcs.glBindVertexArray(VAO);
    GL::funcs.glBindBuffer(GL_ARRAY_BUFFER, VBO);

    GL::funcs.glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    Shader *shader = ShaderManager::getShader("laserShader");
    shader->use();

    GL::funcs.glDrawArrays(GL_TRIANGLES, 0, 6);
}
