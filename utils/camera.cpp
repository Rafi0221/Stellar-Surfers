#include "camera.h"

#include <QtMath>
#include <QMatrix4x4>

Camera::Camera()
{
    Position = QVector3D(0,0,0);
    WorldUp = QVector3D(0,1,0);
    Yaw = 0;
    Pitch = 0;
    Roll = 0;
    UpdateCameraVectors();
}

QMatrix4x4 Camera::GetViewMatrix(){
    QMatrix4x4 result;
    result.lookAt(Position, Position + Front, Up);
    return result;
}

void Camera::SetPosition(QVector3D position){
    Position = position;
}

QVector3D Camera::GetPosition(){
    return Position;
}

QVector3D Camera::GetFront(){
    return Front;
}

void Camera::SetYaw(float yaw){
    Yaw = yaw;
    UpdateCameraVectors();
}

float Camera::GetYaw(){
    return Yaw;
}

void Camera::SetPitch(float pitch){
    Pitch = pitch;
    UpdateCameraVectors();
}

float Camera::GetPitch(){
    return Pitch;
}

void Camera::SetRoll(float roll){
    Roll = roll;
    UpdateCameraVectors();
}

float Camera::GetRoll(){
    return Roll;
}

void Camera::UpdateCameraVectors(){
    QMatrix4x4 rotationMatrix;
    rotationMatrix.rotate(Pitch, 1, 0, 0);
    rotationMatrix.rotate(Yaw, 0, 1, 0);
    rotationMatrix.rotate(Roll, 0, 0, 1);

    QVector3D front(0,0,1);
    QVector3D up(0,1,0);
    QVector3D right(1,0,0);

    Front = rotationMatrix.map(front).normalized();
    Up = rotationMatrix.map(up).normalized();
    Right = rotationMatrix.map(right).normalized();
}
