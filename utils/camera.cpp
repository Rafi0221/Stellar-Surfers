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
    speed = 0.01f;
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

QVector3D Camera::getRight(){
    return right;
}

QVector3D Camera::getUp(){
    return up;
}

void Camera::move(float distance){
    position += front * distance;
}

void Camera::setYaw(float yaw){
    this->yaw = yaw;
}

float Camera::getYaw(){
    return yaw;
}

void Camera::setPitch(float pitch){
    this->pitch = pitch;
}

float Camera::getPitch(){
    return pitch;
}

void Camera::setRoll(float roll){
    this->roll = roll;
}

float Camera::getRoll(){
    return roll;
}

void Camera::updateXYZ(float x, float y, float z){
    setPitch(getPitch() + x);
    setRoll(getRoll() + y);
    setYaw(getYaw() + z);
}

void Camera::updateCameraVectors(){
    QMatrix4x4 rotationMatrix;
    QQuaternion q = QQuaternion::fromEulerAngles(pitch, yaw, roll);
    rotationMatrix.rotate(q);

    QVector3D front(0,0,1);
    QVector3D up(0,1,0);
    QVector3D right(1,0,0);

    this->front = rotationMatrix.map(front).normalized();
    this->up = rotationMatrix.map(up).normalized();
    this->right = rotationMatrix.map(right).normalized();
}

void Camera::addAngles(float pitch, float yaw, float roll){
    for(int i = 0; i < 20; i++){
        QMatrix4x4 rotationMatrix;
        rotationMatrix.rotate(pitch/20.0, right);
        rotationMatrix.rotate(yaw/20.0, up);
        rotationMatrix.rotate(roll/20.0, front);

        right = rotationMatrix.map(right);
        up = rotationMatrix.map(up);
        front = rotationMatrix.map(front);
    }
}

void Camera::updateSpeed(float a) {
    // const float slowdown = 0.000005f;
    a = a * 0.00009f;
    // we don't want to make negative speed even lower
    if (speed + a > 0 or a > 0)
        speed = speed + a;
}

float Camera::getSpeed() {
    return speed;
}

void Camera::notifyCollision() {
    if (speed > 0)
        speed=-speed*0.5;
}
