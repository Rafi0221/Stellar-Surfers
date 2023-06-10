#ifndef FRUSTUM_H
#define FRUSTUM_H

#include "camera.h"

#include <QVector3D>

struct Plane{
    QVector3D normal;
    float distance;

    Plane(){

    }

    Plane(QVector3D point, QVector3D normal){
        this->normal = normal.normalized();
        this->distance = QVector3D::dotProduct(this->normal, point);
    }

//    float getDistanceToPlane(QVector3D point){
//        return QVector3D::dotProduct(normal   , point) - distance;
//    }

    bool isSphereOnPlane(QVector3D center, float radius){
        return (QVector3D::dotProduct(normal, center) - distance) > -radius;
    }
};

struct Frustum{
    Plane topFace;
    Plane bottomFace;

    Plane rightFace;
    Plane leftFace;

    Plane farFace;
    Plane nearFace;

    Frustum(Camera *camera, float aspect, float fovY, float zNear, float zFar){
        float halfVSide = zFar * tanf(fovY * 0.5f);
        float halfHSide = halfVSide * aspect;
//        qDebug() << halfVSide << halfHSide;
        QVector3D frontMultFar = zFar * camera->getFront();

        nearFace = Plane(camera->getPosition() + zNear * camera->getFront(), camera->getFront());
        farFace = Plane(camera->getPosition() + frontMultFar, -camera->getFront());
        rightFace = Plane(camera->getPosition(), QVector3D::crossProduct(frontMultFar + camera->getRight() * halfHSide, camera->getUp()));
        leftFace = Plane(camera->getPosition(), QVector3D::crossProduct(camera->getUp(), frontMultFar - camera->getRight() * halfHSide));
        topFace = Plane(camera->getPosition(), QVector3D::crossProduct(camera->getRight(), frontMultFar + camera->getUp() * halfVSide));
        bottomFace = Plane(camera->getPosition(), QVector3D::crossProduct(frontMultFar - camera->getUp() * halfVSide, camera->getRight()));
//        qDebug() << rightFace.normal << rightFace.distance << leftFace.normal << leftFace.distance << topFace.normal << topFace.distance << bottomFace.normal << bottomFace.distance;
    }

    bool isSphereInside(QVector3D center, float radius){
        return leftFace.isSphereOnPlane(center, radius) &&
               rightFace.isSphereOnPlane(center, radius) &&
               farFace.isSphereOnPlane(center, radius) &&
               nearFace.isSphereOnPlane(center, radius) &&
               topFace.isSphereOnPlane(center, radius) &&
               bottomFace.isSphereOnPlane(center, radius);
    }
};

#endif // FRUSTUM_H
