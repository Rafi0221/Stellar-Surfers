#ifndef CAMERA_H
#define CAMERA_H

#include "../game/space.h"

#include <QVector3D>
#include <QMatrix4x4>

class Camera
{
public:
    Camera();

    QMatrix4x4 getViewMatrix();

    void setPosition(QVector3D position);
    QVector3D getPosition();

    QVector3D getFront();
    QVector3D getRight();
    QVector3D getUp();

    void move(float distance);

    void setYaw(float yaw);
    float getYaw();

    void setPitch(float pitch);
    float getPitch();

    void setRoll(float roll);
    float getRoll();

    void updateXYZ(float pitch, float yaw, float roll);
    void addAngles(float pitch, float yaw, float roll);
    void updateCameraVectors();

    void updateSpeed(float acceleration);
    void notifyCollision(int collisionType);
    float getSpeed();
private:
    QVector3D position;
    QVector3D front;
    QVector3D up;
    QVector3D right;

    float yaw;
    float pitch;
    float roll;

    float speed;
};

#endif // CAMERA_H
