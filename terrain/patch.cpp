#include "patch.h"

#include <QVector2D>
#include <QVector3D>
#include <QMatrix4x4>

QVector3D Patch::getCenter(){
    return boundingSphereCenter;
}

float Patch::getRadius(){
    return boundingSphereRadius;
}

QVector3D Patch::cubeToSphere(QVector3D position, float r){
    float x2 = position.x() * position.x();
    float y2 = position.y() * position.y();
    float z2 = position.z() * position.z();

    float x3 = position.x() * sqrt(1 - y2/2.0 - z2/2.0 + z2*y2/3.0);
    float y3 = position.y() * sqrt(1 - z2/2.0 - x2/2.0 + z2*x2/3.0);
    float z3 = position.z() * sqrt(1 - x2/2.0 - y2/2.0 + x2*y2/3.0);

    return QVector3D(x3,y3,z3) * r;
}

