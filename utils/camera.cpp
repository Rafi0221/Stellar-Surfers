#include "camera.h"

#include <QtMath>
#include <QMatrix4x4>

Camera::Camera()
{
    position = QVector3D(0,0,0);
    yaw = 0;
    pitch = 0;
    roll = 0;
    updateCameraVectors();
}

QMatrix4x4 Camera::getViewMatrix(){
    QMatrix4x4 result;
    result.lookAt(position, position + front, up);
    return result;
}

void Camera::setPosition(QVector3D position){
    this->position = position;
}

QVector3D Camera::getPosition(){
    return position;
}

QVector3D Camera::getFront(){
    return front;
}

void Camera::move(float distance){
    position += front * distance;
}

void Camera::setYaw(float yaw){
    this->yaw = yaw;
    updateCameraVectors();
}

float Camera::getYaw(){
    return yaw;
}

void Camera::setPitch(float pitch){
    this->pitch = pitch;
    updateCameraVectors();
}

float Camera::getPitch(){
    return pitch;
}

void Camera::setRoll(float roll){
    this->roll = roll;
    updateCameraVectors();
}

float Camera::getRoll(){
    return roll;
}

void Camera::updateCameraVectors(){
    QMatrix4x4 rotationMatrix;
    rotationMatrix.rotate(pitch, 1, 0, 0);
    rotationMatrix.rotate(yaw, 0, 1, 0);
    rotationMatrix.rotate(roll, 0, 0, 1);

    QVector3D front(0,0,1);
    QVector3D up(0,1,0);
    QVector3D right(1,0,0);

    this->front = rotationMatrix.map(front).normalized();
    this->up = rotationMatrix.map(up).normalized();
    this->right = rotationMatrix.map(right).normalized();
}
