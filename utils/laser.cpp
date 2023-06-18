#include "laser.h"

#include "../game/space.h"
#include "../opengl/gl.h"
#include "camera.h"
#include "consts.h"
#include "shader.h"
#include "shadermanager.h"

#include <QVector3D>
#include <QOpenGLFunctions_3_3_Core>

Laser::Laser(QVector3D dir, QVector3D center, float speed)
{
    this->dir = dir.normalized();
    this->center = center;
    this->speed = speed;

    GL::funcs.glGenVertexArrays(1, &VAO);
    GL::funcs.glGenBuffers(1, &VBO);

    GL::funcs.glBindVertexArray(VAO);

    GL::funcs.glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    GL::funcs.glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), NULL, GL_DYNAMIC_DRAW);

    GL::funcs.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    GL::funcs.glEnableVertexAttribArray(0);
    GL::funcs.glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    GL::funcs.glEnableVertexAttribArray(1);

    GL::funcs.glBindVertexArray(0);
}

Laser::~Laser(){
    GL::funcs.glDeleteBuffers(1, &VBO);
    GL::funcs.glDeleteVertexArrays(1, &VAO);
}

void Laser::render(Camera *camera){
    QVector3D v0,v1,v2,v3;
    QVector3D v = QVector3D::crossProduct(dir, center-camera->getPosition());
    v = v.normalized();

    QVector3D up = camera->getUp().normalized();
    QVector3D right = camera->getRight().normalized();
//    v0 = center + dir * LASER_LENGTH - v * LASER_WIDTH;
//    v1 = center + dir * LASER_LENGTH + v * LASER_WIDTH;
//    v2 = center - dir * LASER_LENGTH - v * LASER_WIDTH;
//    v3 = center - dir * LASER_LENGTH + v * LASER_WIDTH;

    v0 = center + right * LASER_RADIUS - up * LASER_RADIUS;
    v1 = center + right * LASER_RADIUS + up * LASER_RADIUS;
    v2 = center - right * LASER_RADIUS - up * LASER_RADIUS;
    v3 = center - right * LASER_RADIUS + up * LASER_RADIUS;

    float vertices[] = {
        v0.x(), v0.y(), v0.z(),  1, -1,
        v2.x(), v2.y(), v2.z(), -1, -1,
        v3.x(), v3.y(), v3.z(), -1,  1,
        v3.x(), v3.y(), v3.z(), -1,  1,
        v1.x(), v1.y(), v1.z(),  1,  1,
        v0.x(), v0.y(), v0.z(),  1, -1
    };

    GL::funcs.glBindVertexArray(VAO);
    GL::funcs.glBindBuffer(GL_ARRAY_BUFFER, VBO);

    GL::funcs.glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    Shader *shader = ShaderManager::getShader("laserShader");
    shader->use();

    GL::funcs.glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Laser::update(float deltaTime){
    this->center += this->dir * speed * deltaTime;
}

int Laser::checkCollisions(Space *space){
   if(space->checkCollisionPlanet(center))
       return PLANET_COLLISION;
   if(space->checkCollisionAsteroid(center))
       return ASTEROID_COLLISION;
    return 0;
}

QVector3D Laser::getCenter(){
    return center;
}
